//
// Static Work Distribution on Large Chunks
// with fine grained Round Robin Assignment
//

#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <span>
#include <random>

using namespace std;
// for simpler code
using uid = uniform_int_distribution<>;

int main(){
    // 2^16 items
    int num_items = 1 << 18;
    int num_bins = 4;
    int items_per_bin = num_items/num_bins;

    // Creating 4 buckets of random values
    random_device rd;
    mt19937 mt(rd());

    // bins, these denote the different job lengths
    uid b1 (1,25);
    uid b2 (26,50);
    uid b3 (51,75);
    uid b4 (76,100);
    
    // our work vector
    vector<int> work;
    
    // lambda to assign work diff jobs
    auto work_assign = [&](uid bin_type){
        for(int i = 0; i < items_per_bin; i++){
            // generating from that bin using our randgen and adding to worklist
            int x = bin_type(mt);
            work.push_back(x);
        }
    };

    // calling the lambda on all the 4 bins
    work_assign(b1);
    work_assign(b2);
    work_assign(b3);
    work_assign(b4);

    int num_threads = 8;

    auto work_start = [&](int thread_id){
        for (int i = thread_id; i < num_items; i+= num_threads){
            this_thread::sleep_for(chrono::microseconds(work[i]));
        }
    };

    // starting clock
    auto start_time = chrono::high_resolution_clock::now();
    
    vector<jthread> threads;
    for(int i = 0; i < num_threads; i++){
        threads.emplace_back(work_start,i);
    }

    // end of clock
    auto end_time = chrono::high_resolution_clock::now();
    auto total_duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

    cout << "Total execution time: " << total_duration.count() << " us\n";

    return 0;


}

// compile with
// g++ .\fine_grain.cpp --std=c++20 -lpthread -O3 -o fine 