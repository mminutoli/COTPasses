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


static bool buildPath(DomTreeNode *root, DomTreeNode *dest,
                      std::vector<BasicBlock *> &path)
{
  if (root == dest) return true;

  bool found = false;
  for (DomTreeNode::iterator I = root->begin(), E = root->end();
       !found && I != E; ++I)
  {
    path.push_back((*I)->getBlock());
    found = buildPath(*I, dest, path);
    if (!found) path.pop_back();
  }

  return found;
}


char ControlDependencyGraph::ID = 0;


bool ControlDependencyGraph::runOnFunction(Function &F)
{
  PostDominatorTree &PDT = getAnalysis<PostDominatorTree>();

  std::vector<std::pair<BasicBlock *, BasicBlock *> > EdgeSet;

  // The code commented out below is a desperate attempt to add a node
  // to the CFG in a way that shold make the algorithm work properly.
  /*
  BasicBlock * Start = BasicBlock::Create(F.getContext(), "START", &F);
  BranchInst::Create(Start, &F.getEntryBlock());
  EdgeSet.push_back(std::make_pair(Start, &F.getEntryBlock()));
  */
  PDT.DT->addNewBlock(Start, PDT.DT->getRoot());
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

    std::vector<BasicBlock *> path;
    bool found = buildPath(PDT.getNode(BB), PDT.getNode(Edge.second), path);

    while (found && !path.empty())
    {
      BasicBlock * node = path.back();
      CDG->addDependency(Edge.first, node, CONTROL);
      path.pop_back();
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


INITIALIZE_PASS(ControlDependencyGraph, "cdgraph",
                "Control Dependency Graph Contruction",
                true,
                true)
