/** ---*- C++ -*--- DataDependencies.cpp
 *
 * Copyright (C) 2012 Marco Minutoli <mminutoli@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include "cot/DependencyGraph/DataDependencies.h"

#include "cot/AllPasses.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Function.h"
#include "llvm/Type.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/MemoryDependenceAnalysis.h"

using namespace cot;
using namespace llvm;


char DataDependencyGraph::ID = 0;

bool DataDependencyGraph::runOnFunction(llvm::Function &F)
{
   //AliasAnalysis &AA = getAnalysis<AliasAnalysis>();
   MemoryDependenceAnalysis& MDA = getAnalysis<MemoryDependenceAnalysis>();
   
   for (Function::BasicBlockListType::iterator it = F.getBasicBlockList().begin(); it != F.getBasicBlockList().end(); ++it) {
      // Make sure there exists a node for each BB:
      DDG->getNodeByData(&*it);
      
      for (BasicBlock::iterator iit = it->begin(); iit != it->end(); ++iit ) {
         Instruction *pInstruction = dyn_cast<Instruction>(&*iit);
         if (isa<StoreInst>(pInstruction)) {
            MemDepResult res = MDA.getDependency(pInstruction);
            
            if (res.isDef()) {
               // There's a depenency with res.getInst()
               DDG->addDependency(&*it, res.getInst()->getParent(), DATA);
            } else if (res.isClobber()) {
               // There might be a dependency with res.getInst(). Let's be
               // conservative.
               DDG->addDependency(&*it, res.getInst()->getParent(), DATA);
            } else {
               if (res.isUnknown()) {
                  // No dependencies found.
               } else if (res.isNonFuncLocal()) {
                  // There might be some dependencies with extern instructions, but
                  // we do not care of this eventuality.
               } else if (res.isNonLocal()) {
                  // No dependency found in pInstruction's basic block, but there
                  // might be in others. To be conservative, we'll add a dependency
                  // with all the other basic blocks thSat contain an instruction
                  // that accesses memory.
                  for (Function::BasicBlockListType::iterator it2 = F.getBasicBlockList().begin(); it != F.getBasicBlockList().end(); ++it2) {
                     if (&*it2 != &*it)
                        for (BasicBlock::iterator iit2 = it2->begin(); iit2 != it2->end(); ++iit2) {
                           if (iit2->mayReadOrWriteMemory())
                              DDG->addDependency(&*it, &*it2, DATA);
                        }
                  }
               } 
            }
         }
         
         // Data dependency between temporaries. It's easy to detect a DD between
         // temporaries because LLVM uses the SSA form. So in orderd to detect a DD,
         // it suffices to find all operands in an instruction of a basic block and
         // add a dependency between that basic block and the one which contains
         // the instruction that defines the operand.      
         for (Instruction::const_op_iterator cuit = iit->op_begin(); cuit != iit->op_end(); ++cuit)
            if(Instruction* pInstruction = dyn_cast<Instruction>(*cuit))
               DDG->addDependency(pInstruction->getParent(), &*it, DATA);
      }
   }
   return false;
}


void DataDependencyGraph::getAnalysisUsage(AnalysisUsage &AU) const
{
   AU.addRequiredTransitive<AliasAnalysis>();
   AU.addRequiredTransitive<MemoryDependenceAnalysis>();
   AU.setPreservesAll();
}


void DataDependencyGraph::print(raw_ostream &OS, const Module*) const
{
  DDG->print(OS, getPassName());
}


DataDependencyGraph *cot::CreateDataDependencyGraphPass()
{
   return new DataDependencyGraph();
}

INITIALIZE_PASS(DataDependencyGraph, "ddg",
        "Data Dependency Graph Construction",
        true,
        true)
