// watch this
// https://www.youtube.com/watch?v=iLHNF7SgVN4&ab_channel=CppCon
//
// https://stackoverflow.com/questions/55216224/what-is-the-difference-between-continuation-stealing-and-child-stealing
//
// we implement a distributed work queue for work stealing here (central one in dyn1.cpp)
// this is useful in graph traversals or problems where we dont know how long each task
// might be taking
// for eg. you dont know from a node from a graph, how deep the DFS traversal will end up going
// or another eg. can be a minmax game simualtion, where there can be multiple legal moves
// but one move might lead to victory/loss very fast compared to another one which will
// stall the game, in cases like these where the length of the task can vary, stealing
// is almost always going to be optimal.
// we will see 3 solves with this
// 1) Graph Traversals (skewed vs uniform))
// 2) SAT Solving (backtracking can vary)
// 3) Minimax / Parallel AlphaBeta Search


int main(){

}

