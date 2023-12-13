/*
 * BenchmarkLib.cpp
 *
 *  Created on: 25.01.2011
 *      Author: villarraga
 */


#include "BenchmarkLib.h"

using namespace std;


long long max_mem_usage;
ofstream benmkng_file;

void create_benmkng_file(string name) {
	// creates files and checks ...
	benmkng_file.open("benchmarking_info.txt");
	if(!benmkng_file)
		cerr << "-E- Error: create_benmkng_file. Info: Imposible to create file" << endl;

	benmkng_file << "- Benchmarking Information" << endl;
	benmkng_file << "- Project Name: " << name << endl;
	benmkng_file << endl;

	max_mem_usage = 0;
}

void close_benmkng_file(void) {
	benmkng_file.close();
}

void write_benmkng_info(string info, int dat) {
	ios_base::fmtflags initialState = benmkng_file.setf(ios_base::fixed, ios_base::floatfield); // saves initial state
	benmkng_file.setf(ios_base::showpoint);
	benmkng_file.precision(4); // precision 4 decimal digits
	benmkng_file.setf(ios::fixed,ios::floatfield);

	// prints into file
	benmkng_file << info << ": " << dat << " ";

	benmkng_file.setf(initialState); // restores original format
}

void write_benmkng_time(string stage, double t) {
	benmkng_file << stage << ": " << t << " ";
}

void write_benmkng_newline() {
	benmkng_file << endl;
}

void reset_peak_memory() {
	max_mem_usage=0;
}


void update_benmkng_memory(long long mem) {

	// we are interested on the peak value
	if (mem > max_mem_usage)
		max_mem_usage = mem;

}

long long get_mem_peak() {
	return max_mem_usage;
}


void write_benmkng_memory() {
	benmkng_file << "Max. Memory Usage (MB): " << max_mem_usage / 1e6 << endl;
}

double totalTime(void) {
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    double t_usr = (double)ru.ru_utime.tv_sec + (double)ru.ru_utime.tv_usec / 1000000.0; // user time
    double t_sys = (double)ru.ru_stime.tv_sec + (double)ru.ru_stime.tv_usec / 1000000.0; // system time
    return t_usr+t_sys;
}

double userTime(void) {
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return (double)ru.ru_utime.tv_sec + (double)ru.ru_utime.tv_usec / 1000000; }

int memReadStats(int field) {
    char    name[256];
    pid_t pid = getpid();
    sprintf(name, "/proc/%d/statm", pid);
    FILE*   in = fopen(name, "rb");
    if (in == NULL) return 0;
    int     value;
    for (; field >= 0; field--)
        fscanf(in, "%d", &value);
    fclose(in);
    return value;
}


long long memUsing()
{
	return (long long)memReadStats(0) * (long long)getpagesize();
}

// taken from:
// http://stackoverflow.com/questions/669438/how-to-get-memory-usage-at-run-time-in-c
void process_mem_usage(double& vm_usage, double& resident_set)
{
   vm_usage = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;

}



