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

#include "llvm/BasicBlock.h"
#include "llvm/Assembly/Writer.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/GraphTraits.h"
#include "llvm/Support/raw_ostream.h"


#include <iterator>
#include <map>
#include <set>

namespace cot
{

  enum DependencyType
  {
    CONTROL,
    DATA
  };

  template <class NodeT> class DependencyLinkIterator;

  template <class NodeT = llvm::BasicBlock>
  class DependencyNode
  {
  public:
    typedef std::pair<DependencyNode<NodeT> *, DependencyType> DependencyLink;
    typedef std::vector<DependencyLink> DependencyLinkList;
    typedef DependencyLinkIterator<NodeT> iterator;
    typedef DependencyLinkIterator<NodeT> const_iterator;

    iterator begin()
    {
      return DependencyLinkIterator<NodeT>(mDependencies.begin());
    }

    iterator end()
    {
      return DependencyLinkIterator<NodeT>(mDependencies.end());
    }

    const_iterator begin() const
    {
      return DependencyLinkIterator<NodeT>(mDependencies.begin());
    }

    const_iterator end() const
    {
      return DependencyLinkIterator<NodeT>(mDependencies.end());
    }

    DependencyNode(const NodeT* pData) :
    mpData(pData) { }

    void addDependencyTo(DependencyNode<NodeT>* pNode, DependencyType type)
    {
      DependencyLink link = DependencyLink(pNode, type);
      mDependencies.push_back(link);
    }

    const NodeT *getData() const { return mpData; }

  private:
    const NodeT* mpData;
    DependencyLinkList mDependencies;
  };

  typedef DependencyNode<llvm::BasicBlock> DepGraphNode;

  template <class NodeT = llvm::BasicBlock>
  class DependencyLinkIterator
      : public std::iterator<std::input_iterator_tag, NodeT>
  {
    typedef typename DependencyNode<NodeT>::DependencyLinkList::const_iterator
                     InnerIterator;
  public:
    DependencyLinkIterator() : itr() {}

    DependencyLinkIterator(const InnerIterator &r)
        : itr(r) {}

    DependencyLinkIterator<NodeT> &operator++()
    {
      ++itr;
      return *this;
    }

    DependencyLinkIterator<NodeT> operator++(int)
    {
      DependencyLinkIterator<NodeT> old = *this;
      ++itr;
      return old;
    }


    DependencyNode<NodeT> *operator->()
    {
      typename DependencyNode<NodeT>::DependencyLink L = *itr;
      return L.first;
    }

    DependencyNode<NodeT> *operator*()
    {
      typename DependencyNode<NodeT>::DependencyLink L = *itr;
      return L.first;
    }

    bool operator!=(const DependencyLinkIterator &r) const
    {
      return itr != r.itr;
    }

    bool operator==(const DependencyLinkIterator &r) const
    {
      return !(operator!=(r));
    }

    DependencyType getDependencyType() const
    {
      typename DependencyNode<NodeT>::DependencyLink L = *itr;
      return L.second;
    }

  private:
    InnerIterator itr;
  };

  /////////////////////////////////////////////////////////////////////////////

  template <class NodeT = llvm::BasicBlock>
  class DependencyGraph
  {
  public:
    typedef typename std::vector<DependencyNode<NodeT>* >::iterator nodes_iterator;
    typedef typename std::vector<DependencyNode<NodeT>* >::const_iterator const_nodes_iterator;

    DependencyNode<NodeT>* getRootNode() const { return RootNode; }

    DependencyNode<NodeT>* getNodeByData(const NodeT* pData)
    {
      typename DataToNodeMap::iterator it = mDataToNode.find(pData);
      if (it == mDataToNode.end())
      {
        it = mDataToNode.insert(it, typename DataToNodeMap::value_type(pData,
                new DependencyNode<NodeT > (pData)));
        mNodes.push_back(it->second);
        if (!RootNode)
        {
          RootNode = it->second;
        }
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
        PrintDependencyTree(OS, this);
    }

  private:
    typedef std::vector<DependencyNode<NodeT>* > NodeSet;
    typedef std::map<const NodeT*, DependencyNode<NodeT>*> DataToNodeMap;
    DependencyNode<NodeT> *RootNode;
    NodeSet mNodes;
    DataToNodeMap mDataToNode;
  };

  typedef DependencyGraph<llvm::BasicBlock> DepGraph;


  /*!
   * Overloaded operator that pretty print a DependencyNode
   */
  template<class NodeT>
  static llvm::raw_ostream &operator<<(llvm::raw_ostream &o,
                                       const DependencyNode<NodeT> *N)
  {
    const NodeT *block = N->getData();
    if (block)
      WriteAsOperand(o, block, false);
    else
      o << "<<EntryNode>>";
    o << " { ";
    typename DependencyNode<NodeT>::const_iterator I = N->begin();
    typename DependencyNode<NodeT>::const_iterator E = N->end();
    for (; I != E; ++I)
    {
      WriteAsOperand(o, (*I)->getData(), false);
      o << ":" << I.getDependencyType() << " ";
    }
    o << "}";
    return o << "\n";
  }


  /*!
   * Print function.
   * Traverse in depth-first the DependencyGraph and print nodes.
   */
  template<class NodeT>
  static void PrintDependencyTree(llvm::raw_ostream &o,
                                  const DependencyGraph<NodeT> *G)
  {
    llvm::df_iterator<DependencyNode<NodeT> *> I = df_begin(*(G->begin_children()));
    llvm::df_iterator<DependencyNode<NodeT> *> E = df_end(*(G->end_children()));
    for (; I != E; ++I)
    {
      int pathLength = I.getPathLength();
      o.indent(pathLength + 2);
      o << "[" << pathLength << "] "<< *I;
    }
  }
}


namespace llvm
{

template <> struct GraphTraits<cot::DepGraphNode*>
{
  typedef cot::DepGraphNode   NodeType;
  typedef NodeType::iterator  ChildIteratorType;

  static NodeType *getEntryNode(NodeType *N) {
    return N;
  }
  static inline ChildIteratorType child_begin(NodeType *N) {
    return N->begin();
  }
  static inline ChildIteratorType child_end(NodeType *N) {
    return N->end();
  }

  typedef df_iterator<cot::DepGraphNode*> nodes_iterator;

  static nodes_iterator nodes_begin(cot::DepGraphNode *N) {
    return df_begin(getEntryNode(N));
  }

  static nodes_iterator nodes_end(cot::DepGraphNode *N) {
    return df_end(getEntryNode(N));
  }
};


template <> struct GraphTraits<cot::DepGraph *>
    : public GraphTraits<cot::DepGraphNode*> {
  static NodeType *getEntryNode(cot::DepGraph *DT) {
    return DT->getRootNode();
  }

  static nodes_iterator nodes_begin(cot::DepGraph *N) {
    if (getEntryNode(N))
      return df_begin(getEntryNode(N));
    else
      return df_end(getEntryNode(N));
  }

  static nodes_iterator nodes_end(cot::DepGraph *N) {
    return df_end(getEntryNode(N));
  }
};

}

#endif // DEPENDENCYGRAPH_H_
