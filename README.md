# Parallel C++

# Static Partitioning Strategies
The Sample Problem we are taking is a simple parallel for loop that processes a vector of jobs with varying durations. The goal is to assign jobs to threads in a way that balances the workload and minimizes execution time.

## 1) Coarse-Grained
- We divide the work into 8 contigous chunks, each assigned to a thread. Each thread processes a chunk of jobs sequentially.
- This way each of the 8 threads get a contiguous block of jobs to work on.
```
Thread 0 -> [0 : jobs_per_thread)
Thread 1 -> [jobs_per_thread : 2*jobs_per_thread) and so on.
```
- **File**: [coarse_grain.cpp](StaticPartitioning/coarse_grain.cpp)


### ✅ Advantages:
- Low overhead, Cache locality is good as each thread works on a contiguous block of memory.
- Ideal when all jobs are of similar duration.

### ❌ Disadvantages:
- Bad for load imbalance; if one of chunk has heavy jobs (e.g., jobs from bin 76–100), that thread becomes a bottleneck while others finish early.

## 2. Fine-Grained (*Round-Robin Assignment*)
- Each thread processes every `num_threads`-th job from the entire work array:
    * Thread 0 does jobs: `0, 8, 16, 24, ...`
    * Thread 1 does: `1, 9, 17, 25, ...` and so on.

- This is a round-robin static scheduling strategy.
- **File**: [fine_grain.cpp](StaticPartitioning/fine_grain.cpp)


### ✅ Advantages:
- Good for load balancing when job durations vary a lot.
- Even if heavy jobs are clustered in bins, it evens out across threads.

### ❌ Disadvantages:
- Poor cache locality, as threads jump around in memory.
- Slightly more overhead due to poor spatial access patterns.
- no work stealing (which would help if imbalance occurs during runtime).


## Dyanmic Partitioning Strategies
Dynamic partitioning strategies allow threads to dynamically adjust their workload based on the current state of the system and the jobs available. This can help to balance the load more effectively, especially when job durations vary significantly.
## 1. Task-Based Parallelism
- Instead of dividing the work into fixed chunks, we can use an atomic variable to keep track of the next job to be processed.
- Each thread can then pick up the next available job from the shared atomic variable.
- **File**: [dyn1.cpp](DynamicPartitioning/dyn1.cpp)
### ✅ Advantages:
- Excellent load balancing, as threads can pick up tasks based on their current workload.
- Reduces idle time for threads, as they can take on more work if they finish early
### ❌ Disadvantages:
- More overhead due to the need for synchronization and communication between threads.
- Complexity increases, as threads need to manage their own work queues (in other cases, we'll see) and atomic variables.
