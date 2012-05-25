/** ---*- C++ -*--- DataDependencies.h
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



#ifndef DATADEPENDENCIES_H
#define DATADEPENDENCIES_H

#include "llvm/Pass.h"


namespace cot {

using namespace llvm;

/*!
 * Data Dependency Graph Node
 */
template <class NodeT>
class DataDependencyGraphNodeBase
{
public:

private:
  NodeT *TheBB;
  std::vector<DataDependencyGraphNodeBase<NodeT> *> Neighbours;
};


template <class NodeT>
class DataDependencyGraphBase
{
public:

};

class DataDependencyGraph : public FunctionPass
{
public:
  static char ID; // Pass ID, replacement for typeid
  DataDependencyGraphBase<BasicBlock>* DDT;

  DataDependencyGraph() : FunctionPass(ID)
  {
    DDT = new DataDependencyGraphBase<BasicBlock>();
  }

  ~DataDependencyGraph()
  {
    delete DDT;
  }

  virtual bool runOnFunction(Function &F);

  virtual void getAnalysisUsage(AnalysisUsage &AU) const;

  virtual const char *getPassName() const
  {
    return "Data Dependency Graph";
  }
};

}

#endif // DATADEPENDENCIES_H
