#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()

int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;
} // LeafNode::getMinimum()

void LeafNode::notFullInsert(int value)
{
  int i = 0;
  for(i = count; (i > 0) && (value < values[i-1]); i--)
    values[i] = values[i-1];
  values[i] = value;
  count++;
}

void LeafNode::leafSplit(int index, LeafNode* newLeaf)
{
  int i;
  for(i = index; i < leafSize; i++)
  {
    newLeaf->values[newLeaf->count++] = values[i];
    count--;
  }
}

void LeafNode::splitInsert(int value, LeafNode* newLeaf)
{
  if(!(leafSize % 2)) //If leaf size is even
  {
    if(values[leafSize/2 - 1] > value) //if value is less than the mid value
    {
      leafSplit(leafSize/2 - 1, newLeaf);
      notFullInsert(value);
    }
    else //if value is greater than the mid value
    {
      leafSplit(leafSize/2, newLeaf);
      newLeaf->notFullInsert(value);
    }
  }
  else //If leaf size is odd
  {
    if(values[leafSize/2] > value) //if value is less than mid value
    {
      leafSplit(leafSize/2, newLeaf);
      notFullInsert(value);
    }
    else //if value is greater than mid value
    {
      leafSplit(leafSize/2 + 1, newLeaf);
      newLeaf->notFullInsert(value);
    }
  }
}

void LeafNode::proSort(int j)
{
  for(int i = j; i < leafSize; i++)
    values[i - j] = values[i];
}

LeafNode* LeafNode::insert(int value)
{
  int i;
  if(count == 0) //If the leaf is empty
  {
    values[0] = value;
    count++;
  }
  else if (count < leafSize && count > 0) //If the leaf is not empty nor full
    notFullInsert(value);
  else //If the leaf is full
  {
    if(leftSibling && ((LeafNode*)leftSibling)->count < leafSize) //If left sibling is not full
    {
      ((LeafNode*)leftSibling)->notFullInsert(values[0]);
      //nToMove = count - (count + ((LeafNode*)leftSibling)->count)/2;
      proSort(1);
      count--;
      notFullInsert(value);
      /*for(i = 0; i < nToMove; i++)
        ((LeafNode*)leftSibling)->notFullInsert(values[i]);
      count -= nToMove;
      proSort(nToMove);*/
    }
    else if(rightSibling && (((LeafNode*)rightSibling)->count < leafSize)) //If right sibling is not full
    {
      for(i = leafSize - 1; (i >= 0) && (value < values[i]); i--);
      if(i == leafSize - 1)
      {
        ((LeafNode*)rightSibling)->notFullInsert(value);
      }
      else
      {
        ((LeafNode*)rightSibling)->notFullInsert(values[leafSize-1]);
        count--;
        notFullInsert(value);
      }
      /*nToMove = count - (count + ((LeafNode*)rightSibling)->count)/2;
      for(i = 1; i <= nToMove; i++)
        ((LeafNode*)rightSibling)->notFullInsert(values[leafSize - i]);
      count -= nToMove;*/
    }
    else if(!leftSibling && !rightSibling) //If no sibling
    {
      LeafNode* newLeaf = new LeafNode(leafSize, parent, this, NULL);
      rightSibling = newLeaf;
      splitInsert(value, newLeaf);
      return newLeaf;
    }
    else if(!leftSibling && rightSibling && rightSibling->getCount() == leafSize)
    {
      LeafNode* newLeaf = new LeafNode(leafSize, parent, this, rightSibling);
      rightSibling->setLeftSibling(newLeaf);
      setRightSibling(newLeaf);
      splitInsert(value, newLeaf);
      return newLeaf;
    }
    /*else if(rightSibling && ((LeafNode*)rightSibling)->count == leafSize && !leftSibling) //right full; left not full
    {
      LeafNode* newLeaf = new LeafNode(leafSize, parent, this, this->rightSibling);
      newLeaf->rightSibling = rightSibling;
      ((LeafNode*)rightSibling)->leftSibling = newLeaf;
      rightSibling = newLeaf;
      splitInsert(value, newLeaf);
      return newLeaf;
    }*/
    else if(leftSibling && ((LeafNode*)leftSibling)->count == leafSize && !rightSibling) //left full; no right
    {
      LeafNode* newLeaf = new LeafNode(leafSize, parent, this, NULL);
      setRightSibling(newLeaf);
      splitInsert(value, newLeaf);
      return newLeaf;
    }
    else if(leftSibling && rightSibling && ((LeafNode*)leftSibling)->count == leafSize && ((LeafNode*)rightSibling)->count == leafSize)
    //if both siblings are full
    {
      LeafNode* newLeaf = new LeafNode(leafSize, parent, this, rightSibling);
      ((LeafNode*)rightSibling)->leftSibling = newLeaf;
      rightSibling = newLeaf;
      splitInsert(value, newLeaf);
      return newLeaf;
    }
  }
  return NULL; // to avoid warnings for now.
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()
