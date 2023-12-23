#!/usr/bin/env bash

circuit="c3540"
runtime_threshold=10.0
vm_threshold=1000000
timeout_time="20s"

print_usage () {
    echo "Usage: $0 [-c benchmark_name] [-r runtime_threshold] [-m memory_threshold] [-t timeout_time]"
    echo "Example: $0 -c c432 -r 1.0 -m 50000 -t 10s"
}

while getopts "hc:r:m:t:" option; do
    case $option in
        c) circuit="$OPTARG";;
        r) runtime_threshold="$OPTARG";;
        m) vm_threshold="$OPTARG";;
        t) timeout_time="$OPTARG";;
        h)
            print_usage
            exit 0
            ;;
        *)
            print_usage
            exit 1
            ;;
    esac
done

cd "$(dirname "${BASH_SOURCE[0]}")"
if ! [[ -d ../../build ]]; then
    >&2 echo "[ERROR] Could not find build directory."
    exit 1
fi
cd "../../build"
if ! [[ -f "../benchmarks/iscas85/$circuit.bench" ]]; then
    >&2 echo "[ERROR] Could not find benchmark."
    exit 1
fi

echo "[INFO] Running ISCAS'85 $circuit benchmark..."
time_start=$(date +%s%3N)
bench_result=$(./src/bench/VDSProject_bench "../benchmarks/iscas85/$circuit.bench")
time_end=$(date +%s%3N)
time_ms=$(( $time_end - $time_start ))
if ! [[ -d "results_$circuit/txt" ]]; then
    >&2 echo "[ERROR] Could not find reference results."
    exit 1
fi

within_requirements=true
runtime_regex="Runtime: ([0-9]+.[0-9]+)"
if [[ $bench_result =~ $runtime_regex ]]; then
    runtime="${BASH_REMATCH[1]}"
    echo "[!] Runtime: $runtime"
else
    >&2 echo "[ERROR] Could not match benchmark runtime."
    exit 1
fi
vm_regex="VM: ([0-9]*);"
if [[ $bench_result =~ $vm_regex ]]; then
    vm="${BASH_REMATCH[1]}"
    echo "[!] VM usage: $vm"
else
    >&2 echo "[ERROR] Could not match benchmark VM usage."
    exit 1
fi

if [ "`echo "${runtime} < ${runtime_threshold}" | bc`" -ne 1 ]; then
    >&2 echo "[ERROR] Benchmark is not within runtime requirements ($runtime >= $runtime_threshold)."
    within_requirements=false
fi
if [ "`echo "${vm} < ${vm_threshold}" | bc`" -ne 1 ]; then
    >&2 echo "[ERROR] Benchmark is not within VM usage requirements ($vm >= $vm_threshold)."
    within_requirements=false
fi
if [ "$within_requirements" = false ]; then
    exit 1
else
    echo "[SUCCESS] Runtime and VM usage are within the requirements."
fi

echo "[INFO] Finished executing benchmark! ($time_ms ms)"

i=1
files=(results_$circuit/txt/*)
total_files=${#files[@]}
echo "[INFO] Running a total of $total_files verifications."
for file in ${files[*]} ; do
    filename=$(echo "${file##*/}")
    echo -n "[$i/$total_files] Verifying equivalence of $filename circuit... "
    if ! [[ -f "../benchmarks/references/results_$circuit/txt/$filename" ]]; then
        echo "No reference file."
        echo "[WARN] Aborting subsequent verifications due to incorrect reference files."
        break
    fi

    time_start=$(date +%s%3N)
    result=$(timeout $timeout_time ./src/verify/VDSProject_verify results_$circuit/txt/$filename ../benchmarks/references/results_$circuit/txt/$filename || echo "Timeout.")
    time_end=$(date +%s%3N)
    time_ms=$(( $time_end - $time_start ))

    if [ "$result" = "Not Equivalent!" ]; then
        echo "Not equivalent! ($time_ms ms)"
        >&2 echo "[ERROR] Verification failed! Exiting."
        exit 1
    elif [ "$result" = "Timeout." ]; then
        echo "$result ($time_ms ms)"
        echo "[WARN] Aborting subsequent verifications due to timeout."
        break
    fi

    echo "$result ($time_ms ms)"
    ((i++))
done;

total=$(( i - 1 ))
if (( total == total_files )); then
    echo "[SUCCESS] Verified the equivalence of $total_files circuits!"
else
    echo "[WARN] Could not verify the equivalence of $(( total_files - total )) out of $total_files circuits."
fi
