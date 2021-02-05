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
	static void removeRec(Node*&, Key);
	static void deepDelete(Node*&);
	static Node* deepCopy(Node*);
	static Node* getMin(Node*);
	static void rotateRight(Node*&);
	static void leftRotation(Node*&);
	static void searchKey(Node*&, Key, Node*&);

};

template<class Key, class Item>
BST<Key, Item>::~BST()
{

	deepDelete(root);

}

//Copy constructor
template<class Key, class Item>
BST<Key, Item>::BST(const BST& original)
{

	this->root = deepCopy(original.root);

}
//Move constructor.
template<class Key, class Item>
BST<Key, Item>::BST(BST&& currentNode)
{

	this->root = currentNode.root;
	currentNode.root = nullptr;

}

// Deep copy the tree
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

// Right rotated around pivot node.
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

// Left rotate around pivot node.
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

// Remove node.
template<class Key, class Item>
void BST<Key, Item>::removeRec(Node*& currentNode, Key key)
{
	// pointer to store the parent of the current node
	Node* parent = nullptr;

	// start with the root node
	Node* curr = currentNode;

	// search key in the BST and set its parent pointer
	searchKey(curr, key, parent);

	// return if the key is not found in the tree
	if (curr == nullptr) {
		return;
	}

	// Case 1: node to be deleted has no children, i.e., it is a leaf node
	if (curr->leftChild == nullptr && curr->rightChild == nullptr)
	{
		// if the node to be deleted is not a root node, then set its
		// parent left/right child to null
		if (curr != currentNode)
		{
			if (parent->leftChild == curr) {
				parent->leftChild = nullptr;
			}
			else {
				parent->rightChild = nullptr;
			}
		}
		// if the tree has only a root node, set it to null
		else {
			currentNode = nullptr;
		}

		// deallocate the memory
		delete curr;
		curr = nullptr;
	}

	// Case 2: node to be deleted has two children
	else if (curr->leftChild && curr->rightChild)
	{
		// find its inorder successor node
		Node* successor = getMin(curr->rightChild);

		// store successor value
		Key val = successor->key;
		Item itemVal = successor->item;

		// recursively delete the successor. Note that the successor
		// will have at most one child (right child)
		removeRec(currentNode, successor->key);

		// copy value of the successor to the current node
		curr->key = val;
		curr->item = itemVal;
	}

	// Case 3: node to be deleted has only one child
	else
	{
		// choose a child node
		Node* child = (curr->leftChild) ? curr->leftChild : curr->rightChild;

		// if the node to be deleted is not a root node, set its parent
		// to its child
		if (curr != currentNode)
		{
			if (curr == parent->leftChild) {
				parent->leftChild = child;
			}
			else {
				parent->rightChild = child;
			}
		}

		// if the node to be deleted is a root node, then set the root to the child
		else {
			currentNode = child;
		}

		// deallocate the memory
		//free(curr);
		delete curr;
	}
}

template<class Key, class Item>
inline void BST<Key, Item>::searchKey(Node*& currentNode, Key key , Node*& parent)
{

	// traverse the tree and search for the key
	while (currentNode != nullptr && currentNode->key != key)
	{
		// update the parent to the current node
		parent = currentNode;

		// if the given key is less than the current node, go to the left subtree;
		// otherwise, go to the right subtree
		if (key < currentNode->key) {
			currentNode = currentNode->leftChild;
		}
		else {
			currentNode = currentNode->rightChild;
		}
	}

}

template<class Key, class Item>
typename BST<Key, Item>::Node* BST<Key, Item>::getMin(Node* currentNode)
{
	while (currentNode->leftChild != nullptr) {
		currentNode = currentNode->leftChild;
	}
	return currentNode;
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

