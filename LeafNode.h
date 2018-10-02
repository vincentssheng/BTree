#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode:public BTreeNode
{
  int *values;
public:
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
    BTreeNode *right);
  int getMinimum() const;
  LeafNode* insert(int value); // returns pointer to new Leaf if splits
  // else NULL
  void notFullInsert(int value);
  void leafSplit(int index, LeafNode* newLeaf);
  void splitInsert(int value, LeafNode* newLeaf);
  void proSort(int j);
  void print(Queue <BTreeNode*> &queue);
}; //LeafNode class

#endif
