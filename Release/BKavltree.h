#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<stack>
#include"BKheap.h"

using namespace std;

enum status { LH = -1, EH, RH };
struct node {
	string key ="";
	int numberSub = 0;
	bool isLoading = 0;
	Heap *scoreHeap = new Heap(50);
	stack<float> scoreStack;
	node* left = NULL;
	node* right = NULL;
	status balance = EH;
	node(string name, int value) { key = name; numberSub = value; }
	node(string name, int val, float s) { key = name, numberSub = val, scoreHeap->heapInsert(s); }
};
node* newNode(string namein, int numbersub, float score) {
	node *res = new node(namein, numbersub, score);
	return res;
}
node* rotateRight(node* & root) {
	node* tempPtr = root->left;
	root->left = tempPtr->right;
	tempPtr->right = root;
	return tempPtr;
}
node* rotateLeft(node* & root) {
	node* tempPtr = root->right;
	root->right = tempPtr->left;
	tempPtr->left = root;
	return tempPtr;
}
node* leftBalance(node* &root, bool &taller) {

	node* leftTree = root->left;
	//Case 1: L of L
	if (leftTree->balance == LH)
	{
		root->balance = EH;
		leftTree->balance = EH;
		root = rotateRight(root); // single ratation right
		taller = false;
	}
	else
	{
		node* rightTree = leftTree->right;
		if (rightTree->balance == LH) {
			root->balance = RH;
			leftTree->balance = EH;
		}
		else if (rightTree->balance == EH)
		{
			leftTree->balance = EH; root->balance = EH;
		}
		else {
			root->balance = EH;
			leftTree->balance = LH;
		}
		rightTree->balance = EH;
		root->left = rotateLeft(leftTree);
		root = rotateRight(root);
		taller = false;
	}
	return root;
}
node* rightBalance(node* &root, bool &taller) {
	node* rightTree = root->right;
	if (rightTree->balance == RH) {
		root->balance = EH;
		rightTree->balance = EH;
		root = rotateLeft(root);


		taller = false;
	}
	else {
		node* leftTree = rightTree->left;
		if (leftTree->balance == RH) {
			root->balance = LH;
			rightTree->balance = EH;
		}
		else if (leftTree->balance == EH) { rightTree->balance = EH; root->balance = EH; }
		else {
			root->balance = EH;
			rightTree->balance = RH;
		}
		leftTree->balance = EH;
		root->right = rotateRight(rightTree);
		root = rotateLeft(root);
		taller = false;
	}
	return root;
}
node* deleteRightBalance(node* & root, bool & shorter) {
	if (!shorter) return root;
	if (root->balance == LH) root->balance = RH;
	else if (root->balance == EH) {
		root->balance = RH;
		shorter = false;
	}
	else {
		node* rightTree = root->right;
		if (rightTree->balance == LH) {
			node* leftTree = rightTree->left;
			if (leftTree->balance == LH) {
				rightTree->balance = RH;
				root->balance = EH;
			}
			else if (leftTree->balance == EH) {
				root->balance = LH;
				rightTree->balance = EH;
			}
			else {
				root->balance = LH;
				rightTree->balance = EH;
			}
			leftTree->balance = EH;
			root->right = rotateRight(rightTree);
			root = rotateLeft(root);
		}
		else {
			if (rightTree->balance != EH) {
				root->balance = EH;
				rightTree->balance = EH;
			}
			else {
				root->balance = RH;
				rightTree->balance = LH;
				shorter = false;
			}
			root = rotateLeft(root);
		}
	}
	return root;
}
node* deleteLeftBalance(node* & root, bool &shorter) {
	if (!shorter) return root;
	if (root->balance == RH)root->balance = EH;
	else if (root->balance == EH) {
		root->balance = LH;
		shorter = false;
	}
	else {
		node* leftTree = root->left;
		if (leftTree->balance == RH) {
			node* rightTree = leftTree->right;
			if (rightTree->balance == RH) {
				leftTree->balance = LH;
				root->balance = EH;
			}
			else if (rightTree->balance == EH) {
				root->balance = RH;
				leftTree->balance = EH;
			}
			else {
				root->balance = RH;
				leftTree->balance = EH;
			}
			rightTree->balance = EH;
			root->left = rotateLeft(leftTree);
			root = rotateRight(root);
		}
		else {
			if (leftTree->balance != EH) {
				root->balance = EH;
				leftTree->balance = EH;
			}
			else
			{
				root->balance = LH;
				leftTree->balance = RH;
				shorter = false;
			}
			root = rotateRight(root);
		}

	}
	return root;
}
class avlTree {
public:
	node* root = NULL;
	node* AVLInsert(node*& root, node* newPtr, bool & taller) {
		if (root == NULL) {
			root = newPtr;
			taller = true;
			return root;
		}
		if (newPtr->key < root->key)
		{
			root->left = AVLInsert(root->left, newPtr, taller);
			if (taller)
			{
				if (root->balance == LH) root = leftBalance(root, taller);
				else if (root->balance == EH) root->balance = LH;
				else {
					root->balance = EH;
					taller = false;
				}
			}
		}
		else {
			root->right = AVLInsert(root->right, newPtr, taller);
			if (taller) {
				if (root->balance == LH) {
					root->balance = EH;
					taller = false;
				}
				else if (root->balance == EH) root->balance = RH;
				else root = rightBalance(root, taller);
			}
		}
		return root;
	}
	node* AVLDelete(node* & root, string deletekey, bool & shorter, bool & success) {
		if (root == NULL) {
			shorter = false;
			success = false;
			return NULL;
		}
		if (deletekey < root->key) {
			root->left = AVLDelete(root->left, deletekey, shorter, success);
			if (shorter)
				root = deleteRightBalance(root, shorter);
		}
		else if (deletekey > root->key) {
			root->right = AVLDelete(root->right, deletekey, shorter, success);
			if (shorter) root = deleteLeftBalance(root, shorter);
		}
		else {
			node* deleteNode = root;
			if (root->right == NULL) {
				node* newRoot = root->left;
				success = true;
				shorter = true;
				delete deleteNode;
				return newRoot;
			}
			else if (root->left == NULL) {
				node* newRoot = root->right;
				success = true;
				shorter = true;
				delete deleteNode;
				return newRoot;
			}
			else {
				node* exchPtr = root->left;
				while (exchPtr->right != NULL)
					exchPtr = exchPtr->right;
				root->key = exchPtr->key;
				root->left = AVLDelete(root->left, exchPtr->key, shorter, success);
				if (shorter)
					root = deleteRightBalance(root, shorter);
			}
		}
		return root;
	}
	void print_preorder(node* root) {
		if (root == NULL) return;
		else
		{
			cout << "  " << root->key << "  ";
			print_preorder(root->left);
			print_preorder(root->right);
		}
	}
	void print_inorder(node* root) {
		if (root == NULL) return;
		else
		{
			
			print_inorder(root->left);
			cout << "  " << root->key << "|" << root->numberSub << endl;
			print_inorder(root->right);
		}
	}
	void print_preorderbalance(node* root) {
		if (root == NULL) return;
		else
		{
			cout << "  " << root->balance << "  ";
			print_preorderbalance(root->left);
			print_preorderbalance(root->right);
		}
	}
	bool taller = false;
	bool shorter = false;
	node* search(string value) {
		if (root==NULL) return NULL;
		node* current = root;
		while (current->key != value) {
			if (value > current->key) current = current->right;
			else current = current->left;
			if (current == NULL) return NULL;
		}
		return current;
	}
	bool is_empty() { 
		if (root) return 0;
		else return 1;
	}

	void saveAVL(node *p, std::ofstream &out) {
		if (!p) {
			out << "#" << endl;
			//fprintf_s(out, "%s ", "#");
			return;
		}
		else {
			//fprintf_s(out, "%s*%d*%f", p->key, p->numberSub, p->scoreHeap->getMax());
			out << p->key << "*" << p->numberSub << "*" << p->scoreHeap->getMax() << endl;
			saveAVL(p->left, out);
			saveAVL(p->right, out);
		}
	}

	void loadAVL(node *&p, std::ifstream &fin) {
		string val;

		if (!getline(fin, val) || val == "#")
			return;
		int star1 = val.find("*");
		int star2 = star1 + val.substr(star1 + 1, val.length() - 1).find("*");
		string id = val.substr(0, star1);
		string numsub = val.substr(star1 + 1, star2);
		string score = val.substr(star2 + 2, val.length() - 1);

		stringstream temp(numsub);
		int num;
		temp >> num;
		stringstream temp2(score);
		float scoref;
		temp2 >> scoref;
		p = newNode(id, num, scoref);
		loadAVL(p->left, fin);
		loadAVL(p->right, fin);
	}
};
bool operator >(string a, string b) {
	if (a.compare(b) > 0) return true;
	else return false;
}
bool operator <(string a, string b) {
	if (a.compare(b) < 0) return true;
	else return false;
}
bool operator !=(string a, string b) {
	if (a.compare(b) != 0) return true;
	else return false;
}


