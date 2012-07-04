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


using namespace cot;
using namespace llvm;


char DataDependencyGraph::ID = 0;

bool DataDependencyGraph::runOnFunction(llvm::Function &F)
{
   // Data dependency between temporaries. It's easy to detect a DD between
   // temporaries because LLVM uses the SSA form. So in orderd to detect a DD,
   // it suffices to find all operands in an instruction of a basic block and
   // add a dependency between that basic block and the one which contains
   // the instruction that defines the operand.      
   for (Function::BasicBlockListType::const_iterator it = F.getBasicBlockList().begin(); it != F.getBasicBlockList().end(); ++it) {
      for (BasicBlock::const_iterator iit = it->begin(); iit != it->end(); ++iit ) {
         for (Instruction::const_op_iterator cuit = iit->op_begin(); cuit != iit->op_end(); ++cuit) {
            Instruction* pInstruction = dyn_cast< Instruction>(*cuit);
            if(pInstruction) {
               DDG->addDependency(&*it, pInstruction->getParent(), DATA);
            }
         }
      }
   }
   return false;
}


void DataDependencyGraph::getAnalysisUsage(AnalysisUsage &AU) const
{
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


INITIALIZE_PASS(DataDependencyGraph, "ddgraph",
        "Data Dependency Graph Construction",
        true,
        true)
