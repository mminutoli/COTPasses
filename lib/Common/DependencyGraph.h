/** ---*- C++ -*--- ProgramDependencies.cpp
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

#include <map>

namespace llvm
{

   enum DependencyType
   {
      DEP_TYPE_CONTROL,
      DEP_TYPE_DATA,
   };

   template <class T = llvm::BasicBlock>
   class DependencyNode
   {
   public:

      DependencyNode(T* pData) :
      mpData(pData) { }
      void addDependencyTo(const DependencyType<T>* pNode, DependencyType type);
   private:
      T* mpData;
      std::multimap<DependencyNode<T>*, DependencyType> mNeighbors;
   };

   template <class T = llvm::BasicBlock>
   class DependencyGraph
   {
   public:

      const DependencyNode<T>* getNodeByData(const T* pData) const
      {
         DataToNodeMap::const_iterator it = mDataToNode.find(pData);
         if (it == mDataToNode.end())
            it = mDataToNode.insert(it, std::make_pair(pData, new DependencyNode<T > (pData)));
         return it->second;
      }

      void addDependency(const T* pDependent, const T* pDepency, DependencyType type)
      {
         DependencyNode<T>* pFrom = getNodeByData(pDependent);
         DependencyNode<T>* pTo = getNodeByData(pDepency);
         pFrom->addDependencyTo(pTo, type);
      }
   private:
      typedef std::set<DependencyNode<T*> > NodeSet;
      typedef std::map<T*, DependencyNode<T>*> DataToNodeMap;
      NodeSet mNodes;
      DataToNodeMap mDataToNode;
   };
}