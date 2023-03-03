#include "redBlackTree.h"

void RedBlackTree::CreatePrintQueueByDepth(Node* root, std::queue<Node*>& printQueue, int depth) const
{
	if (depth == 0) {
		printQueue.push(root);
	}
	else if (root == nullptr) {
		CreatePrintQueueByDepth(nullptr, printQueue, depth - 1);
		CreatePrintQueueByDepth(nullptr, printQueue, depth - 1);
	}
	else {
		if (pRoot->GetLeft() == nullptr && root->GetRight() == nullptr) {
			CreatePrintQueueByDepth(nullptr, printQueue, depth - 1);
			CreatePrintQueueByDepth(nullptr, printQueue, depth - 1);
		}
		else {
			CreatePrintQueueByDepth(root->GetLeft(), printQueue, depth - 1);
			CreatePrintQueueByDepth(root->GetRight(), printQueue, depth - 1);
		}
	}
}

std::queue<Node*> RedBlackTree::CreatePrintQueue() const
{
	if (pRoot == nullptr) {
		return std::queue<Node*>();
	}

	std::queue<Node*> result;
	const int height = pRoot->Height();
	for (int depth = 0; depth <= height; depth++) {
		CreatePrintQueueByDepth(pRoot, result, depth);
	}

	return result;
}

void RedBlackTree::FixDoubleReds(Node*& node, Node*& parent, Node*& grandParent) {
	Node* uncle{ nullptr };
	bool isParentLeftChild = (parent == grandParent->GetLeft());
	bool isNodeLeftChild = (node == parent->GetLeft());
	bool areNodeAndParentSameSide = (isParentLeftChild == isNodeLeftChild);
	//Parent(Left), Uncle(Right)  OR the opposite
	uncle = grandParent->GetChild(!isParentLeftChild);
	//Parent(Left, RED), Uncle(Right, RED) OR the opposite
	if (uncle != nullptr && uncle->IsRed() == true) {
		//RECOLOR
		uncle->PaintBlack();
		parent->PaintBlack();
		grandParent->PaintRed();
		node = grandParent;
		parent = node->GetParent();
	}
	//Parent(Left, RED), Uncle(Right, BLACK), node = 'Left' of parent OR the opposite
	else if (areNodeAndParentSameSide) {
		parent->PaintBlack();
		grandParent->PaintRed();
		Node* sibling = parent->GetChild(!isNodeLeftChild);
		Node* greatGrandParent = grandParent->GetParent();
		if (grandParent == pRoot) {
			std::unique_lock<std::mutex> gpLock(grandParent->rotateMtx);
			std::unique_lock<std::mutex> pLock(parent->rotateMtx);
			if (sibling != nullptr) {
				sibling->rotateMtx.lock();
			}
			pRoot = parent;
			SingleRotate(parent, grandParent, sibling, !isParentLeftChild);
			if (sibling != nullptr) {
				sibling->rotateMtx.unlock();
			}
			pLock.unlock();
			gpLock.unlock();
		}
		else {
			std::unique_lock<std::mutex> ggp_lk(greatGrandParent->rotateMtx);
			std::unique_lock<std::mutex> gp_lk(grandParent->rotateMtx);
			std::unique_lock<std::mutex> p_lk(parent->rotateMtx);
			if (sibling != nullptr) {
				sibling->rotateMtx.lock();
			}
			if (grandParent == greatGrandParent->GetLeft()) {
				ConnectLeftChild(greatGrandParent, parent);
			}
			else {
				ConnectRightChild(greatGrandParent, parent);
			}
			SingleRotate(parent, grandParent, sibling, !isParentLeftChild);
			if (sibling != nullptr) {
				sibling->rotateMtx.unlock();
			}
			p_lk.unlock();
			gp_lk.unlock();
			ggp_lk.unlock();
		}
	}
	//parent(Left), uncle(Right, BLACK), node = 'Right' of parent OR the opposite
	else {
		node->PaintBlack();
		grandParent->PaintRed();
		Node* left = node->GetLeft();
		Node* right = node->GetRight();
		Node* greatGrandParent = grandParent->GetParent();
		if (grandParent == pRoot) {
			std::unique_lock<std::mutex> gpLock(grandParent->rotateMtx);
			std::unique_lock<std::mutex> pLock(parent->rotateMtx);
			std::unique_lock<std::mutex> nodeLock(node->rotateMtx);
			if (left != nullptr) {
				left->rotateMtx.lock();
			}
			if (right != nullptr) {
				right->rotateMtx.lock();
			}
			pRoot = node;
			DoubleRotate(node, parent, grandParent, left, right, !isNodeLeftChild);
			if (right != nullptr) {
				right->rotateMtx.unlock();
			}
			if (left != nullptr) {
				left->rotateMtx.unlock();
			}
			nodeLock.unlock();
			pLock.unlock();
			gpLock.unlock();
		}
		else {
			std::unique_lock<std::mutex> ggpLock(greatGrandParent->rotateMtx);
			std::unique_lock<std::mutex> gpLock(grandParent->rotateMtx);
			std::unique_lock<std::mutex> pLock(parent->rotateMtx);
			std::unique_lock<std::mutex> nodeLock(node->rotateMtx);
			if (left != nullptr) {
				left->rotateMtx.lock();
			}
			if (right != nullptr) {
				right->rotateMtx.lock();
			}

			if (grandParent == greatGrandParent->GetLeft()) {
				ConnectLeftChild(greatGrandParent, node);
			}
			else {
				ConnectRightChild(greatGrandParent, node);
			}
			DoubleRotate(node, parent, grandParent, left, right, !isNodeLeftChild);

			if (right != nullptr) {
				right->rotateMtx.unlock();
			}
			if (left != nullptr) {
				left->rotateMtx.unlock();
			}
			nodeLock.unlock();
			pLock.unlock();
			gpLock.unlock();
			ggpLock.unlock();
		}
	}
}

void RedBlackTree::TestDoubleReds(Node* node, Node* parent) {
	while (node != pRoot
		&& node->IsRed() == true
		&& node->GetParent()->IsRed() == true) {
		parent = node->GetParent();
		Node* grandParent = parent->GetParent();
		if (grandParent == nullptr) {
			pRoot->PaintBlack();
			return;
		}
		FixDoubleReds(node, parent, grandParent);
	}
}

bool RedBlackTree::Contains(int value) {
	Node* node{ pRoot };
	if (node == nullptr) {
		return false;
	}
	std::unique_lock<std::mutex> searchLock(node->searchMtx);
	bool found{ false };
	while (node != nullptr && found == false) {
		if (value < node->GetValue()) {
			node = node->GetLeft();
		}
		else if (value > node->GetValue()) {
			node = node->GetRight();
		}
		else {
			found = true;
		}
		if (found == false) {
			if (node != nullptr) {
				std::unique_lock<std::mutex> nextSearchLock(node->searchMtx);//hand-over-hand locking
				searchLock.unlock();
				searchLock = std::move(nextSearchLock);
			}
			else {
				searchLock.unlock();
			}
		}
		else {
			searchLock.unlock();
		}
	}
	return found;
}

void RedBlackTree::ConnectParentChild(Node* parent, Node* child, bool isChildLeftSide) {
	if (parent == nullptr) {
		return;
	}
	if (child != nullptr) {
		child->SetParent(parent);
	}
	if (isChildLeftSide == true) {
		parent->SetLeft(child);
	}
	else {
		parent->SetRight(child);
	}
}

//child of the new pivot becomes the new child of the old pivot

void RedBlackTree::SingleRotate(Node* newPivot, Node* oldPivot, Node* childOfOldPivot, bool isRotateLeft) {

	if (isRotateLeft == true) {
		ConnectLeftChild(newPivot, oldPivot);
		ConnectRightChild(oldPivot, childOfOldPivot);
	}
	else {
		ConnectRightChild(newPivot, oldPivot);
		ConnectLeftChild(oldPivot, childOfOldPivot);
	}
}

void RedBlackTree::DoubleRotate(Node* newPivot, Node* oldPivot1, Node* oldPivot2, Node* left, Node* right, bool isLeftRightRotation) {
	if (isLeftRightRotation == true) {
		ConnectLeftChild(newPivot, oldPivot1);
		ConnectRightChild(newPivot, oldPivot2);
		ConnectRightChild(oldPivot1, left);
		ConnectLeftChild(oldPivot2, right);
	}
	else {
		ConnectRightChild(newPivot, oldPivot1);
		ConnectLeftChild(newPivot, oldPivot2);
		ConnectLeftChild(oldPivot1, right);
		ConnectRightChild(oldPivot2, left);
	}
}

void RedBlackTree::ConnectLeftChild(Node* parent, Node* child) {
	if (parent == nullptr) {
		return;//to->throw
	}
	if (child != nullptr) {
		child->SetParent(parent);
	}
	parent->SetLeft(child);
}

void RedBlackTree::ConnectRightChild(Node* parent, Node* child) {
	if (parent == nullptr) {
		return;//to->throw
	}
	if (child != nullptr) {
		child->SetParent(parent);
	}
	parent->SetRight(child);
}

bool RedBlackTree::Add(int value) {
	bool found{ false };
	Node* lockedNode{};
	std::unique_lock<std::mutex> rootLock(rootAddMtx);
	if (pRoot == nullptr) {
		pRoot = new Node(value);
		pRoot->PaintBlack();
		rootLock.unlock();

		return true;
	}
	lockedNode = pRoot;

	std::unique_lock<std::mutex> addLock(pRoot->addMtx);

	Node* node{ pRoot };
	Node* parent{ nullptr };
	while (node != nullptr && found == false) {
		if (value < node->GetValue()) {
			parent = node;
			node = node->GetLeft();
		}
		else if (value > node->GetValue()) {
			parent = node;
			node = node->GetRight();
		}
		else {
			found = true;
		}
		if (node != nullptr && found == false) {
			if (node->IsRed() == false && parent->IsRed() == false && parent != lockedNode) {
				std::unique_lock<std::mutex> newAddLock(parent->addMtx);
				if (parent == pRoot) {
					rootLock.unlock();
				}
				addLock.unlock();
				addLock = std::move(newAddLock);
				lockedNode = parent;
			}
		}
	}
	if (found == false) {
		node = new Node(value);
		node->rotateMtx.lock();
		bool isChildLeftSide = value < parent->GetValue();
		ConnectParentChild(parent, node, isChildLeftSide);

		node->rotateMtx.unlock();
		TestDoubleReds(node, parent);
	}
	pRoot->PaintBlack();
	addLock.unlock();

	return !found;
}

void RedBlackTree::PrintTree() const {

	if (pRoot != nullptr) {
		std::queue<Node*> printQueue = CreatePrintQueue();

		int nodeRowCount = 0;
		int nodeRowTarget = 1;
		int coutWidth = 4 << pRoot->Height();
		int offset{};
		while (printQueue.empty() == false) {
			if (nodeRowCount == 0) {
				std::cout.width(coutWidth / 2);
			}
			else {
				std::cout.width(coutWidth - 1 + offset);
			}

			if (printQueue.front() != nullptr) {
				Node* node = printQueue.front();
				std::cout <<
					((node->GetValue() == std::numeric_limits<int>::min()) ? "X" : std::to_string(node->GetValue()))
					<< "(";
				if (node->IsRed() == false) {
					std::cout << "B)";
				}
				else {
					std::cout << "R)";
				}
				offset = -3;
			}
			else {
				std::cout << "X";
				offset = 0;
			}

			// check if we are done with the current row
			if (++nodeRowCount == nodeRowTarget) {
				std::cout << std::endl;
				nodeRowTarget *= 2;
				coutWidth /= 2;
				nodeRowCount = 0;
			}
			else {
				std::cout << ' ';
			}
			printQueue.pop();
		}
	}
	else {
		std::cout << "Tree is Empty" << std::endl;
	}
	std::cout << std::endl;
}
