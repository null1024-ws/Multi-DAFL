#include <iostream>
#include "interface.hpp"

int main(){
    double prob;
    int share_time;
    char* log_path = "/home/moran/Workspace/Project/DirectedFuzz/afl/MultiTest/DAFL/benchmark/small-size-test/playground/output/guarding_condition.log";
    get_prob_share_time(&prob, &share_time, log_path);
    std::cout << "prob is " << prob << std::endl;
    std::cout << "share_time is " << share_time << std::endl;
    return 0;
}