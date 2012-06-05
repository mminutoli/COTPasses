/** ---*- C++ -*--- ControlDependencies.cpp
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

#include "cot/DependencyGraph/ControlDependencies.h"

#include "cot/AllPasses.h"
#include "llvm/Function.h"
#include "llvm/Support/raw_ostream.h"


using namespace cot;
using namespace llvm;


char ControlDependencyGraph::ID = 0;


bool ControlDependencyGraph::runOnFunction(Function &F)
{
  for (Function::BasicBlockListType::const_iterator it = F.getBasicBlockList().begin(); it != F.getBasicBlockList().end(); ++it)
    for (Function::BasicBlockListType::const_iterator it2 = F.getBasicBlockList().begin(); it2 != F.getBasicBlockList().end(); ++it2)
      CDG->addDependency(&*it, &*it2, DATA);
  return false;
}


void ControlDependencyGraph::getAnalysisUsage(AnalysisUsage &AU) const
{
  AU.setPreservesAll();
}


void ControlDependencyGraph::print(raw_ostream &OS, const Module*) const
{
  CDG->print(OS, getPassName());
}


ControlDependencyGraph *CreateControlDependencyGraphPass()
{
  return new ControlDependencyGraph();
}


INITIALIZE_PASS(ControlDependencyGraph, "cdgraph",
                "Control Dependency Graph Contruction",
                true,
                true)
