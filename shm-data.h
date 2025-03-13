#ifndef SHMDATA_H
#define SHMDATA_H

#include <stdint.h> // 用于固定宽度整数类型，如int64_t

#define SHM_KEY 710
#define MAX_LOG_NUM 100000  // Log data number, max size: 300MB
// #define MAX_LOG_NUM 100

// Log content data structure
typedef struct LogData {
    int index;                // index rank
    int share;                // share times
    int is_negated;           // enter this condition or not
    int64_t low[2];           // low boundary
    int64_t high[2];          // high boundary
    int64_t expr[2];          // expression value
    int andor;                // and/or 
} LogData;

typedef struct SharedMemory {
    int count;  // current log number
    int tail;   // insert position for next log
    LogData log_data[MAX_LOG_NUM];  // all log data
} SharedMemory;

#endif 