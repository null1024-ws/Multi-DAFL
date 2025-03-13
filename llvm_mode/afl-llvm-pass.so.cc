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
   american fuzzy lop - LLVM-mode instrumentation pass
   ---------------------------------------------------

   Written by Laszlo Szekeres <lszekeres@google.com> and
              Michal Zalewski <lcamtuf@google.com>

   LLVM integration design comes from Laszlo Szekeres. C bits copied-and-pasted
   from afl-as.c are Michal's fault.

   This library is plugged into LLVM when invoking clang through afl-clang-fast.
   It tells the compiler to add code roughly equivalent to the bits discussed
   in ../afl-as.h.
*/

#define AFL_LLVM_PASS

#include "../config.h"
#include "../debug.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "llvm/Support/CommandLine.h"

using namespace llvm;

bool selective_coverage = false;
bool dfg_scoring = false;
bool no_filename_match = false;
// std::set<std::string> instr_targets;
// std::map<std::string,std::pair<unsigned int,unsigned int>> dfg_node_map;
std::vector<std::map<std::string, std::set<std::string>>> instr_targets_vector;
// dfg_map_vector is a vector of maps of function names and their DFG scores
std::vector<std::map<std::string,std::pair<unsigned int,unsigned int>>> dfg_map_vector;
// target file names
std::vector<std::string> target_file_names;

namespace {

  class AFLCoverage : public ModulePass {

    public:

      static char ID;
      AFLCoverage() : ModulePass(ID) { }

      bool runOnModule(Module &M) override;

      // StringRef getPassName() const override {
      //  return "American Fuzzy Lop Instrumentation";
      // }

  };

}


/**
 * Retrieves a list of file names in the specified folder.
 *
 * @param folderPath The path to the folder.
 *
 * @return A vector containing the file names in the folder.
 *
 * @throws None
 * 
 * @author Morangeous
 */
std::vector<std::string> getFilesInFolder(const std::string& folderPath) {
    std::vector<std::string> fileNames;

    DIR* dir = opendir(folderPath.c_str());
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_REG) {  // Check if it's a regular file
                fileNames.push_back(entry->d_name);
            }
        }
        closedir(dir);
    }

    return fileNames;
}


void initCoverageTarget(const char* select_file) {
  std::string line;
  std::ifstream stream(select_file);
  std::map<std::string, std::set<std::string>> instr_targets;
  
  while (std::getline(stream, line)){
    std::size_t colon = line.find(":");
    std::string target_file = line.substr(0, colon);
    std::string target_func = line.substr(colon+1, std::string::npos);
    instr_targets[target_file].insert(target_func);
  }
  
  instr_targets_vector.push_back(instr_targets);
}


void initDFGNodeMap(const char* dfg_file) {
  unsigned int idx = 0;
  std::string line;
  std::ifstream stream(dfg_file);
  std::map<std::string,std::pair<unsigned int,unsigned int>> dfg_node_map;

  while (std::getline(stream, line)) {
    std::size_t space_idx = line.find(" ");
    std::string score_str = line.substr(0, space_idx);
    std::string targ_line = line.substr(space_idx + 1, std::string::npos);
    int score = stoi(score_str);
    dfg_node_map[targ_line] = std::make_pair(idx++, (unsigned int) score);
    if (idx >= TRGT_MAP_SIZE) {
      std::cout << "Input DFG is too large (check TRGT_MAP_SIZE)" << std::endl;
      exit(1);
    }
  }
  dfg_map_vector.push_back(dfg_node_map);
}

void initialize(void) {
  
  // select_folder and dfg_folder are all folder paths of target files.
  char* select_folder = getenv("DAFL_SELECTIVE_COV");
  char* dfg_folder = getenv("DAFL_DFG_SCORE");

  std::vector<std::string> select_files = getFilesInFolder(select_folder);
  std::vector<std::string> dfg_files = getFilesInFolder(dfg_folder);

  // align the file name of select file and dfg file 
  if(select_files.size() != dfg_files.size()) {
    FATAL("The number of files in select folder and dfg folder are not equal, select_files: %zu, dfg_files: %zu, select_path: %s, dfg_path: %s", select_files.size(), dfg_files.size(), select_folder, dfg_folder);
  }

  // begin to cope with all of the target files
  for(unsigned int i = 0; i < select_files.size(); i++) {
    // read select and dfg file
    std::string select_file = select_files[i];
    std::string dfg_file = dfg_files[i];
    
    // Check if the file name of select folder and dfg folder are equal
    if(select_file != dfg_file) {
      FATAL("The file name of select folder and dfg folder are not equal");
    }

    // Set target file names
    target_file_names.push_back(select_file);

    // Check if the file name of select folder and dfg file name are not empty
    if(!select_file.empty()) {
      selective_coverage = true;
      initCoverageTarget((std::string(select_folder) + "/" + select_file).c_str());  
    }

    if(!dfg_file.empty()) {
      dfg_scoring = true;
      initDFGNodeMap((std::string(dfg_folder) + "/" + dfg_file).c_str());
    }
  }

  // do not know the functionality of this flag, let it alone
  if (getenv("DAFL_NO_FILENAME_MATCH")) no_filename_match = true;

}


char AFLCoverage::ID = 0;


bool AFLCoverage::runOnModule(Module &M) {

  LLVMContext &C = M.getContext();

  IntegerType *Int8Ty  = IntegerType::getInt8Ty(C);
  IntegerType *Int32Ty = IntegerType::getInt32Ty(C);

  initialize();

  /* Get globals for the SHM region and the previous location. Note that
     __afl_prev_loc is thread-local. */

  GlobalVariable *AFLMapPtr =
      new GlobalVariable(M, PointerType::get(Int8Ty, 0), false,
                         GlobalValue::ExternalLinkage, 0, "__afl_area_ptr");

  // Create Target Map Pointer Here
  GlobalVariable *TargetMapPtr = new GlobalVariable(
    M, 
    PointerType::get(PointerType::get(Int32Ty, 0), 0), 
    false,
    GlobalVariable::ExternalLinkage, 
    0, 
    "__target_map_ptr"
    );

  GlobalVariable *AFLPrevLoc = new GlobalVariable(
      M, Int32Ty, false, GlobalValue::ExternalLinkage, 0, "__afl_prev_loc",
      0, GlobalVariable::GeneralDynamicTLSModel, 0, false);

  /* Instrument all the things! */

  int inst_blocks = 0;
  int skip_blocks = 0;
  int inst_dfg_nodes = 0;
  std::string file_name = M.getSourceFileName();
  std::set<std::string> covered_targets;

  

  for (auto &F : M) {

    // Get file name from function in case the module is a combined bc file.
    if (auto *SP = F.getSubprogram()) {
        file_name = SP->getFilename().str();
    }

    // Keep only the file name.
    std::size_t tokloc = file_name.find_last_of('/');
    if (tokloc != std::string::npos) {
      file_name = file_name.substr(tokloc + 1, std::string::npos);
    }

    bool is_inst_targ = false;
    const std::string func_name = F.getName().str();
    std::vector<int> target_vector_index;

   
    /*
    In the latest version, we only record the target vector index. 
    The index i means that No.i target file contains the current function score, which is needed to be instrumented
    */
    if (selective_coverage){
      for(int i = 0; i < instr_targets_vector.size(); i++){
        std::set<std::string> func_name_set = instr_targets_vector[i][file_name];
        bool is_file_exist = !func_name_set.empty();
        bool is_func_exist = func_name_set.find(func_name) != func_name_set.end();
        if(is_file_exist && is_func_exist){
          is_inst_targ = true;
          target_vector_index.push_back(i);
        }
      }
    } else {
      is_inst_targ = true;
    }

    /* Now iterate through the basic blocks of the function. */

    for (auto &BB : F) {
      bool is_dfg_node = false;
      std::vector<std::pair<unsigned int, unsigned int>> idx_score_vec;
      std::vector<int> update_target_idx;

      if (is_inst_targ) {
        inst_blocks++;
      }
      else {
        skip_blocks++;
        continue;
      }

      /* Iterate through the instructions in the basic block to check if this
       * block is a DFG node. If so, retrieve its proximity score. */

      if (dfg_scoring) {
        for (auto &inst : BB) {
          DebugLoc dbg = inst.getDebugLoc();
          DILocation* DILoc = dbg.get();
          if (DILoc && DILoc->getLine()) {
            int line_no = DILoc->getLine();
            std::ostringstream stream;
            stream << file_name << ":" << line_no;
            std::string targ_str = stream.str();
            std::map<std::string,std::pair<unsigned int,unsigned int>> dfg_node_map;
            for (int i = 0; i < target_vector_index.size(); i++) {
              dfg_node_map = dfg_map_vector[target_vector_index[i]];
              if (dfg_node_map.count(targ_str) > 0){
                // Attention: the target index is updated here
                update_target_idx.push_back(target_vector_index[i]);
                is_dfg_node = true;
                idx_score_vec.push_back(dfg_node_map[targ_str]);
                inst_dfg_nodes++;
                // we don't break, it is because we need to insert more than one node.
              }
              
            }
          }
      }

      BasicBlock::iterator IP = BB.getFirstInsertionPt();
      IRBuilder<> IRB(&(*IP));

      /* Make up cur_loc */

      unsigned int cur_loc = AFL_R(MAP_SIZE);

      ConstantInt *CurLoc = ConstantInt::get(Int32Ty, cur_loc);

      /* Load prev_loc */

      LoadInst *PrevLoc = IRB.CreateLoad(AFLPrevLoc);
      PrevLoc->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));
      Value *PrevLocCasted = IRB.CreateZExt(PrevLoc, IRB.getInt32Ty());

      /* Load SHM pointer */

      LoadInst *MapPtr = IRB.CreateLoad(AFLMapPtr);
      MapPtr->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));
      Value *MapPtrIdx =
          IRB.CreateGEP(MapPtr, IRB.CreateXor(PrevLocCasted, CurLoc));

      /* Update bitmap */

      LoadInst *Counter = IRB.CreateLoad(MapPtrIdx);
      Counter->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));
      Value *Incr = IRB.CreateAdd(Counter, ConstantInt::get(Int8Ty, 1));
      IRB.CreateStore(Incr, MapPtrIdx)
          ->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));

      /* Set prev_loc to cur_loc >> 1 */

      StoreInst *Store =
          IRB.CreateStore(ConstantInt::get(Int32Ty, cur_loc >> 1), AFLPrevLoc);
      Store->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));

      /* We insert DFG code for multiple targets here*/
      if (is_dfg_node){
        // iteractor of idx_score_vec
        std::pair<unsigned int, unsigned int> idx_score;
        int current_target;
        // insert IR code
        LoadInst *TargetMapPtrValue = IRB.CreateLoad(TargetMapPtr);
        TargetMapPtrValue->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));
        Value *TargetIdx, *TargetPtr,*ElementIdx, *ElementPtr, *ScoreStore;

        for (int i = 0; i < idx_score_vec.size(); i++) {
          current_target = update_target_idx[i];
          idx_score = idx_score_vec[i];
          TargetIdx = ConstantInt::get(Int32Ty, current_target);
          ElementIdx = ConstantInt::get(Int32Ty, idx_score.first);
          TargetPtr = IRB.CreateGEP(TargetMapPtrValue, TargetIdx);
          ElementPtr = IRB.CreateGEP(TargetPtr, ElementIdx);
          ScoreStore = ConstantInt::get(Int32Ty, idx_score.second);
          IRB.CreateStore(ScoreStore, ElementPtr)
              ->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));
          
        }  
      }

    }
  }
  }

  /* Say something nice. */
  for (auto it = covered_targets.begin(); it != covered_targets.end(); ++it)
    std::cout << "Covered " << (*it) << std::endl;
  OKF("Selected blocks: %u, skipped blocks: %u. instrumented DFG nodes: %u",
      inst_blocks, skip_blocks, inst_dfg_nodes);

  return true;

}


static void registerAFLPass(const PassManagerBuilder &,
                            legacy::PassManagerBase &PM) {

  PM.add(new AFLCoverage());

}


static RegisterStandardPasses RegisterAFLPass(
    PassManagerBuilder::EP_ModuleOptimizerEarly, registerAFLPass);

static RegisterStandardPasses RegisterAFLPass0(
    PassManagerBuilder::EP_EnabledOnOptLevel0, registerAFLPass);
