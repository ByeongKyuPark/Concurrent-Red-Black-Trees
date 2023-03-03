#pragma once
#include <limits>
#include <mutex>

class Node {
	friend class RedBlackTree;

	std::mutex searchMtx;
	std::mutex addMtx;
	std::mutex rotateMtx;

	//TODO::unique_ptr
	Node* pLeft;
	Node* pRight;
	Node* pParent;
	//TODO::shared_ptr
	int element;
	bool isRed;

public:
	//TODO::clone()
	Node(int val = std::numeric_limits<int>::min()) :pLeft{}, pRight{}, pParent{}, element{ val },isRed{ true }
	{}
	~Node() {
		delete pLeft;
		delete pRight;
		pParent = nullptr;
		pLeft = nullptr;
		pRight = nullptr;
	}

	int GetValue() const noexcept {
		return element;
	}

	Node* GetChild(bool isLeftChild) noexcept{
		return (isLeftChild == true) ? pLeft : pRight;
	}

	Node* GetLeft() noexcept {
		return pLeft;
	}
	Node* GetRight() noexcept {
		return pRight;
	}
	Node* GetParent() noexcept {
		return pParent;
	}

	void SetLeft(Node* Left) noexcept {
		pLeft = Left;
	}
	void SetRight(Node* Right) noexcept {
		pRight = Right;
	}
	void SetParent(Node* Parent) noexcept {
		pParent = Parent;
	}
	void SetValue(int newValue) noexcept {
		element = newValue;
	}

	bool IsRed() const noexcept {
		return isRed;
	}

	void PaintRed() noexcept {
		isRed = true;
	}
	void PaintBlack() noexcept {
		isRed = false;
	}
	int Height() const {
		if (pLeft == nullptr && pRight == nullptr) {
			return 0;
		}
		int heightLeft = (pLeft == nullptr) ? 0 : pLeft->Height();
		int heightRight = (pRight == nullptr) ? 0 : pRight->Height();
		return 1 + ((heightLeft > heightRight) ? heightLeft : heightRight);
	}
};