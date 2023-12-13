/*
 * BenchmarkLib.h
 *
 *  Created on: 01.12.2010
 *      Author: villarraga
 */

#ifndef BENCHMARKLIB_H_
#define BENCHMARKLIB_H_

/*
 * Taken from: Minisat-1.14 Global.h library
 */

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

// creates text file
void create_benmkng_file(string name);

// closes file
void close_benmkng_file(void);

// writes info like: node and loop numbers
void write_benmkng_info(string info, int dat);

// writes the time for an stage
void write_benmkng_time(string stage, double t);

// eofl
void write_benmkng_newline(void);

// records internally memory usage
void update_benmkng_memory(long long mem);

// returns current peak memory value
long long get_mem_peak();

// write at the end of the file memory usage info.
void write_benmkng_memory();

// peak memory equal to 0
void reset_peak_memory();

// returns user + system time - Taken from: Minisat-1.14 Global.h library
double totalTime(void);

// returns user time - Taken from: Minisat-1.14 Global.h library
double userTime(void);

// Taken from: Minisat-1.14 Global.h library
int memReadStats(int field);

long long memUsing();

void process_mem_usage(double& vm_usage, double& resident_set);

#endif /* BENCHMARKLIB_H_ */
