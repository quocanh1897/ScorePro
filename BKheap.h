#pragma once

#include <iostream>

using namespace std;

class Heap {//MAX HEAP
private:
	void reheapUp(int childLoc) {
		if (childLoc > 0) {
			int parent = (childLoc - 1) / 2;
			if (arr[childLoc] > arr[parent]) {
				swap(arr[childLoc], arr[parent]);
				reheapUp(parent);
			}
		}
	};

	void reheapDown(int rootLoc) {//rootLoc: vị trí nút trong arr
		int leftChild = rootLoc * 2 + 1;
		int rightChild = rootLoc * 2 + 2;
		int largeChild;
		//int lastLoc = rootLoc;
		int pre = arr[rootLoc];
		int pos = rootLoc;
		if (leftChild <= this->last) {
			if (rightChild <= this->last && arr[rightChild] > arr[leftChild])
				largeChild = rightChild;
			else
				largeChild = leftChild;

			if (arr[largeChild] > arr[pos]) {
				swap(arr[largeChild], arr[pos]);
				reheapDown(largeChild);
			}
		}
	};

	void swap(int &a, int &b) {
		int temp = a;
		a = b;
		b = temp;
	}

	int* arr;
	int last;//chi muc phan tu cuoi cung
	int maxSize;

public:
	Heap(int maxsize) {
		this->arr = new int[maxsize];
		this->last = -1;
	};

	~Heap() {
		delete arr;
		arr = NULL;
	};

	void buildHeap(int *arrIn, int arrInSize) {
		this->arr = arrIn;
		int walker = 1;
		while (walker < arrInSize) {
			reheapUp(walker);
			walker = walker + 1;
		}
		this->last = arrInSize - 1;
	};

	bool heapInsert(int dataIn) {
		if (this->isFull())
			return false;
		this->last++;
		arr[this->last] = dataIn;
		reheapUp(this->last);
		return true;
	};

	bool heapDelete(int &dataOut) {
		if (this->isEmpty())
			return false;
		dataOut = this->arr[0];
		this->arr[0] = this->arr[this->last];
		this->last--;
		reheapDown(0);
		return true;
	};

	int getSize() { return last + 1; };

	bool isFull() { return last + 1 == maxSize; };

	bool isEmpty() { return last == 0; };

	void print(int rootLoc, int level) {
		for (int i = rootLoc; i < this->last + 1; i++) {
			cout << this->arr[i] << "__";
		}
		cout << endl;
	};

};