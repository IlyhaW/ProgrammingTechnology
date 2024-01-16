#pragma once
#include <iostream>
#include "Vector.h"
class Tree
{
private:
	struct Node {
		int count;
		Vector<Node*> Children;
		int data;
	};
	Node* root;
	Node* MaxSubtree;
	Node* MinSubtree;
	int MaxWidth = 0;
	int MinWidth = 100000000;

	Node* AddTree(Node* root, int value, int k);

	void PrintTree(Node* root, int space);
	int Max(int a, int b) {
		return a > b ? a : b;
	}
	int Min(int a, int b) {
		return a < b ? a : b;
	}
	int FindHeight(Node* root);

	void FindRoots(Node* root, int h);
	void FindMaxWidth(Node*subroot);
	int FindWidth(Node* root, int& count);
public:
	Tree() : root(nullptr), MaxSubtree(nullptr), MinSubtree(nullptr) {}

	bool IsEmpty();

	void Print(int space);
	void PrintMaxSubTree(int h);
	void PrintMinSubTree(int h);
	void AddValue(int num, int kol);
};

