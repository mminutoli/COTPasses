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

#include "cot/ControlDependencyGraph/ControlDependencies.h"

#include "cot/AllPasses.h"
#include "llvm/Support/raw_ostream.h"


using namespace cot;
using namespace llvm;


char ControlDependencyGraph::ID = 0;


bool ControlDependencyGraph::runOnFunction(Function &F)
{
  errs() << "Run CDG construction\n";
  return false;
}


void ControlDependencyGraph::getAnalysisUsage(AnalysisUsage &AU) const
{
  AU.setPreservesAll();
}


void ControlDependencyGraph::print(raw_ostream &o, const Module*) const
{
  CDG->print(o);
}


ControlDependencyGraph *CreateControlDependencyGraphPass()
{
  return new ControlDependencyGraph();
}


INITIALIZE_PASS(ControlDependencyGraph, "cdgraph",
                "Control Dependency Graph Contruction",
                true,
                true)


/*!
 * Control Dependency Graph Printer Pass
 */
namespace {
class ControlDependencyGraphPrinter : public FunctionPass
{
public:
  static char ID; // Pass identification, replacement for typeid
  ControlDependencyGraphPrinter() : FunctionPass(ID) {}

  void getAnalysisUsage(AnalysisUsage &AU) const
  {
    AU.setPreservesAll();
    AU.addRequired<ControlDependencyGraph>();
  }

  bool runOnFunction(Function &F)
  {
    getAnalysis<ControlDependencyGraph>().dump();
    return false;
  }
};
}

char ControlDependencyGraphPrinter::ID = 0;
INITIALIZE_PASS(ControlDependencyGraphPrinter, "print-cdg",
                "Print Control Dependency Graph",
                true,
                true)

Pass *CreateControlDependencyGraphPrinterPass()
{
  return new ControlDependencyGraphPrinter();
}
