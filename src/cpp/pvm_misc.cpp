#include "pvm_misc.h"

int PVM_GLOBAL_NUM_THREADS = omp_get_max_threads();

void set_global_num_threads(const int num_cpu_threads)
{
	PVM_GLOBAL_NUM_THREADS = num_cpu_threads;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////// PRINTING
string print_binary_uint64(uint64_t value)
{
	string binary_string;
	for (int i = 0; i < 64; i++)
	{
		if (value & (1ULL << i))
		{ 
			binary_string += "1";
		}
		else 
		{ 
			binary_string += "0";
		}
	}
	return binary_string;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////// RANGE STUFF

PassRanges::PassRanges(const int64_pvm in_pref_pass_size, const int64_pvm in_full_size)
{
	this->full_size = in_full_size;
	this->pref_pass_size = in_pref_pass_size;
	this->num_passes = this->full_size / this->pref_pass_size;
	if (this->full_size % this->pref_pass_size) { this->num_passes += 1; }
}

Range PassRanges::get_pass_range(const int64_pvm pass_index)
{
	Range range;
	range.begin = pass_index * this->pref_pass_size;
	range.end = range.begin + this->pref_pass_size;
	if (this->full_size < range.end) { range.end = this->full_size; }
	return range;
}

int64_pvm PassRanges::get_pass_size(const int64_pvm pass_index)
{
	const Range range = this->get_pass_range(pass_index);
	return range.end - range.begin;
}


void get_multi_thread_loop_limits(const int tid, const int num_threads, const int num_loops, int &start, int &finish)
{
	const int loops_per_thread = num_loops / num_threads;
	start = tid * loops_per_thread;
	if (tid == (num_threads - 1)) { finish = num_loops; }
	else { finish = start + loops_per_thread; }
}

void get_multi_thread_loop_limits(const int tid, const int num_threads, 
	const int64_pvm num_loops, int64_pvm &start, int64_pvm &finish)
{
	const int64_pvm loops_per_thread = num_loops / (int64_pvm)num_threads;
	start = tid * loops_per_thread;
	if (tid == (num_threads - 1)) { finish = num_loops; }
	else { finish = start + loops_per_thread; }
}

Range get_threads_loop_range(const int tid, const int num_threads, const int64_pvm num_loops)
{
	Range range;
	const int64_pvm loops_per_thread = num_loops / (int64_pvm)num_threads;
	range.begin = tid * loops_per_thread;
	if (tid == (num_threads - 1)) { range.end = num_loops; }
	else { range.end = range.begin + loops_per_thread; }
	return range;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// TIME STUFF
double get_time_difference(clock_t prev_time)
{
	return ((double)clock() - prev_time) / ((double)CLOCKS_PER_SEC);
}

void dirty_sleep(int ms)
{
#ifdef _WIN64
	Sleep((uint32_pvm)ms);
#elif __linux__
	struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
	nanosleep(&ts, NULL);
#endif
	return;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////// PROCESSING STATUS

ProcessingStatus::ProcessingStatus() 
{
	this->kill_state = false;
	this->pause_state = false;
	this->state_string = "";
	this->num_processed_items = 0;
}

void ProcessingStatus::restart()
{
	this->set_kill_state(false);
	this->set_pause_state(false);
	this->set_state_string("");
	this->set_processed_items(0);
}

void ProcessingStatus::set_kill_state(const bool in_kill_state)
{
	this->kill_mutex.lock();
	this->kill_state = in_kill_state;
	this->kill_mutex.unlock();
}

void ProcessingStatus::set_pause_state(const bool in_pause_state)
{
	this->pause_mutex.lock();
	this->pause_state = in_pause_state;
	this->pause_mutex.unlock();
}

bool ProcessingStatus::check_state()
{
	bool p_state, out_state;
	while (true)
	{
		this->pause_mutex.lock();
		p_state = this->pause_state;
		this->pause_mutex.unlock();

		if (p_state)
		{
			dirty_sleep(200);

			this->kill_mutex.lock();
			out_state = this->kill_state;
			this->kill_mutex.unlock();

			if (out_state) { return out_state; }
			else { continue; }
		}

		else { break; }
	}

	this->kill_mutex.lock();
	out_state = this->kill_state;
	this->kill_mutex.unlock();
	return out_state;
}


bool ProcessingStatus::check_kill_state()
{
	this->kill_mutex.lock();
	const bool out_kill_state = this->kill_state;
	this->kill_mutex.unlock();
	return out_kill_state;
}

void ProcessingStatus::set_state_string(std::string state)
{
	this->state_mutex.lock();
	this->state_string = state;
	cout << state << endl;
	this->state_mutex.unlock();
}

void ProcessingStatus::set_processed_items(const int in_num_processed_items)
{
	this->state_mutex.lock();
	this->num_processed_items = in_num_processed_items;
	this->state_mutex.unlock();
}

void ProcessingStatus::increment_processed_item_count(int item_count_to_add)
{
	this->state_mutex.lock();
	this->num_processed_items += item_count_to_add;
	this->state_mutex.unlock();
}


void ProcessingStatus::set_state(const int in_num_processed_items, string in_state_string)
{
	this->state_mutex.lock();
	this->num_processed_items = in_num_processed_items;
	this->state_string = in_state_string;
	this->state_mutex.unlock();
}

int ProcessingStatus::get_processed_items()
{
	this->state_mutex.lock();
	const int out_num_processed_items = this->num_processed_items;
	this->state_mutex.unlock();
	return out_num_processed_items;
}

int ProcessingStatus::get_current_state(std::string &current_state)
{
	this->state_mutex.lock();
	current_state = this->state_string;
	const int out_num_processed_items = this->num_processed_items;
	this->state_mutex.unlock();
	return out_num_processed_items;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////// SynchronizedQueue

SynchronizedQueue::SynchronizedQueue() {}
SynchronizedQueue::~SynchronizedQueue() {}

void SynchronizedQueue::push(int64_pvm value)
{
	std::lock_guard<mutex> l(this->queue_lock);
	bool wake = this->sync_queue.empty();
	this->sync_queue.push(value);
	if (wake) this->cond_var.notify_all();
}

int64_pvm SynchronizedQueue::pop()
{
	std::unique_lock<mutex> u(this->queue_lock);
	while (this->sync_queue.empty())
		this->cond_var.wait(u);
	int64_pvm value = this->sync_queue.front();
	this->sync_queue.pop();
	return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////// SynchronizedBarrier

SynchronizedBarrier::SynchronizedBarrier(int num_total_threads) { this->ntt = num_total_threads; }

void SynchronizedBarrier::wait()
{
	std::unique_lock<mutex> u(this->barrier_lock);

	if (this->nwt >= this->ntt) { this->nwt = 1; }
	else { this->nwt += 1; }

	if (this->nwt < this->ntt) { this->cond_var.wait(u); }        
	this->cond_var.notify_all();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////// ThreadSafeCounter

ThreadSafeCounter::ThreadSafeCounter(const int start_value) { this->counter = start_value; }

int ThreadSafeCounter::get_next(const int inc_value)
{
	this->m.lock();
	const int return_value = this->counter;
	this->counter += inc_value;
	this->m.unlock();
	return return_value;
}

void ThreadSafeCounter::set(const int value)
{
	this->m.lock();
	this->counter = value;
	this->m.unlock();
}