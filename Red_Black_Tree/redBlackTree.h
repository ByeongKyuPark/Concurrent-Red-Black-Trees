#pragma once
#include "node.h"
#include <iostream>//std::cerr
#include <queue>//print queue
#include <string>//to_string

class RedBlackTree {
	int size;
	Node* pRoot;
	std::mutex rootAddMtx;

	void FixDoubleReds(Node*& node, Node*& parent, Node*& grandParent);
	void TestDoubleReds(Node* node, Node* parent);

	//child of the new pivot becomes the new child of the old pivot
	void SingleRotate(Node* newPivot, Node* oldPivot, Node* childOfOldPivot, bool isRotateLeft);
	void DoubleRotate(Node* newPivot, Node* oldPivot1, Node* oldPivot2, Node* left, Node* right, bool isLeftRightRotation);

	void ConnectLeftChild(Node* parent, Node* child);
	void ConnectRightChild(Node* parent, Node* child);
	void ConnectParentChild(Node* parent, Node* child, bool isChildLeftSide = true);

	void CreatePrintQueueByDepth(Node* root, std::queue<Node*>& printQueue, int depth) const;
	std::queue<Node*> CreatePrintQueue() const;

public:
	RedBlackTree() :size{}, pRoot{ nullptr } {}

	bool Contains(int value);//TODO::https://stanford-ppl.github.io/website/papers/ppopp207-bronson.pdf
	bool Add(int value);
	void PrintTree() const;
};