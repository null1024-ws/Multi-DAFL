
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <iomanip>
#include <regex>
#include <array>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include "kde.hpp"
#include "../shm-data.h"
#include "interface.hpp"


vector<double> split(const string &s, char delim) {
    vector<double> elems;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(stod(item));
    }
    return elems;
}

size_t fetch_value(string line, string prefix, size_t start_pos){
    return stoi(
        line.substr(
            start_pos + prefix.length(), 
            line.find(')', start_pos) - start_pos - prefix.length()
        )
    );
}


// // This function needs to be rewritten
// void get_prob_share_time(double* prob, int* share_time, char* file_path){
//     KDE kde;
//     // we can setup the parameter here. Current we use default version
//     ifstream file(file_path);


//     const string IDX_STRING = "index(";
//     const string VALUE_STRING = "expr(";
//     const string SHARE_PREFIX = "share(";
//     const string LOW_PREFIX = "low(";
//     const string HIGH_PREFIX = "high(";
//     const string BINOMIAL_PREFIX = "isboolean(";
//     int isBool = 0;
//     // begin calculate
//     string line;
//     map<int, vector<vector<double>>> idx_val_map;
//     map<int, pair<double, double>> idx_bond_map;
//     map<int, int> idx_share_map;
//     int biggest_idx = -1;

//     while (getline(file, line)) {
//         size_t idxPos = line.find(IDX_STRING);
//         size_t valPos = line.find(VALUE_STRING);
//         size_t shrPos = line.find(SHARE_PREFIX);
//         size_t lowPos = line.find(LOW_PREFIX);
//         size_t highPos = line.find(HIGH_PREFIX);
//         size_t isboolPos = line.find(BINOMIAL_PREFIX);
//         int idx = -1;


//         //find value and index...
//         if (idxPos != string::npos && valPos != string::npos) {
//             // int idx = stoi(line.substr(idxPos + IDX_STRING.length(), line.find(')', idxPos) - idxPos - IDX_STRING.length()));
//             idx = fetch_value(line, IDX_STRING, idxPos);
//             vector<double> data = split(line.substr(valPos + VALUE_STRING.length(), line.find(')', valPos) - valPos - VALUE_STRING.length()), ',');
//             idx_val_map[idx].push_back(data);

//             // Update biggest index
//             if (idx > biggest_idx){
//                 biggest_idx = idx;
//             }
//         }

//         // Get share times
//         if (idx != -1 && shrPos != string::npos){
//             auto it = idx_share_map.find(idx);
//             if (it == idx_share_map.end()){
//                 idx_share_map[idx] = fetch_value(line, SHARE_PREFIX, shrPos);
//             }
//         }

//         // Get last low and high value
//         if (idx != -1 && lowPos != string::npos && highPos != string::npos){
//             idx_bond_map[idx] = make_pair(
//                     fetch_value(line, LOW_PREFIX, lowPos),
//                     fetch_value(line, HIGH_PREFIX, highPos)
//                 );
//         }

//         // Is boolean type condition?
//         if (idx != -1 && isboolPos != string::npos){
//             isBool = fetch_value(line, BINOMIAL_PREFIX, isboolPos);
//         }
//     }
//     file.close();

//     // begin to return back
//     /* Notice: we only focus on the last index, for other index of GC is passed*/

//     // cout << "begin analyzing variable: index" << biggest_idx << endl;
    
//     for (auto &data : idx_val_map[biggest_idx]) {
//         kde.add_data(data);
//     }
//     //calculate the probability  high-low
//     if (!!isBool){
//         *prob = kde.get_01_distr_prob();
//     } else {
//         int high = idx_bond_map[biggest_idx].second;
//         int low = idx_bond_map[biggest_idx].first;
//         *prob = kde.cdf(high) - kde.cdf(low);
//     }

//     // assign the share times
//     *share_time = idx_share_map[biggest_idx];
    
//     // cout << fixed << setprecision(10) << "prob from low to high: " << prob << endl;
//     // cout << fixed << setprecision(10) << "high: " << high << " low:" << low << endl;
//     // cout << fixed << setprecision(10) << "shared times " << idx_share_map[biggest_idx] << endl;

// }



// // This function needs to be rewritten
// void get_prob_share_time(double* prob, int* share_time, char* file_path){
//     KDE kde;
//     // we can setup the parameter here. Current we use default version
//     ifstream file(file_path);


//     const string IDX_STRING = "index(";
//     const string SHARE_PREFIX = "share(";
//     const string LOW_PREFIX = "low(";
//     const string HIGH_PREFIX = "high(";
//     const string VALUE_STRING = "expr(";
//     const string BINOMIAL_PREFIX = "andor(";
//     int isBool = 0;
//     // begin calculate
//     string line;
//     map<int, vector<vector<double>>> idx_val_map;
//     map<int, pair<double, double>> idx_bond_map;
//     map<int, int> idx_share_map;
//     int biggest_idx = -1;

//     while (getline(file, line)) {
//         size_t idxPos = line.find(IDX_STRING);
//         size_t valPos = line.find(VALUE_STRING);
//         size_t shrPos = line.find(SHARE_PREFIX);
//         size_t lowPos = line.find(LOW_PREFIX);
//         size_t highPos = line.find(HIGH_PREFIX);
//         size_t isboolPos = line.find(BINOMIAL_PREFIX);
//         int idx = -1;


//         //find value and index...
//         if (idxPos != string::npos && valPos != string::npos) {
//             // int idx = stoi(line.substr(idxPos + IDX_STRING.length(), line.find(')', idxPos) - idxPos - IDX_STRING.length()));
//             idx = fetch_value(line, IDX_STRING, idxPos);
//             vector<double> data = split(line.substr(valPos + VALUE_STRING.length(), line.find(')', valPos) - valPos - VALUE_STRING.length()), ',');
//             idx_val_map[idx].push_back(data);

//             // Update biggest index
//             if (idx > biggest_idx){
//                 biggest_idx = idx;
//             }
//         }

//         // Get share times
//         if (idx != -1 && shrPos != string::npos){
//             auto it = idx_share_map.find(idx);
//             if (it == idx_share_map.end()){
//                 idx_share_map[idx] = fetch_value(line, SHARE_PREFIX, shrPos);
//             }
//         }

//         // Get last low and high value
//         if (idx != -1 && lowPos != string::npos && highPos != string::npos){
//             idx_bond_map[idx] = make_pair(
//                     fetch_value(line, LOW_PREFIX, lowPos),
//                     fetch_value(line, HIGH_PREFIX, highPos)
//                 );
//         }

//         // Is boolean type condition?
//         if (idx != -1 && isboolPos != string::npos){
//             isBool = fetch_value(line, BINOMIAL_PREFIX, isboolPos);
//         }
//     }
//     file.close();

//     // begin to return back
//     /* Notice: we only focus on the last index, for other index of GC is passed*/

//     // cout << "begin analyzing variable: index" << biggest_idx << endl;
    
//     for (auto &data : idx_val_map[biggest_idx]) {
//         kde.add_data(data);
//     }
//     //calculate the probability  high-low
//     if (!!isBool){
//         *prob = kde.get_01_distr_prob();
//     } else {
//         int high = idx_bond_map[biggest_idx].second;
//         int low = idx_bond_map[biggest_idx].first;
//         *prob = kde.cdf(high) - kde.cdf(low);
//     }

//     // assign the share times
//     *share_time = idx_share_map[biggest_idx];
    
//     // cout << fixed << setprecision(10) << "prob from low to high: " << prob << endl;
//     // cout << fixed << setprecision(10) << "high: " << high << " low:" << low << endl;
//     // cout << fixed << setprecision(10) << "shared times " << idx_share_map[biggest_idx] << endl;

// }


struct IfRangeData {
    int index;
    int share;
    int is_negated;
    std::array<long long, 2> low;
    std::array<long long, 2> high;
    std::array<long long, 2> expr;
    int andor;
};

// 解析函数
IfRangeData parseIfRange(const std::string& input) {
    std::regex pattern(R"(\[if_range\]: index\((\d+)\), share\((\d+)\), negated\((\d+)\), low\((-?\d+),\s*(-?\d+)\), high\((-?\d+),\s*(-?\d+)\), expr\((-?\d+),\s*(-?\d+)\), andor\((\d+)\))");
    std::smatch match;

    IfRangeData data;

    if (std::regex_search(input, match, pattern)) {
        // 按顺序提取匹配到的数值
        data.index = std::stoi(match[1]);
        data.share = std::stoi(match[2]);
        data.is_negated = std::stoi(match[3]);
        data.low = {std::stoll(match[4]), std::stoll(match[5])};
        data.high = {std::stoll(match[6]), std::stoll(match[7])};
        data.expr = {std::stoll(match[8]), std::stoll(match[9])};
        data.andor = std::stoi(match[10]);
    } else {
        throw std::invalid_argument("Input string does not match the expected format.");
    }

    return data;
}

void get_prob_share_time_old(double* prob, int* share_time, char* file_path){
    ifstream file(file_path);

    string line;
    map<int, vector<array<long long, 2>>> idx_val_map;
    map<int, vector<pair<long long, long long>>> idx_bond_map;
    map<int, int> idx_share_map;
    map<int, int> idx_andor_map;
    int biggest_idx = -1;

    regex pattern(R"(\[if_range\]: index\((\d+)\), share\((\d+)\), negated\((\d+)\), low\((\d+),\s*(\d+)\), high\((\d+),\s*(\d+)\), expr\((\d+),\s*(\d+)\), andor\((\d+)\))");
    smatch match;
    int is_negated = 1;

    while (getline(file, line)){
        try{
            IfRangeData data = parseIfRange(line);
            idx_val_map[data.index].push_back({data.expr[0], data.expr[1]});
            if (data.index > biggest_idx){
                
                biggest_idx = data.index;
            }

            // Get share times
            if (data.index != -1){
                idx_share_map[data.index] = data.share;

                idx_bond_map[data.index] = {
                    {data.low[0], data.high[0]},
                    {data.low[1], data.high[1]}
                };

                idx_andor_map[data.index] = data.andor;
            }
            
            // Get is_negated
            is_negated = data.is_negated;

            
        } catch (const invalid_argument& e){
            cerr << e.what() << endl;
        }
    }
    file.close();

    if (biggest_idx == -1){
        // In this situation, we obtain nothing
        *prob = 1;
        *share_time = 1;
        return;
    }

    KDE kde_0, kde_1;
    double prob_0, prob_1;
    int low_0, high_0, low_1, high_1;
    for(auto &val: idx_val_map[biggest_idx]){
        kde_0.add_data(val[0]);
        kde_1.add_data(val[1]);
    }

    if (idx_bond_map[biggest_idx][0].first == -1 && idx_bond_map[biggest_idx][0].second == -1){
        // *prob = kde_0.get_01_distr_prob();
        *prob = kde_0.estimate_01_likelihood();
    } else {
        low_0 =  idx_bond_map[biggest_idx][0].first;
        high_0 = idx_bond_map[biggest_idx][0].second;
        low_1 =  idx_bond_map[biggest_idx][1].first;
        high_1 = idx_bond_map[biggest_idx][1].second;

        prob_0 = kde_0.cdf(low_0) - kde_0.cdf(high_0);
        if (low_1 != -1 && high_1 != -1){
            prob_1 = kde_1.cdf(idx_bond_map[biggest_idx][1].first) - kde_1.cdf(idx_bond_map[biggest_idx][1].second); 
        } else {
            // Only prob_0 is valid
            prob_1 = 1;
        }
        
        if (idx_andor_map[biggest_idx]){
            *prob = prob_0 * prob_1;
        } else {
            *prob = prob_0 + prob_1;
        }
        
    }

    if (is_negated == 0){
        // is_negated == 0 means do not entry the condition
        *prob = 1 - *prob;
    }

    *share_time = idx_share_map[biggest_idx];

}


void get_prob_share_time(double* prob, int* share_time){

    string line;
    unordered_map<int, vector<array<long long, 2>>> idx_val_map;
    // map<int, vector<pair<long long, long long>>> idx_bond_map;
    unordered_map<int, array<pair<long long, long long>, 2>> idx_bond_map;
    unordered_map<int, int> idx_share_map;
    unordered_map<int, int> idx_andor_map;
    unordered_map<int, int> idx_negated_map;
    int biggest_idx = -1;
    const size_t MAX_VEC_SIZE = 10000;


    // Test the value of prob
    printf("[KDE-get_prob_share_time]: the init prob is %lf\n", *prob);

    // Get shmid points to the target shmid
    char* shmid_ptr_ptr = getenv("SHMID_PTR_PTR");
    if (shmid_ptr_ptr == NULL){
        fprintf(stderr, "[KDE]: SHM_ID_PTR_PTR environment variable not set\n");
        exit(1);
    }

    // Get the shmid from shm_id_ptr_ptr, whose shm point to the target shmid. Therefore, it is shm_id_ptr
    int shmid_ptr = atoi(shmid_ptr_ptr);
    if (shmid_ptr == 0) {
      perror("[KDE]: SHMID_PTR parse error\n");
      exit(1);
    }

    int* target_shmid = (int*) shmat(shmid_ptr, NULL, 0);
    if (target_shmid == (void*)-1) {
        perror("[KDE]: shmat failed");
        exit(1);
    }

    // Get the target share memory address
    SharedMemory* shm_ptr = (SharedMemory*) shmat(*target_shmid, NULL, 0);


    // Visit the circular array
    printf("[KDE-get_prob_share_time]: before calculate head\n");
    int head = (shm_ptr->tail - shm_ptr->count + MAX_LOG_NUM) % MAX_LOG_NUM; // 计算head
    printf("[KDE-get_prob_share_time]: total number is %d, MAX_LOG_NUM is %d, head is %d\n", shm_ptr->count, MAX_LOG_NUM, head);
    int index = 0;
    LogData data;

        // 获取循环开始前的时间点
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < shm_ptr->count; i++) {
        try {
            index = (head + i) % MAX_LOG_NUM; 
            // printf("[KDE-get_prob_share_time]: index is %d \n", index);
            data = shm_ptr->log_data[index];
            
            // printf("[KDE-get_prob_share_time]: data.index is %d\n", data.index);
            // Add a boundary 
            auto& vec = idx_val_map[data.index];
            // Check whether the capacity is lower than MAX_VEC_SIZE
            if (vec.capacity() < MAX_VEC_SIZE){
                vec.reserve(MAX_VEC_SIZE);
            }

            // Too much value, delete the oldest value.
            if (vec.size() >= MAX_VEC_SIZE) {
                vec.erase(vec.begin());  
            }
            vec.push_back({data.expr[0], data.expr[1]});
            
            // // Old unlimited method
            // idx_val_map[data.index].push_back({data.expr[0], data.expr[1]});
            
            if (data.index > biggest_idx){
                biggest_idx = data.index;
            }

            // Get share times
            if (data.index != -1){
                // printf("[KDE-get_prob_share_time]: data.index is not -1\n");
                idx_share_map[data.index] = data.share;
                idx_bond_map[data.index] = {{
                    {data.low[0], data.high[0]},
                    {data.low[1], data.high[1]}
                }};
                idx_andor_map[data.index] = data.andor;
                idx_negated_map[data.index] = data.is_negated;
                // printf("[KDE-get_prob_share_time]: finish one cycle\n");
            }

        } catch (const std::exception& e) {
            std::cerr << "Error at i:" << i << "; index is:" << index << "; data_index is:" << data.index << "; " << e.what() << std::endl;
        }
        
    }

        // 获取循环结束后的时间点
    auto end = std::chrono::high_resolution_clock::now();

    // 计算时间差
    std::chrono::duration<double> duration = end - start;

    // 输出执行时间
    std::cout << "[KDE-get_prob_share_time]: time cost: " << duration.count() << " s" << std::endl;

    
    if (biggest_idx == -1){
        // In this situation, we obtain nothing
        *prob = 1;
        *share_time = 1;
        // 释放共享内存映射
        if (shmdt(target_shmid) == -1) {
            perror("[KDE]: shmdt(target_shmid) failed");
        }

        if (shmdt(shm_ptr) == -1) {
            perror("[KDE]: shmdt(shm_ptr) failed");
        }

        // 清理 STL 容器
        idx_val_map.clear();
        idx_bond_map.clear();
        idx_share_map.clear();
        idx_andor_map.clear();
        idx_negated_map.clear();
        return;
    }

    printf("[KDE-get_prob_share_time]: Begin to add data to KDE\n");
    KDE kde_0, kde_1;
    double prob_0, prob_1;
    int low_0, high_0, low_1, high_1;
    printf("[KDE-get_prob_share_time]: Local variable prepared\n");
    for(auto &val: idx_val_map[biggest_idx]){
        kde_0.add_data(val[0]);
        kde_1.add_data(val[1]);
        // printf("[KDE-get_prob_share_time]: value_0: %lld, value_1: %lld\n", val[0], val[1]);
    }
    
    printf("[KDE-get_prob_share_time]: the biggest index is %d\n", biggest_idx);

    if (idx_bond_map[biggest_idx][0].first == -1 && idx_bond_map[biggest_idx][0].second == -1){
        // *prob = kde_0.get_01_distr_prob();
        printf("[KDE-get_prob_share_time]: calculate the boolean type\n");
        *prob = kde_0.estimate_01_likelihood();
    } else {
        printf("[KDE-get_prob_share_time]: calculate the arithmatic type\n");
        printf("[KDE-get_prob_share_time]: low_0: %d, low_1: %d, high_0: %d, high_1: %d\n", 
        low_0, low_1, high_0, high_1);
        low_0 =  idx_bond_map[biggest_idx][0].first;
        high_0 = idx_bond_map[biggest_idx][0].second;
        low_1 =  idx_bond_map[biggest_idx][1].first;
        high_1 = idx_bond_map[biggest_idx][1].second;

        prob_0 = kde_0.cdf(low_0) - kde_0.cdf(high_0);
        printf("[KDE-get_prob_share_time]: prob_0: %lf\n", prob_0);
        if (low_1 != -1 && high_1 != -1){
            prob_1 = kde_1.cdf(idx_bond_map[biggest_idx][1].first) - kde_1.cdf(idx_bond_map[biggest_idx][1].second); 
        } else {
            // Only prob_0 is valid
            prob_1 = 1;
        }
        
        printf("[KDE-get_prob_share_time]: prob_1: %lf\n", prob_1);
        if (idx_andor_map[biggest_idx]){
            *prob = prob_0 * prob_1;
        } else {
            *prob = prob_0 + prob_1;
        }
        
    }

    printf("[KDE-get_prob_share_time]: prob is %lf\n", *prob);
    if (idx_negated_map[biggest_idx] == 0){
        // is_negated == 0 means do not entry the condition
        *prob = 1 - *prob;
    }

    *share_time = idx_share_map[biggest_idx];

    // Free all of the memory
    
    // 释放共享内存映射
    if (shmdt(target_shmid) == -1) {
        perror("[KDE]: shmdt(target_shmid) failed");
    }

    if (shmdt(shm_ptr) == -1) {
        perror("[KDE]: shmdt(shm_ptr) failed");
    }

    // 清理 STL 容器
    idx_val_map.clear();
    idx_bond_map.clear();
    idx_share_map.clear();
    idx_andor_map.clear();
    idx_negated_map.clear();


}
