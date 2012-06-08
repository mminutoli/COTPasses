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
#include "cot/PostDomFrontier/PostDominanceFrontier.h"
#include "llvm/Function.h"
#include "llvm/Support/raw_ostream.h"


using namespace cot;
using namespace llvm;


char ControlDependencyGraph::ID = 0;


bool ControlDependencyGraph::runOnFunction(Function &F)
{
  PostDominanceFrontier &PDF = getAnalysis<PostDominanceFrontier>();

  std::vector<BasicBlock *> Roots = PDF.getRoots();
  if (Roots.size() > 1)
  {
    // I want to create a common root but I am not sure this is the way.
    for (std::vector<BasicBlock *>::const_iterator I = Roots.begin(),
             E = Roots.end(); I != E; ++I)
      CDG->addDependency(*I, 0, CONTROL);
  }
  for (PostDominanceFrontier::const_iterator I = PDF.begin(), E = PDF.end();
       I != E; ++I)
  {
    const std::set<BasicBlock *> &nodeFrontier = I->second;
    if (!nodeFrontier.empty())
    {
      for (std::set<BasicBlock *>::const_iterator FI = nodeFrontier.begin(),
               FE = nodeFrontier.end();
           FI != FE; ++FI)
      {
        CDG->addDependency(*FI, I->first, CONTROL);
      }
    }
  }
  return false;
}


void ControlDependencyGraph::getAnalysisUsage(AnalysisUsage &AU) const
{
  AU.setPreservesAll();
  AU.addRequired<PostDominanceFrontier>();
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
