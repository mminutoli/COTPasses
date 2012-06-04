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
#include "llvm/Analysis/DOTGraphTraitsPass.h"


using namespace llvm;
using namespace cot;


namespace llvm {

template <>
struct DOTGraphTraits<cot::DepGraphNode *> : public DefaultDOTGraphTraits {
  DOTGraphTraits (bool isSimple = false)
      : DefaultDOTGraphTraits(isSimple) {}
};

template <>
struct DOTGraphTraits<cot::DepGraph *>
    : public DOTGraphTraits<cot::DepGraphNode *>
{
  DOTGraphTraits (bool isSimple = false)
      : DOTGraphTraits<cot::DepGraphNode *>(isSimple) {}
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
}

}

char DataDependencyViewer::ID = 0;
INITIALIZE_PASS(DataDependencyViewer, "view-ddg",
                "View data dependency graph of function", false, false)
