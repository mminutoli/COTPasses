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
#include "llvm/Support/raw_ostream.h"

namespace cot {

/*!
 * Control Dependency Graph Node
 */
template <class NodeT>
class ControlDependencyNodeBase
{
public:
  typedef typename std::vector<ControlDependencyNodeBase<NodeT> *>::iterator
                   iterator;
  typedef typename std::vector<ControlDependencyNodeBase<NodeT> *>::const_iterator
                   const_iterator;

  iterator begin() { return Neighbours.begin(); }
  iterator end() { return Neighbours.end(); }
  const_iterator begin() { return Neighbours.begin(); }
  const_iterator end() { return Neighbours.end(); }

  NodeT *getBlock() const { return TheBB; }

private:
  NodeT *TheBB;
  std::vector<ControlDependencyNodeBase<NodeT> *> Neighbours;
};


template <class NodeT>
static llvm::raw_ostream &
operator<<(llvm::raw_ostream &o, const ControlDependencyNodeBase<NodeT> *Node)
{
  o << "ToDo: Print information of nodes.";
  return o << "\n";
}


/*!
 * Control Dependency Graph
 */
template <class NodeT>
class ControlDependencyGraphBase
{
public:

};


class ControlDependencyGraph : public llvm::FunctionPass
{
public:
  static char ID; // Pass ID, replacement for typeid
  ControlDependencyGraphBase<llvm::BasicBlock> *CDG;

  ControlDependencyGraph() : llvm::FunctionPass(ID)
  {
    CDG = new ControlDependencyGraphBase<llvm::BasicBlock>();
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
};

}

#endif // CONTROLDEPENDENCIES_H
