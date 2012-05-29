/** ---*- C++ -*--- ProgramDependencies.cpp
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


#include "cot/ProgramDependencyGraph/ProgramDependencies.h"

#include "cot/AllPasses.h"
#include "llvm/Support/raw_ostream.h"


using namespace cot;
using namespace llvm;


char ProgramDependencyGraph::ID = 0;


bool ProgramDependencyGraph::runOnFunction(Function &F)
{
  errs() << "Run PDG construction\n";
  return false;
}


void ProgramDependencyGraph::getAnalysisUsage(AnalysisUsage &AU) const
{
  AU.setPreservesAll();
}


ProgramDependencyGraph *CreateProgramDependencyGraphPass()
{
  return new ProgramDependencyGraph();
}


INITIALIZE_PASS(ProgramDependencyGraph, "pdgraph",
                "Program Dependency Graph Construction",
                true,
                true)