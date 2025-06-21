//
// We use atomics here to assign work to the available thread
// this way as soon as a thread gets free, it can start working
// no threads are left idle and Core Utilisation is increased
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

    // we declare an atomic variable that is shared across all the threads
    atomic<int> idx = 0;

    // same as below while loop lambda
    // auto work_start = [&](){
    //     for (int i = idx.fetch_add(1); i < num_items; i = idx.fetch_add(1)){
    //         this_thread::sleep_for(chrono::microseconds(work[i]));
    //     }
    // };

    //
    // a more readable version, is via the while loop impl. of this lambda
    // as the for-loop incremental and initalisation of i looks the same
    // this is bc, an i++ would induce race-conditon or value-mismatch
    //
    auto work_start = [&](){
        while(true){
            int i = idx.fetch_add(1);
            if(i >= num_items) break;
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