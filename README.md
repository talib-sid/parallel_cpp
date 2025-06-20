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
- Low overhead — simple thread management and memory access.
- Cache-friendly — good spatial locality in vector access.
- Ideal when all jobs are of similar duration.

### ❌ Disadvantages:
- Bad for load imbalance — if one chunk has heavy jobs (e.g., jobs from bin 76–100), that thread becomes a bottleneck while others finish early.
- No dynamic scheduling; it assumes even distribution.

## 2. Fine-Grained Round-Robin Assignment
- Each thread processes every `num_threads`-th job from the entire work array:
    * Thread 0 does jobs: `0, 8, 16, 24, ...`
    * Thread 1 does: `1, 9, 17, 25, ...` and so on.

- This is a round-robin static scheduling strategy.
- **File**: [fine_grain.cpp](StaticPartitioning/fine_grain.cpp)


### ✅ Advantages:
- Excellent load balancing when job durations vary a lot.
- Even if heavy jobs are clustered in bins, round-robin evens it out across threads.
- Better for heterogeneously distributed work (like ours: 25–100ms).

### ❌ Disadvantages:
- Poor cache locality, as threads jump around in memory.
- Slightly more overhead due to poor spatial access patterns.
- Still no work stealing (which would help if imbalance occurs during runtime).


