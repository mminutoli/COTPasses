/** ---*- C++ -*--- ControlDependencies.h
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



#ifndef CONTROLDEPENDENCIES_H
#define CONTROLDEPENDENCIES_H

#include "llvm/Pass.h"
#include "cot/Common/DependencyGraph.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/Support/raw_ostream.h"

namespace cot
{
  typedef DependencyGraph<llvm::BasicBlock> ControlDepGraph;

  /*!
   * Control Dependency Graph
   */
  class ControlDependencyGraph : public llvm::FunctionPass
  {
  public:
    static char ID; // Pass ID, replacement for typeid
    ControlDepGraph *CDG;

    ControlDependencyGraph() : llvm::FunctionPass(ID)
    {
      CDG = new ControlDepGraph();
    }

    ~ControlDependencyGraph()
    {
      delete CDG;
    }

    bool runOnFunction(llvm::Function &F);

    void getAnalysisUsage(llvm::AnalysisUsage &AU) const;

    const char *getPassName() const
    {
      return "Control Dependency Graph";
    }

    void print(llvm::raw_ostream &OS, const llvm::Module* M = 0) const;
  };
}

#endif // CONTROLDEPENDENCIES_H
