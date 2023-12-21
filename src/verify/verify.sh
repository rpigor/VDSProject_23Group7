#!/usr/bin/env bash

circuit=${1:-c3540}
timeout_time=${2:-20s}

cd "$(dirname "${BASH_SOURCE[0]}")"
if ! [[ -d ../../build ]]; then
    >&2 echo "[ERROR] Binaries were not built."
    exit 1
fi
cd "../../build"
if ! [[ -f "../benchmarks/iscas85/$circuit.bench" ]]; then
    >&2 echo "[ERROR] Could not find benchmark."
    exit 1
fi

echo "[INFO] Running ISCAS'85 $circuit benchmark."
./src/bench/VDSProject_bench "../benchmarks/iscas85/$circuit.bench"
if ! [[ -d "results_$circuit/txt" ]]; then
    >&2 echo "[ERROR] Could not find reference results."
    exit 1
fi

i=1
files=(results_$circuit/txt/*)
total_files=${#files[@]}
echo "[INFO] Running a total of $total_files verifications."
for file in ${files[*]} ; do
    filename=$(echo "${file##*/}")
    echo -n "[$i/$total_files] Verifying equivalence of $filename circuit... "
    if ! [[ -f "../results/results_$circuit/txt/$filename" ]]; then
        echo "No reference file."
        echo "[WARN] Aborting subsequent verifications due to incorrect reference files."
        break
    fi

    time_start=$(date +%s%3N)
    result=$(timeout $timeout_time ./src/verify/VDSProject_verify results_$circuit/txt/$filename ../results/results_$circuit/txt/$filename || echo "Timeout.")
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
