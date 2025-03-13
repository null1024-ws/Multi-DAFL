/*
  Copyright 2015 Google LLC All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at:

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

/*
   american fuzzy lop - LLVM instrumentation bootstrap
   ---------------------------------------------------

   Written by Laszlo Szekeres <lszekeres@google.com> and
              Michal Zalewski <lcamtuf@google.com>

   LLVM integration design comes from Laszlo Szekeres.

   This code is the rewrite of afl-as.h's main_payload.
*/

#include "../android-ashmem.h"
#include "../config.h"
#include "../types.h"
#include "../shm-data.h"
#include "afl-llvm-rt.o.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

/* This is a somewhat ugly hack for the experimental 'trace-pc-guard' mode.
   Basically, we need to make sure that the forkserver is initialized after
   the LLVM-generated runtime initialization pass, not before. */

#ifdef USE_TRACE_PC
#  define CONST_PRIO 5
#else
#  define CONST_PRIO 0
#endif /* ^USE_TRACE_PC */


/* Globals needed by the injected instrumentation. The __afl_area_initial region
   is used for instrumentation output before __afl_map_shm() has a chance to run.
   It will end up as .comm, so it shouldn't be too wasteful. */

u8  __afl_area_initial[MAP_SIZE];
u8* __afl_area_ptr = __afl_area_initial;

u32  __afl_target_map_initial[TRGT_SIZE][TRGT_MAP_SIZE];
u32 (*__target_map_ptr)[TRGT_MAP_SIZE] = __afl_target_map_initial;

__thread u32 __afl_prev_loc;


/* Running in persistent mode? */

static u8 is_persistent;


/* SHM setup. */

static void __afl_map_shm(void) {

  u8 *id_str = getenv(SHM_ENV_VAR);
  // u8 *id_str_dfg = getenv(SHM_ENV_VAR_DFG);
  u8 *id_trgt_map = getenv(SHM_ENV_TRGT_MAP);

  /* If we're running under AFL, attach to the appropriate region, replacing the
     early-stage __afl_area_initial region that is needed to allow some really
     hacky .init code to work correctly in projects such as OpenSSL. */

  /* By Moran
  I separate the memory allocation and check for target_map here
  */
  if(id_trgt_map) {

    u32 shm_map_id = atoi(id_trgt_map);
    __target_map_ptr = shmat(shm_map_id, NULL, 0);

    if (__target_map_ptr == (void *)-1) _exit(1);
  }


  if (id_str) {

    u32 shm_id = atoi(id_str);
    // u32 shm_id_dfg = atoi(id_str_dfg);

    __afl_area_ptr = shmat(shm_id, NULL, 0);
    // __afl_area_dfg_ptr = shmat(shm_id_dfg, NULL, 0);

    /* Whooooops. */

    if (__afl_area_ptr == (void *)-1) _exit(1);
    // if (__afl_area_dfg_ptr == (void *)-1) _exit(1);

    /* Write something into the bitmap so that even with low AFL_INST_RATIO,
       our parent doesn't give up on us. */

    __afl_area_ptr[0] = 1;

  }

}


/* Fork server logic. */

static void __afl_start_forkserver(void) {

  static u8 tmp[4];
  s32 child_pid;

  u8  child_stopped = 0;

  /* Phone home and tell the parent that we're OK. If parent isn't there,
     assume we're not running in forkserver mode and just execute program. */

  if (write(FORKSRV_FD + 1, tmp, 4) != 4) return;

  while (1) {

    u32 was_killed;
    int status;

    /* Wait for parent by reading from the pipe. Abort if read fails. */

    if (read(FORKSRV_FD, &was_killed, 4) != 4) _exit(1);

    /* If we stopped the child in persistent mode, but there was a race
       condition and afl-fuzz already issued SIGKILL, write off the old
       process. */

    if (child_stopped && was_killed) {
      child_stopped = 0;
      if (waitpid(child_pid, &status, 0) < 0) _exit(1);
    }

    if (!child_stopped) {

      /* Once woken up, create a clone of our process. */

      child_pid = fork();
      if (child_pid < 0) _exit(1);

      /* In child process: close fds, resume execution. */

      if (!child_pid) {

        close(FORKSRV_FD);
        close(FORKSRV_FD + 1);
        return;

      }

    } else {

      /* Special handling for persistent mode: if the child is alive but
         currently stopped, simply restart it with SIGCONT. */

      kill(child_pid, SIGCONT);
      child_stopped = 0;

    }

    /* In parent process: write PID to pipe, then wait for child. */

    if (write(FORKSRV_FD + 1, &child_pid, 4) != 4) _exit(1);

    if (waitpid(child_pid, &status, is_persistent ? WUNTRACED : 0) < 0)
      _exit(1);

    /* In persistent mode, the child stops itself with SIGSTOP to indicate
       a successful run. In this case, we want to wake it up without forking
       again. */

    if (WIFSTOPPED(status)) child_stopped = 1;

    /* Relay wait status to pipe, then loop back. */

    if (write(FORKSRV_FD + 1, &status, 4) != 4) _exit(1);

  }

}


/* A simplified persistent mode handler, used as explained in README.llvm. */

int __afl_persistent_loop(unsigned int max_cnt) {

  static u8  first_pass = 1;
  static u32 cycle_cnt;

  if (first_pass) {

    /* Make sure that every iteration of __AFL_LOOP() starts with a clean slate.
       On subsequent calls, the parent will take care of that, but on the first
       iteration, it's our job to erase any trace of whatever happened
       before the loop. */

    if (is_persistent) {

      memset(__afl_area_ptr, 0, MAP_SIZE);
      // By Moran. Initialize the map with 0
      // memset(__afl_area_dfg_ptr, 0, sizeof(u32) * DFG_MAP_SIZE);
      memset(__target_map_ptr, 0, sizeof(u32) * TRGT_SIZE * TRGT_MAP_SIZE);
      __afl_area_ptr[0] = 1;
      __afl_prev_loc = 0;
    }

    cycle_cnt  = max_cnt;
    first_pass = 0;
    return 1;

  }

  if (is_persistent) {

    if (--cycle_cnt) {

      raise(SIGSTOP);

      __afl_area_ptr[0] = 1;
      __afl_prev_loc = 0;

      return 1;

    } else {

      /* When exiting __AFL_LOOP(), make sure that the subsequent code that
         follows the loop is not traced. We do that by pivoting back to the
         dummy output region. */

      __afl_area_ptr = __afl_area_initial;
      // __afl_area_dfg_ptr = __afl_area_initial_dfg;
      __target_map_ptr = __afl_target_map_initial;

    }

  }

  return 0;

}


/* This one can be called from user code when deferred forkserver mode
    is enabled. */

void __afl_manual_init(void) {

  static u8 init_done;

  if (!init_done) {

    __afl_map_shm();
    __afl_start_forkserver();
    init_done = 1;

  }

}


/* Proper initialization routine. */

__attribute__((constructor(CONST_PRIO))) void __afl_auto_init(void) {

  is_persistent = !!getenv(PERSIST_ENV_VAR);

  if (getenv(DEFER_ENV_VAR)) return;

  __afl_manual_init();

}


/* The following stuff deals with supporting -fsanitize-coverage=trace-pc-guard.
   It remains non-operational in the traditional, plugin-backed LLVM mode.
   For more info about 'trace-pc-guard', see README.llvm.

   The first function (__sanitizer_cov_trace_pc_guard) is called back on every
   edge (as opposed to every basic block). */

void __sanitizer_cov_trace_pc_guard(uint32_t* guard) {
  __afl_area_ptr[*guard]++;
}


/* Init callback. Populates instrumentation IDs. Note that we're using
   ID of 0 as a special value to indicate non-instrumented bits. That may
   still touch the bitmap, but in a fairly harmless way. */

void __sanitizer_cov_trace_pc_guard_init(uint32_t* start, uint32_t* stop) {

  u32 inst_ratio = 100;
  u8* x;

  if (start == stop || *start) return;

  x = getenv("AFL_INST_RATIO");
  if (x) inst_ratio = atoi(x);

  if (!inst_ratio || inst_ratio > 100) {
    fprintf(stderr, "[-] ERROR: Invalid AFL_INST_RATIO (must be 1-100).\n");
    abort();
  }

  /* Make sure that the first element in the range is always set - we use that
     to avoid duplicate calls (which can happen as an artifact of the underlying
     implementation in LLVM). */

  *(start++) = R(MAP_SIZE - 1) + 1;

  while (start < stop) {

    if (R(100) < inst_ratio) *start = R(MAP_SIZE - 1) + 1;
    else *start = 0;

    start++;

  }

}

void mdafl_if_range(int index, int share_time, int val, int low, int high, int is_boolean){
  /*
  Note: is_boolean equals to 0/1, 0 means this is not a boolean condition; 1 means this is a boolean condition.
  */
  // int distance = abs(val - (low + high) / 2) - (high - low) / 2;

  // for temporary debug
  char* gc_file_path = getenv("AFL_GC_PATH");
  printf("gc_file_path is %s", gc_file_path);
  // assert(!gc_file_path && "Invalid value of AFL_GC_PATH");
  // gc_file_path = "/home/moran/Workspace/Project/DirectedFuzz/afl/MultiTest/DAFL/MultiTest/DynamicTest/swftophp-4-7/output/guarding_condition.log";

  FILE *fp = fopen(gc_file_path, "a+");
  fprintf(fp, "[if_range]: index(%d), val(%d), share(%d), low(%d), high(%d), isboolean(%d)\n", index, val, share_time, low, high, is_boolean);

  fclose(fp);

}


// This is the modified log function, whose parameter are long long array
// Original Version
// void mdafl_gc_log(int index, long long * low, long long * high, long long * expr, int andor){
//   char* gc_file_path = getenv("AFL_GC_PATH");
//   printf("gc_file_path is %s", gc_file_path);
//   // assert(!gc_file_path && "Invalid value of AFL_GC_PATH");
//   // gc_file_path = "/home/moran/Workspace/Project/DirectedFuzz/afl/MultiTest/DAFL/MultiTest/DynamicTest/swftophp-4-7/output/guarding_condition.log";

//   FILE *fp = fopen(gc_file_path, "a+");
//   fprintf(fp, "[if_range]: index(%d), low[0](%lld), high[0](%lld), expr[0](%lld), andor(%d)\n", index, low[0], high[0], expr[0], andor);
//   fprintf(fp, "[if_range]: index(%d), low[1](%lld), high[1](%lld), expr[1](%lld), andor(%d)\n", index, low[1], high[1], expr[1], andor);

//   fclose(fp);

// }


// Latest version
// Write the share memory here
void mdafl_gc_log_old(int index, int share, int is_negated, long long * low, long long * high, long long * expr, int andor){
  char* gc_file_path = getenv("AFL_GC_PATH");
  printf("gc_file_path is %s", gc_file_path);
  // assert(!gc_file_path && "Invalid value of AFL_GC_PATH");
  // gc_file_path = "/home/moran/Workspace/Project/DirectedFuzz/afl/MultiTest/DAFL/MultiTest/DynamicTest/swftophp-4-7/output/guarding_condition.log";

  FILE *fp = fopen(gc_file_path, "a+");
  fprintf(fp, "[if_range]: index(%d), share(%d), negated(%d), low(%lld, %lld), high(%lld, %lld), expr(%lld, %lld), andor(%d)\n", index, share, is_negated, low[0], low[1], high[0], high[1], expr[0], expr[1], andor);

  fclose(fp);

}


void mdafl_gc_log(int index, int share, int is_negated, long long * low, long long * high, long long * expr, int andor){
  // // For add logs
  // FILE *file;
  // char filename[] = "/data/sda_18tdisk/morangeous/Fuzzing/whole-pipeline/data/debug-data/fuzz-workspace/fuzz.log";
  // time_t now;
  // struct tm *timeinfo;

  // // 打开文件，以追加模式写入
  // file = fopen(filename, "a");
  // if (file == NULL) {
  //     perror("Cannot open file");
  //     return 1;
  // }
  


  char* shmid_ptr_ptr = getenv("SHMID_PTR_PTR");
  if (shmid_ptr_ptr == NULL){
      perror("[Instrumentation]: SHMID_PTR_PTR environment variable not set\n");
      exit(1);
  }

  // Get the shmid from shm_id_ptr_ptr, whose shm point to the target shmid. Therefore, it is shm_id_ptr
  int shmid_ptr = atoi(shmid_ptr_ptr);
  if (shmid_ptr == 0) {
      perror("[Instrumentation]: SHMID_PTR parse error\n");
      exit(1);
  }

  int* target_shmid = (int*) shmat(shmid_ptr, NULL, 0);
  if (target_shmid == (void*)-1) {
    perror("[Instrumentation]: shmat failed");
    exit(1);
  }

  // Get the target share memory address
  SharedMemory* shm_ptr = (SharedMemory*) shmat(*target_shmid, NULL, 0);
  
  // If array is full of data, cover the oldest data
  if (shm_ptr->count < MAX_LOG_NUM) {
      
      shm_ptr->count++;
  }

  // Write new data to tail pointer
  shm_ptr->log_data[shm_ptr->tail].index = index;
  shm_ptr->log_data[shm_ptr->tail].share = share;
  shm_ptr->log_data[shm_ptr->tail].is_negated = is_negated;
  shm_ptr->log_data[shm_ptr->tail].low[0] = low[0];
  shm_ptr->log_data[shm_ptr->tail].low[1] = low[1];
  shm_ptr->log_data[shm_ptr->tail].high[0] = high[0];
  shm_ptr->log_data[shm_ptr->tail].high[1] = high[1];
  shm_ptr->log_data[shm_ptr->tail].expr[0] = expr[0];
  shm_ptr->log_data[shm_ptr->tail].expr[1] = expr[1];
  shm_ptr->log_data[shm_ptr->tail].andor = andor;

  // Update the tail pointer
  shm_ptr->tail = (shm_ptr->tail + 1) % MAX_LOG_NUM;
  
  // detach shm_ptr and target_shmid
  if (shmdt(shm_ptr) == -1){
    perror("[Instrumentation]: shm_ptr shmdt failed");
    exit(1);
  }
  if (shmdt(target_shmid) == -1){
    perror("[Instrumentation]: target_shmid shmdt failed");
    exit(1);
  }

}