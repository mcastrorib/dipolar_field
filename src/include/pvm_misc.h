#ifndef PVM_MISC_H
#define PVM_MISC_H

#ifdef _WIN64
	#include <Windows.h>
	#undef min
	#undef max
#endif

#include <string>
#include <iostream>
#include <ctime>
#include <mutex>
#include <queue>
#include <omp.h>
#include <thread>
#include <condition_variable>
#include "pvm_ints.h"

using std::cin;
using std::cout;
using std::endl;

using std::mutex;
using std::thread;

using std::string;
using std::to_string;

using std::clock;
using std::clock_t;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////// GLOBAL NUM USED THREADS FOR OMP
extern int PVM_GLOBAL_NUM_THREADS;
void set_global_num_threads(const int num_cpu_threads);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////// PRINTING
string print_binary_uint64(uint64_t value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////// RANGE STUFF
struct Range
{
	int64_pvm begin, end;
};

struct PassRanges
{
	int num_passes;
	int64_pvm pref_pass_size, full_size;
	PassRanges(const int64_pvm in_pref_pass_size, const int64_pvm in_full_size);
	Range get_pass_range(const int64_pvm pass_index);
	int64_pvm get_pass_size(const int64_pvm pass_index);
};

void get_multi_thread_loop_limits(
	const int tid, const int num_threads, 
	const int num_loops, int &start, int &finish);

void get_multi_thread_loop_limits(
	const int tid, const int num_threads, 
	const int64_pvm num_loops, int64_pvm &start, int64_pvm &finish);

Range get_threads_loop_range(const int tid, const int num_threads, const int64_pvm num_loops);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// TIME STUFF
double get_time_difference(clock_t prev_time);
void dirty_sleep(int ms);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////// PROCESSING STATUS
class ProcessingStatus
{
private:
	bool kill_state;
	bool pause_state;
	int num_processed_items;
	string state_string;
	mutex kill_mutex;
	mutex pause_mutex;
	mutex state_mutex;

public:
	ProcessingStatus();

	void restart();
	bool check_state();
	bool check_kill_state();

	void set_kill_state(const bool in_kill_state);
	void set_pause_state(const bool in_pause_state);
	void set_state_string(std::string state);
	void set_processed_items(const int in_num_processed_items);
	void set_state(const int in_num_processed_items, string in_state_string);
	void increment_processed_item_count(int item_count_to_add = 1);	

	int get_processed_items();
	int get_current_state(std::string &current_state);
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////// SynchronizedQueue
class SynchronizedQueue
{
private:
	mutex queue_lock;
	std::queue<int64_pvm> sync_queue;
	std::condition_variable cond_var;
public:
	SynchronizedQueue();
	~SynchronizedQueue();
	void push(int64_pvm value);
	int64_pvm pop();
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////// SynchronizedQueue
class SynchronizedBarrier
{
private:
	mutex barrier_lock;
	std::condition_variable cond_var;
	int ntt;  //num_total_threads
	int nwt;  //num_wainting_threads 
public:
	SynchronizedBarrier(int num_total_threads);
	void wait();
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////// SynchronizedQueue
class ThreadSafeCounter
{
private:
	mutex m;
	int counter;
public:
	ThreadSafeCounter(const int start_value = 0);
	int get_next(const int inc_value = 1);
	void set(const int value);
};

#endif