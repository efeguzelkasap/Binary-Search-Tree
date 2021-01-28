#include <iostream>
#include <cassert>

template <class Key, class Item>
class BST
{

public:
	//using Key = Key;
	//using Item = Item;

	BST() = default;
	BST(const BST&);
	BST(BST&&);
	~BST();

	void insert(Key, Item);
	void displayEntries();
	void remove(int);
	static int getMin();
	Item* lookup(Key);

	BST& operator=(const BST &);
	BST& operator=(BST&&);
	friend std::ostream& operator <<(std::ostream &, const BST &);
private:
	struct Node
	{
		Key key;
		Item item;
		Node* leftChild;
		Node* rightChild;

		Node(Key key, Item item) :
			key(key),
			item(item),
			leftChild(nullptr),
			rightChild(nullptr) {}

	};

	Node* root = nullptr;
	static bool isLeaf(Node*);
	static Item* lookupRec(Key, Node*);
	static void insertRec(Key, Item, Node*&);
	static void displayEntriesWorker(Node*);
	static void removeRec(Node*&, int);
	static void deepDelete(Node*&);
	static Node* deepCopy(Node*);
	static int getMinPrivate(Node*);
	static void rotateRight(Node*&);
	static void leftRotation(Node*&);

};



template<class Key, class Item>
BST<Key, Item>::~BST()
{

	deepDelete(root);

}

template<class Key, class Item>
BST<Key, Item>::BST(const BST& original)
{

	this->root = deepCopy(original.root);

}
template<class Key, class Item>
BST<Key, Item>::BST(BST&& currentNode)
{

	this->root = currentNode.root;
	currentNode.root = nullptr;

}

template<class Key, class Item>
typename BST<Key, Item>::Node* BST<Key, Item>::deepCopy(Node* currentNode)
{

	if (currentNode == nullptr)
	{
		return nullptr;
	}

	Node* newTree = new Node(currentNode->key, currentNode->item);

	newTree->leftChild = deepCopy(currentNode->leftChild);
	newTree->rightChild = deepCopy(currentNode->rightChild);

	return newTree;
}

template<class Key, class Item>
void BST<Key, Item>::insert(Key key, Item item)
{
	insertRec(key, item, root);
}


// Post order traversal to delete each node.
template<class Key, class Item>
void BST<Key, Item>::deepDelete(Node*& currentNode)
{

	if (currentNode == nullptr)
		return;

	deepDelete(currentNode->leftChild);
	deepDelete(currentNode->rightChild);
	std::cout << currentNode->key << " Has been delete." << std::endl;
	delete currentNode;

}

template<class Key, class Item>
void BST<Key, Item>::rotateRight(Node*& localRoot)
{

	Node* b = localRoot;
	assert(!isLeaf(b));
	Node* a = b->leftChild;
	assert(!isLeaf(a));
	Node* gamma = b->rightChild;

	b = a;
	a->rightChild = gamma;
	gamma = a;
	b = localRoot;
}

template<class Key, class Item>
void BST<Key, Item>::leftRotation(Node*& localRoot)
{

	Node* b = localRoot;
	assert(!isLeaf(b));
	Node* a = b->rightChild;
	assert(!isLeaf(a));
	Node* gamma = b->leftChild;

	b = a;
	a->leftChild = gamma;
	gamma = a;
	b = localRoot;

}

template<class Key, class Item>
void BST<Key, Item>::remove(int key)
{
	removeRec(root, key);
}

template<class Key, class Item>
int BST<Key, Item>::getMin()
{

	getMinPrivate(root);

}

template<class Key, class Item>
void BST<Key, Item>::removeRec(Node*& currentNode, int key)
{

	if (currentNode == nullptr)
	{
		// Node is not in tree do nothing.
		std::cout << "Value does not exist" << std::endl;
		return;
	}

	if (key < currentNode->key)
	{
		removeRec(currentNode->leftChild, key);
	}
	else if (key > currentNode->key)
	{
		removeRec(currentNode->rightChild, key);
	}
	else
	{
		// Case 1: No children.
		if (currentNode->leftChild == nullptr && currentNode->rightChild == nullptr)
		{
			delete currentNode;
			currentNode = nullptr;
		}
		else if (currentNode->rightChild == nullptr)
		{
			// Case 2: Left child only
			currentNode->key = currentNode->leftChild->key;
			currentNode->item = currentNode->leftChild->item;
			delete currentNode->leftChild;
			currentNode->leftChild = nullptr;
		}
		else if (currentNode->leftChild == nullptr)
		{
			// Case 3: right child only
			currentNode->key = currentNode->rightChild->key;
			currentNode->item = currentNode->rightChild->item;
			delete currentNode->rightChild;
			currentNode->rightChild = nullptr;
		}
		else
		{
			// Case 4: Both right and left.
			int newKey = getMinPrivate(currentNode);
			Item* newItem = lookupRec(newKey, currentNode->rightChild);
			currentNode->key = newKey;
			currentNode->item = *newItem;
			std::cout << newKey << std::endl;
			removeRec(currentNode->rightChild, newKey);
		}
	}
}

template<class Key, class Item>
int BST<Key, Item>::getMinPrivate(Node* currentNode)
{


	if (currentNode->rightChild != nullptr)
	{
		getMinPrivate(currentNode->rightChild);
	}
	else if (currentNode->leftChild != nullptr)
	{
		getMinPrivate(currentNode->leftChild);
	}
	else
	{
		return currentNode->key;
	}

}

template<class Key, class Item>
void BST<Key, Item>::displayEntries()
{

	displayEntriesWorker(root);

}

// In-order traversal to print nodes.
template<class Key, class Item>
void BST<Key, Item>::displayEntriesWorker(Node* currentNode)
{

	if (currentNode != nullptr)
	{
		if (currentNode->leftChild != nullptr)
		{
			displayEntriesWorker(currentNode->leftChild);
		}
		std::cout << currentNode->key << "  " << currentNode->item << std::endl;
		if (currentNode->rightChild != nullptr)
		{
			displayEntriesWorker(currentNode->rightChild);
		}
	}
	else
	{
		std::cout << "The tree is empty." << std::endl;
	}

}

template<class Key, class Item>
void BST<Key, Item>::insertRec(Key key, Item item, Node*& currentNode)
{

	if (currentNode == nullptr)
	{
		currentNode = new Node(key, item);
	}
	else if (key < currentNode->key)
	{
		if (currentNode->leftChild != nullptr)
		{
			insertRec(key, item, currentNode->leftChild);
		}
		else
		{
			currentNode->leftChild = new Node(key, item);
		}
	}
	else if (key > currentNode->key)
	{
		if (currentNode->rightChild != nullptr)
		{
			insertRec(key, item, currentNode->rightChild);
		}
		else
		{
			currentNode->rightChild = new Node(key, item);
		}
	}
	else
	{
		currentNode->item = item;
		currentNode->key = key;

		std::cout << "The Key: " << key << " has already been added to the tree. And has been replaced." << std::endl;
	}

}

template<class Key, class Item>
Item* BST<Key, Item>::lookup(Key soughtKey)
{
	return lookupRec(soughtKey, root);
}


template<class Key, class Item>
Item* BST<Key, Item>::lookupRec(Key soughtKey, Node* currentNode)
{

	if (currentNode != nullptr)
	{

		if (currentNode->key == soughtKey)
		{
			return &currentNode->item;
		}
		else if (currentNode->key < soughtKey)
		{
			lookupRec(soughtKey, currentNode->rightChild);
		}
		else
		{
			lookupRec(soughtKey, currentNode->leftChild);
		}

	}
}

// Leaf nodes do not contain any data and have no children.
template<class Key, class Item>
bool BST<Key, Item>::isLeaf(Node* n)
{
	if (n == nullptr)
		return true;
}

template<class Key, class Item>
std::ostream& operator<<(std::ostream& out, const BST<Key, Item>& b)
{
	if (b.root != nullptr)
	{
		out << b.root->key << " : " << b.root->item << std::endl;
	}
	else
	{
		std::cout << "The tree is empty." << std::endl;
	}

	return out;
}

template<class Key, class Item>
BST<Key, Item>& BST<Key, Item>::operator=(const BST& currentNode)
{

	if (this == &currentNode)
	{
		return *this;
	}

	deepDelete(this->root);
	this->root = deepCopy(currentNode.root);
	return *this;
}

template<class Key, class Item>
BST<Key, Item>& BST<Key, Item>::operator=(BST&& currentNode)
{

	if (this == &currentNode)
	{
		return *this;
	}

	this->root = currentNode.root;
	currentNode.root = nullptr;
	return *this;
}

