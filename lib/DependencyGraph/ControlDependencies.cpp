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
#include "llvm/Instructions.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Support/raw_ostream.h"


using namespace cot;
using namespace llvm;


char ControlDependencyGraph::ID = 0;


bool ControlDependencyGraph::runOnFunction(Function &F)
{
  PostDominatorTree &PDT = getAnalysis<PostDominatorTree>();

  /*
   * The EdgeSet should always contains the Start->EntryNode
   * edge. This will lead to add every node in the path from the
   * ExitNode(the immediate postdom of Start) and the EntryNode as
   * control dependent on Start.
   */
  DomTreeNode *entryNode = PDT[&F.getEntryBlock()];
  while (entryNode && entryNode->getBlock())
  {
    /*
     * Walking the path backward and adding dependencies.
     */
    CDG->addDependency(static_cast<BasicBlock *>(0),
                       entryNode->getBlock(), CONTROL);
    entryNode = entryNode->getIDom();
  }

  std::vector<std::pair<BasicBlock *, BasicBlock *> > EdgeSet;
  for (Function::iterator I = F.begin(), E = F.end(); I != E; ++I)
  {
    for (succ_iterator SI = succ_begin(I), SE = succ_end(I); SI != SE; ++SI)
    {
      if (!PDT.properlyDominates(*SI, I))
        EdgeSet.push_back(std::make_pair(I, *SI));
    }
  }

  typedef std::vector<std::pair<BasicBlock *, BasicBlock *> >::iterator EdgeItr;
  for (EdgeItr I = EdgeSet.begin(), E = EdgeSet.end(); I != E; ++I)
  {
    std::pair<BasicBlock *, BasicBlock *> Edge = *I;
    BasicBlock *BB = PDT.findNearestCommonDominator(Edge.first, Edge.second);

    DomTreeNode *domNode = PDT[Edge.second];
    while (domNode->getBlock() != BB)
    {
      CDG->addDependency(Edge.first, domNode->getBlock(), CONTROL);
      domNode = domNode->getIDom();
    }
  }
  return false;
}


void ControlDependencyGraph::getAnalysisUsage(AnalysisUsage &AU) const
{
  AU.setPreservesAll();
  AU.addRequired<PostDominatorTree>();
}


void ControlDependencyGraph::print(raw_ostream &OS, const Module*) const
{
  CDG->print(OS, getPassName());
}


ControlDependencyGraph *CreateControlDependencyGraphPass()
{
  return new ControlDependencyGraph();
}


INITIALIZE_PASS(ControlDependencyGraph, "cdg",
                "Control Dependency Graph Contruction",
                true,
                true)
