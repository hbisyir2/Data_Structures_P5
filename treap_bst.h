// define your TreapBST here
#ifndef TREAP_BST_H
#define TREAP_BST_H

#include "abstract_bst.h"
#include <random>

using std::mt19937;

template <typename KeyType, typename ValueType>
class TreapBST : public AbstractBST<KeyType, ValueType>
{
public:

	// constructor
	TreapBST();

	// destructor
	~TreapBST();

	// copy constructor
	TreapBST(const TreapBST & oldTreap);

	// copy assignment
	TreapBST& operator=(TreapBST oldTreap);

	// search
	bool empty();

	// search
	ValueType search(const KeyType& key, bool& found);

	// insert
	void insert(const KeyType& key, const ValueType& value);

	// remove
	void remove(const KeyType& key);

private:

	// for random number
	mt19937 rndgen;

	// struct for the treap's node
	template<typename T, typename U>
	struct TreapNode {
		T KeyItem;
		U ValueItem;
		mt19937::result_type priority;
		TreapNode<T, U> * left;
		TreapNode<T, U> * right;
		TreapNode<T, U> * parent;

		TreapNode() : left(nullptr), right(nullptr), parent(nullptr), priority(0) {};
		TreapNode(const T& k, const U& v, const std::mt19937::result_type& r) : KeyItem(k), ValueItem(v), priority(r), left(nullptr), right(nullptr), parent(nullptr) {};
	};

	// starting point of the treap
	TreapNode<KeyType, ValueType> * top;

	// size of the treap
	std::size_t size;

	// Rotate given a parent, with its left child
	void RotateLeftChild(TreapNode<KeyType, ValueType> * & switch1)
	{
		TreapNode<KeyType, ValueType> * switch2 = switch1->left;
		TreapNode<KeyType, ValueType> *switchParent = switch1->parent;
		switch1->left = switch2->right;
		if (switch2->right != nullptr) switch2->right->parent = switch1;
		switch1->parent = switch2;
		switch2->right = switch1;
		switch2->parent = switchParent;
		if (switchParent != nullptr) {
			if (switchParent->KeyItem < switch1->KeyItem) {
				switchParent->right = switch2;
			}
			else {
				switchParent->left = switch2;
			}
		}
		else {
			top = switch2;
		}
		switch1 = switch2;
	}

	// Rotate given a parent, with its right child
	void RotateRightChild(TreapNode<KeyType, ValueType> * & switch1)
	{
		TreapNode<KeyType, ValueType> * switch2 = switch1->right;
		TreapNode<KeyType, ValueType> *switchParent = switch1->parent;
		switch1->right = switch2->left;
		if (switch2->left != nullptr) switch2->left->parent = switch1;
		switch1->parent = switch2;
		switch2->left = switch1;
		switch2->parent = switchParent;
		switch1 = switch2;
		if (switchParent != nullptr) {
			if (switchParent->KeyItem < switch1->KeyItem) {
				switchParent->right = switch2;
			}
			else {
				switchParent->left = switch2;
			}
		}
		else {
			top = switch2;
		}
		switch1 = switch2;
	}

	// Function to destruct a tree recursively
	void destruct(TreapNode<KeyType, ValueType> * & toDestruct) 
	{
		if (toDestruct != nullptr) {
			destruct(toDestruct->left);
			destruct(toDestruct->right);
			delete toDestruct;
		}
		toDestruct = nullptr; 
	}

	// Function to copy a tree recursively
	void copyTree(TreapNode<KeyType, ValueType> * & thisNode, TreapNode<KeyType, ValueType> * origNode)
	{
		if (origNode == nullptr) {
			thisNode = nullptr;
		}
		else {
			thisNode = new TreapNode<KeyType, ValueType>(origNode->KeyItem, origNode->ValueItem, origNode->priority);
			if (origNode->left != nullptr && thisNode->left == nullptr) thisNode->left = new TreapNode<KeyType, ValueType>(origNode->left->KeyItem, origNode->left->ValueItem, origNode->left->priority);
			if (origNode->right != nullptr && thisNode->right == nullptr) thisNode->right = new TreapNode<KeyType, ValueType>(origNode->right->KeyItem, origNode->right->ValueItem, origNode->right->priority);
			if (origNode->parent != nullptr && thisNode->parent == nullptr) thisNode->parent = new TreapNode<KeyType, ValueType>(origNode->parent->KeyItem, origNode->parent->ValueItem, origNode->parent->priority);
			copyTree(thisNode->left, origNode->left);
			copyTree(thisNode->right, origNode->right);
		}
	}

	// swap for copy assignment operator
	void swap(TreapBST<KeyType, ValueType> &x, TreapBST<KeyType, ValueType> &y)
	{
		std::swap(x.top, y.top);
		std::swap(x.size, y.size);
	}
};
#include "treap_bst.txx"

#endif // TREAP_BST_H