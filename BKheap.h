#pragma once

#include <iostream>

using namespace std;

class Heap {//MAX HEAP
private:

	float* arr;
	int last = -1; 
	int maxSize = 0;

	void reheapUp(int childLoc) {
		if (childLoc > 0) {
			int parent = (childLoc - 1) / 2;
			if (arr[childLoc] > arr[parent]) {
				swap(arr[childLoc], arr[parent]);
				reheapUp(parent);
			}
		}
	};

	void reheapDown(int rootLoc) {
		int leftChild = rootLoc * 2 + 1;
		int rightChild = rootLoc * 2 + 2;
		int largeChild;
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

	void swap(float &a, float &b) {
		float temp = a;
		a = b;
		b = temp;
	}

public:
	Heap(int maxsize) {
		this->arr = new float[maxsize];
		this->last = -1;
		this->maxSize = maxsize;
	};

	~Heap() {
		delete arr;
		arr = NULL;
	};


	float getMax() {
		if (this->isEmpty()) {
			return -111111;
		}
		return this->arr[0];
	}

	void buildHeap(float *arrIn, int arrInSize) {
		this->arr = arrIn;
		int walker = 1;
		while (walker < arrInSize) {
			reheapUp(walker);
			walker = walker + 1;
		}
		this->last = arrInSize - 1;
	};

	bool heapInsert(float dataIn) {
		/*if (this->isEmpty()) {
			this->arr[0] = dataIn;
			this->last = 0;
		}*/
		if (this->isFull())
			return false;
		else {
			this->last++;
			arr[this->last] = dataIn;
			reheapUp(this->last);
			return true;
		}

	};

	bool heapDelete(float &dataOut) {
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

	bool isEmpty() { return last == -1; };

	void print(int rootLoc, int level) {
		for (int i = rootLoc; i < this->last + 1; i++) {
			cout << this->arr[i] << "__";
		}
		cout << endl;
	};

};