/** ---*- C++ -*--- ProgramDependencies.h
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



#ifndef PROGRAMDEPENDENCIES_H
#define PROGRAMDEPENDENCIES_H

#include "llvm/Pass.h"


namespace cot {

/*!
 * Program Dependencies Graph Node
 */
template <class NodeT>
class ProgramDependencyGraphNodeBase
{
public:

private:
  NodeT *TheBB;
  std::vector<ProgramDependencyGraphNodeBase<NodeT> *> Neighbours;
};


template <class NodeT>
class ProgramDependencyGraphBase
{
public:

};


class ProgramDependencyGraph : public llvm::FunctionPass
{
public:
  static char ID; // Pass ID, replacement for typeid
  ProgramDependencyGraphBase<llvm::BasicBlock> *PDG;

  ProgramDependencyGraph() : llvm::FunctionPass(ID)
  {
    PDG = new ProgramDependencyGraphBase<llvm::BasicBlock>();
  }

  ~ProgramDependencyGraph()
  {
    delete PDG;
  }

  bool runOnFunction(llvm::Function &F);

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const;

  const char *getPassName() const
  {
    return "Program Dependency Graph";
  }
};

}

#endif // PROGRAMDEPENDENCIES_H
