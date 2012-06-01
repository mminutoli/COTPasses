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
   for (Function::BasicBlockListType::const_iterator it = F.getBasicBlockList().begin(); it != F.getBasicBlockList().end(); ++it)
      for (Function::BasicBlockListType::const_iterator it2 = F.getBasicBlockList().begin(); it2 != F.getBasicBlockList().end(); ++it2)
         DDG->addDependency(&*it, &*it2, DATA);
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
