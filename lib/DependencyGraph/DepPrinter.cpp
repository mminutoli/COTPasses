/** ---*- C++ -*--- DepPrinter.cpp
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

#include "cot/DependencyGraph/DependencyGraph.h"

#include "cot/AllPasses.h"
#include "cot/DependencyGraph/DataDependencies.h"
#include "cot/DependencyGraph/ControlDependencies.h"
#include "cot/DependencyGraph/ProgramDependencies.h"
#include "llvm/Analysis/DOTGraphTraitsPass.h"


using namespace llvm;
using namespace cot;


namespace llvm {

template <>
struct DOTGraphTraits<cot::DepGraphNode *> : public DefaultDOTGraphTraits {
  DOTGraphTraits (bool isSimple = false)
      : DefaultDOTGraphTraits(isSimple) {}

  std::string getNodeLabel(cot::DepGraphNode *Node, cot::DepGraphNode *Graph)
  {
    const BasicBlock *BB = Node->getData();

    if (isSimple())
      return DOTGraphTraits<const Function *>
          ::getSimpleNodeLabel(BB, BB->getParent());
    else
      return DOTGraphTraits<const Function *>
          ::getCompleteNodeLabel(BB, BB->getParent());
  }
};

template <>
struct DOTGraphTraits<cot::DepGraph *>
    : public DOTGraphTraits<cot::DepGraphNode *>
{
  DOTGraphTraits (bool isSimple = false)
      : DOTGraphTraits<cot::DepGraphNode *>(isSimple) {}

  std::string getNodeLabel(cot::DepGraphNode *Node, cot::DepGraph *Graph)
  {
    return DOTGraphTraits<DepGraphNode *>
        ::getNodeLabel(Node,*(Graph->begin_children()));
  }
};


template <>
struct DOTGraphTraits<cot::DataDependencyGraph *>
    : public DOTGraphTraits<cot::DepGraph *>
{
  DOTGraphTraits (bool isSimple = false)
      : DOTGraphTraits<cot::DepGraph *>(isSimple) {}

  static std::string getGraphName(DataDependencyGraph *)
  {
    return "Data dependency graph";
  }

  std::string getNodeLabel(cot::DepGraphNode *Node,
                           cot::DataDependencyGraph *Graph)
  {
    return DOTGraphTraits<DepGraph *>
        ::getNodeLabel(Node, Graph->DDG);
  }
};


template <>
struct DOTGraphTraits<cot::ControlDependencyGraph *>
    : public DOTGraphTraits<cot::DepGraph *>
{
  DOTGraphTraits (bool isSimple = false)
      : DOTGraphTraits<cot::DepGraph *>(isSimple) {}

  static std::string getGraphName(ControlDependencyGraph *)
  {
    return "Control dependency graph";
  }

  std::string getNodeLabel(cot::DepGraphNode *Node,
                           cot::ControlDependencyGraph *Graph)
  {
    return DOTGraphTraits<DepGraph *>
        ::getNodeLabel(Node, Graph->CDG);
  }
};


template <>
struct DOTGraphTraits<cot::ProgramDependencyGraph *>
    : public DOTGraphTraits<cot::DepGraph *>
{
  DOTGraphTraits (bool isSimple = false)
      : DOTGraphTraits<cot::DepGraph *>(isSimple) {}

  static std::string getGraphName(ProgramDependencyGraph *)
  {
    return "Program dependency graph";
  }

  std::string getNodeLabel(cot::DepGraphNode *Node,
                           cot::ProgramDependencyGraph *Graph)
  {
    return DOTGraphTraits<DepGraph *>
        ::getNodeLabel(Node, Graph->PDG);
  }
};
}

namespace cot
{

namespace {
struct DataDependencyViewer
    : public DOTGraphTraitsViewer<DataDependencyGraph, false>
{
  static char ID;
  DataDependencyViewer() :
      DOTGraphTraitsViewer<DataDependencyGraph, false>("ddgraph", ID) {}
};


struct ControlDependencyViewer
    : public DOTGraphTraitsViewer<ControlDependencyGraph, false>
{
  static char ID;
  ControlDependencyViewer() :
      DOTGraphTraitsViewer<ControlDependencyGraph, false>("cdgraph", ID) {}
};


struct ProgramDependencyViewer
    : public DOTGraphTraitsViewer<ProgramDependencyGraph, false>
{
  static char ID;
  ProgramDependencyViewer() :
      DOTGraphTraitsViewer<ProgramDependencyGraph, false>("pdgraph", ID) {}
};
}

}

char DataDependencyViewer::ID = 0;
INITIALIZE_PASS(DataDependencyViewer, "view-ddg",
                "View data dependency graph of function", false, false)

char ControlDependencyViewer::ID = 0;
INITIALIZE_PASS(ControlDependencyViewer, "view-cdg",
                "View control dependency graph of function", false, false)

char ProgramDependencyViewer::ID = 0;
INITIALIZE_PASS(ProgramDependencyViewer, "view-pdg",
                "View program dependency graph of function", false, false)


namespace cot
{
namespace {
struct DataDependencyPrinter
    : public DOTGraphTraitsPrinter<DataDependencyGraph, false>
{
  static char ID;
  DataDependencyPrinter()
      : DOTGraphTraitsPrinter<DataDependencyGraph, false>("ddgragh", ID) {}
};


struct ControlDependencyPrinter
    : public DOTGraphTraitsPrinter<ControlDependencyGraph, false>
{
  static char ID;
  ControlDependencyPrinter()
      : DOTGraphTraitsPrinter<ControlDependencyGraph, false>("cdgragh", ID) {}
};


struct ProgramDependencyPrinter
    : public DOTGraphTraitsPrinter<ProgramDependencyGraph, false>
{
  static char ID;
  ProgramDependencyPrinter()
      : DOTGraphTraitsPrinter<ProgramDependencyGraph, false>("pdgragh", ID) {}
};
}
}


char DataDependencyPrinter::ID = 0;
INITIALIZE_PASS(DataDependencyPrinter, "dot-ddg",
                "Print data dependency graph of function to 'dot' file",
                false, false)

char ControlDependencyPrinter::ID = 0;
INITIALIZE_PASS(ControlDependencyPrinter, "dot-cdg",
                "Print control dependency graph of function to 'dot' file",
                false, false)

char ProgramDependencyPrinter::ID = 0;
INITIALIZE_PASS(ProgramDependencyPrinter, "dot-pdg",
                "Print program dependency graph of function to 'dot' file",
                false, false)
