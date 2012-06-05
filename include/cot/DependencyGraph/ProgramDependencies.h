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
#include "cot/DependencyGraph/DependencyGraph.h"

namespace cot {

typedef DependencyGraph<llvm::BasicBlock> ProgramDepGraph;

/*!
 * Program Dependencies Graph
 */
class ProgramDependencyGraph : public llvm::FunctionPass
{
public:
  static char ID; // Pass ID, replacement for typeid
  ProgramDepGraph *PDG;

  ProgramDependencyGraph() : llvm::FunctionPass(ID)
  {
    PDG = new ProgramDepGraph();
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

  void print(llvm::raw_ostream &OS, const llvm::Module* M = 0) const;
};

}

namespace llvm
{

  template <> struct GraphTraits<cot::ProgramDependencyGraph *>
      : public GraphTraits<cot::DepGraphNode*> {
    static NodeType *getEntryNode(cot::ProgramDependencyGraph *PG) {
      return *(PG->PDG->begin_children());
    }

    static nodes_iterator nodes_begin(cot::ProgramDependencyGraph *N) {
      return df_begin(getEntryNode(N));
    }

    static nodes_iterator nodes_end(cot::ProgramDependencyGraph *N) {
      return df_end(getEntryNode(N));
    }
  };

}


#endif // PROGRAMDEPENDENCIES_H
