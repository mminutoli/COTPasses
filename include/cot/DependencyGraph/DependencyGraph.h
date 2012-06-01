/** ---*- C++ -*--- DependencyGraph.cpp
 *
 * Copyright (C) 2012 Massimo Tristano <massimo.tristano@gmail.com>
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

#ifndef DEPENDENCYGRAPH_H_
#define DEPENDENCYGRAPH_H_

#include "llvm/ADT/GraphTraits.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
#include <set>

namespace cot
{

  enum DependencyType
  {
    CONTROL,
    DATA
  };

  template <class NodeT = llvm::BasicBlock>
  class DependencyNode
  {
  public:
    typedef typename std::vector<std::pair<DependencyNode<NodeT> *, DependencyType> >::iterator iterator;
    typedef typename std::vector<std::pair<DependencyNode<NodeT> *, DependencyType> >::const_iterator const_iterator;

    iterator begin() { return mpData->begin(); }
    iterator end() { return mpData->end(); }
    const_iterator begin() const { return mpData->begin(); }
    const_iterator end() const { return mpData->end(); }

    DependencyNode(const NodeT* pData) :
    mpData(pData) { }

    void addDependencyTo(DependencyNode<NodeT>* pNode, DependencyType type)
    {
      DependencyLink link = DependencyLink(pNode, type);
      mDependencies.push_back(link);
    }

    const NodeT *getData() { return mpData; }

  private:
    typedef std::pair<DependencyNode<NodeT> *, DependencyType> DependencyLink;
    typedef std::vector<DependencyLink> DependencyLinkList;

    const NodeT* mpData;
    DependencyLinkList mDependencies;
  };

  /////////////////////////////////////////////////////////////////////////////

  template <class NodeT = llvm::BasicBlock>
  class DependencyGraph
  {
  public:
    typedef typename std::set<DependencyNode<NodeT>* >::iterator nodes_iterator;
    typedef typename std::set<DependencyNode<NodeT>* >::const_iterator const_nodes_iterator;

    DependencyNode<NodeT>* getNodeByData(const NodeT* pData)
    {
      typename DataToNodeMap::iterator it = mDataToNode.find(pData);
      if (it == mDataToNode.end())
      {
        it = mDataToNode.insert(it, typename DataToNodeMap::value_type(pData,
                new DependencyNode<NodeT > (pData)));
        mNodes.insert(it->second);
      }
      return it->second;
    }

    const DependencyNode<NodeT>* getNodeByData(const NodeT* pData) const
    {
      typename DataToNodeMap::const_iterator it = mDataToNode.find(pData);
      if (it == mDataToNode.end())
      {
        //        errs() << "Node with data " << pData <<
        //                " not found within dependency graph " << this << ".";
        return 0;
      }
      return it->second;
    }

    void addDependency(const NodeT* pDependent, const NodeT* pDepency,
            DependencyType type)
    {
      DependencyNode<NodeT>* pFrom = getNodeByData(pDependent);
      DependencyNode<NodeT>* pTo = getNodeByData(pDepency);
      pFrom->addDependencyTo(pTo, type);
    }

    nodes_iterator begin_children()
    {
      return nodes_iterator(mNodes.begin());
    }

    nodes_iterator end_children()
    {
      return nodes_iterator(mNodes.end());
    }

    const_nodes_iterator begin_children() const
    {
      return const_nodes_iterator(mNodes.begin());
    }

    const_nodes_iterator end_children() const
    {
      return const_nodes_iterator(mNodes.end());
    }

    void print(llvm::raw_ostream &OS, const char *PN) const
    {
      OS << "=============================--------------------------------\n";
      OS << PN << ": \n";
      const_nodes_iterator firstNode = begin_children();
      if (firstNode != end_children())
        PrintDependencyTree(OS, getNodeByData((*firstNode)->getData()));
    }

  private:
    typedef std::set<DependencyNode<NodeT>* > NodeSet;
    typedef std::map<const NodeT*, DependencyNode<NodeT>*> DataToNodeMap;
    NodeSet mNodes;
    DataToNodeMap mDataToNode;
  };


  /*!
   * Overloaded operator that pretty print a DependencyNode
   */
  template<class NodeT>
  static llvm::raw_ostream &operator<<(llvm::raw_ostream &o,
                                       const DependencyNode<NodeT> *N)
  {
    o << " <<node>>";
    return o << "\n";
  }


  /*!
   * Print function.
   * Traverse in depth-first the DependencyGraph and print nodes.
   */
  template<class NodeT>
  static void PrintDependencyTree(llvm::raw_ostream &o,
                                  const DependencyNode<NodeT> *N)
  {
    o << N;
  }
}

#endif // DEPENDENCYGRAPH_H_
