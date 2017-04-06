#include "treap_bst.h"

// implement your TreapBST  here
// using BinarySearchTree as a starting point

// constructor
template <typename KeyType, typename ValueType>
TreapBST<KeyType, ValueType>::TreapBST() : top(nullptr), size(0)
{
	std::random_device rd;
	rndgen.seed(rd());
}

// destructor
template <typename KeyType, typename ValueType>
TreapBST<KeyType, ValueType>::~TreapBST()
{
	destruct(top);
}

// copy constructor
template <typename KeyType, typename ValueType>
TreapBST<KeyType, ValueType>::TreapBST(const TreapBST<KeyType, ValueType> & oldTreap) : top(nullptr), size(0)
{
	size = oldTreap.size;
	if (oldTreap.top == nullptr) {
		top = nullptr;
	}
	else {
		copyTree(this->top, oldTreap.top);
	}

}

// copy assignment operator
template <typename KeyType, typename ValueType>
TreapBST<KeyType, ValueType>& TreapBST<KeyType, ValueType>::operator=(TreapBST<KeyType, ValueType> oldTreap)
{
	swap(*this, oldTreap);
	return *this;
}

// empty
template <typename KeyType, typename ValueType>
bool TreapBST<KeyType, ValueType>::empty()
{
	if (size == 0) return true;
	return false;
}

// search
template <typename KeyType, typename ValueType>
ValueType TreapBST<KeyType, ValueType>::search(const KeyType& key, bool& found)
{
	int i = 0;

	TreapNode<KeyType, ValueType> * current = top;
	KeyType curKey;
	ValueType curVal;

	while (current != nullptr) {
		curKey = current->KeyItem;
		curVal = current->ValueItem;
		if (key == current->KeyItem) {
			found = true;
			return current->ValueItem;
		}

		else if (key < current->KeyItem) {
			current = current->left;
		}

		else if (key > current->KeyItem) {
			current = current->right;
		}
	}
	found = false;
	curVal = ValueType();
	return curVal;
}

// insert
template <typename KeyType, typename ValueType>
void TreapBST<KeyType, ValueType>::insert(const KeyType& key, const ValueType& value)
{
	int priority;
	priority = rndgen();
	
	if (size == 0) {
		TreapNode<KeyType, ValueType> * toInsert = new TreapNode<KeyType, ValueType>(key, value, priority);
		top = toInsert;
		size++;
		return;
	}

	std::string rotation;

	TreapNode<KeyType, ValueType> * current = top;
	TreapNode<KeyType, ValueType> * lastSearched = current;

	while (current != nullptr) {

		if (key == current->KeyItem) {
			throw std::logic_error("Key was found");
		}

		else if (key < current->KeyItem) {
			lastSearched = current;
			current = current->left;
		}

		else if (key > current->KeyItem) {
			lastSearched = current;
			current = current->right;
		}
	}
	TreapNode<KeyType, ValueType> * toInsert = new TreapNode<KeyType, ValueType>(key, value, priority);
	toInsert->parent = lastSearched;

	if (key < lastSearched->KeyItem) {
		lastSearched->left = toInsert;
		rotation = "left";
	}
	else {
		lastSearched->right = toInsert;
		rotation = "right";
	}

	TreapNode<KeyType, ValueType> * above = lastSearched;
	TreapNode<KeyType, ValueType> * lastParent = lastSearched->parent;
	TreapNode<KeyType, ValueType> * left = toInsert->left;
	TreapNode<KeyType, ValueType> * right = toInsert->right;
	while (above->priority < toInsert->priority) {
		if (rotation == "left") {
			RotateLeftChild(above);
		}
		if (rotation == "right") {
			RotateRightChild(above);
		}
		above = toInsert->parent;
		if (above == nullptr) break;
		if (above->KeyItem > toInsert->KeyItem) rotation = "left";
		else if (above->KeyItem < toInsert->KeyItem) rotation = "right";
		left = toInsert->left;
		right = toInsert->right;
		lastParent = lastSearched->parent;
	}
	size++;
}

// remove
template <typename KeyType, typename ValueType>
void TreapBST<KeyType, ValueType>::remove(const KeyType& key)
{
	if (size == 0) throw std::logic_error("Treap is empty");
	KeyType currKey;
	TreapNode<KeyType, ValueType> * current = top;

	bool found = false;
	bool stillTop = true;

	while (current != nullptr) {

		if (key == current->KeyItem) {
			found = true;
			break;
		}

		else if (key < current->KeyItem) {
			current = current->left;
			stillTop = false;
		}

		else if (key > current->KeyItem) {
			current = current->right;
			stillTop = false;
		}
	}
	if (!found) {
		return;
	}
	TreapNode<KeyType, ValueType> * currParent = current->parent;
	if (current->left == nullptr && current->right == nullptr) {
		if (!stillTop) {
			if (currParent->KeyItem < current->KeyItem) currParent->right = nullptr;
			else if (currParent->KeyItem > current->KeyItem) currParent->left = nullptr;
		}
		else {
			top = nullptr;
		}
		delete current;
	}
	else if (current->left == nullptr && current->right != nullptr) {
		TreapNode<KeyType, ValueType> * temp = current->right;
		temp->parent = currParent;
		if (stillTop) {
			top = temp;
		}
		else {
			if (currParent->KeyItem < temp->KeyItem) currParent->right = temp;
			else if (currParent->KeyItem > temp->KeyItem) currParent->left = temp;
		}
		delete current;
	}
	else if (current->left != nullptr && current->right == nullptr) {
		TreapNode<KeyType, ValueType> * temp = current->left;
		temp->parent = currParent;
		if (stillTop) {
			top = temp;
		}
		else {
			if (currParent->KeyItem < temp->KeyItem) currParent->right = temp;
			else if (currParent->KeyItem > temp->KeyItem) currParent->left = temp;
		}
		delete current;
	}
	else {
		while (current->left != nullptr && current->right != nullptr) {
			currKey = current->KeyItem;
			if (current->left->priority < current->right->priority) {
				RotateRightChild(current);
				if (current->left != nullptr) current = current->left;
				stillTop = false;
			}
			else {
				RotateLeftChild(current);
				if (current->right != nullptr) current = current->right;
				stillTop = false;
			}
		}
		currParent = current->parent;
		if (current->left == nullptr && current->right == nullptr) {
			if (!stillTop) {
				if (currParent->KeyItem < current->KeyItem) currParent->right = nullptr;
				else if (currParent->KeyItem > current->KeyItem) currParent->left = nullptr;
			}
			else {
				top = nullptr;
			}
			delete current;
		}
		else if (current->left == nullptr && current->right != nullptr) {
			TreapNode<KeyType, ValueType> * temp = current->right;
			temp->parent = currParent;
			if (stillTop) {
				top = temp;
			}
			else {
				if (currParent->KeyItem < temp->KeyItem) currParent->right = temp;
				else if (currParent->KeyItem > temp->KeyItem) currParent->left = temp;
			}
			delete current;
		}
		else if (current->left != nullptr && current->right == nullptr) {
			TreapNode<KeyType, ValueType> * temp = current->left;
			temp->parent = currParent;
			if (stillTop) {
				top = temp;
			}
			else {
				if (currParent->KeyItem < temp->KeyItem) currParent->right = temp;
				else if (currParent->KeyItem > temp->KeyItem) currParent->left = temp;
			}
			delete current;
		}
	}
	size--;
}