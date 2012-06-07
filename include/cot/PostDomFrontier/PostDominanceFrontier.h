/** ---*- C++ -*--- PostDominanceFrontier.h
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


#ifndef POSTDOMINANCEFRONTIER_H
#define POSTDOMINANCEFRONTIER_H

#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/DominanceFrontier.h"

namespace cot
{
/// PostDominanceFrontier Class - Concrete subclass of DominanceFrontier that is
/// used to compute the a post-dominance frontier.
///
struct PostDominanceFrontier : public llvm::DominanceFrontierBase {
  static char ID;
  PostDominanceFrontier()
    : DominanceFrontierBase(ID, true) {}

  virtual bool runOnFunction(llvm::Function &) {
    Frontiers.clear();
    llvm::PostDominatorTree &DT = getAnalysis<llvm::PostDominatorTree>();
    Roots = DT.getRoots();
    if (const llvm::DomTreeNode *Root = DT.getRootNode())
      calculate(DT, Root);
    return false;
  }

  virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const {
    AU.setPreservesAll();
    AU.addRequired<llvm::PostDominatorTree>();
  }

private:
  const DomSetType &calculate(const llvm::PostDominatorTree &DT,
                              const llvm::DomTreeNode *Node);
};
}

#endif // POSTDOMINANCEFRONTIER_H
