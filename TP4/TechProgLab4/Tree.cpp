#include "Tree.h"

Tree::Node* Tree::AddTree(Node* root, int value, int k)
{
	if (root == nullptr) {
		root = new Node;
		root->data = value;
		root->count = 0;
	}
	else if (root->Children.Size() < k) {
		root->Children.Push(nullptr);
		root->Children[root->Children.Size() - 1] = AddTree(root->Children[root->Children.Size() - 1], value, k);
	}
	else {
		if (root->count == k) root->count = 0;
		root->Children[root->count] = AddTree(root->Children[root->count], value, k);
		root->count++;
	}
	return root;
}

void Tree::PrintTree(Node* root, int space)
{
	for (int i = 0; i < space; i++)
		std::cout << " ";

	std::cout << root->data << "\n";
	if (!root->Children.Empty()) {
		for (int i = 0; i < root->Children.Size(); ++i) {
			PrintTree(root->Children[i], space + 4);
		}
	}
}
int Tree::FindHeight(Node* root)
{
	if (root == nullptr) return 0;
	int mx = 0;
	for (int i = 0; i < root->Children.Size(); ++i) {
		mx = Max(mx, FindHeight(root->Children[i]));
	}
	return 1 + mx;
}

void Tree::FindRoots(Node* root, int h)
{
	if (!root) return;
	if (FindHeight(root) == h && root != this->root) {
		FindMaxWidth(root);
	}
	for (int i = 0; i < root->Children.Size(); ++i) {
		FindRoots(root->Children[i], h);
	}
}

void Tree::FindMaxWidth(Node* root)
{
	Node* temproot = root;
	if (!temproot) return;
	int countlist = 0;
	FindWidth(temproot, countlist);
	int width = countlist;
	MaxWidth = Max(MaxWidth, width);
	MinWidth = Min(MinWidth, width);
	if (MaxWidth == width) {
		MaxSubtree = temproot;
	}
	if (MinWidth == width) {
		MinSubtree = temproot;
	}
}

int Tree::FindWidth(Node* root, int& countlist) {
	if (root == nullptr) return 0;
	if (root->Children.Empty()) countlist++;
	for (int i = 0; i < root->Children.Size(); ++i) {
		FindWidth(root->Children[i], countlist);
	}
}

bool Tree::IsEmpty()
{
	return !root;
}

void Tree::Print(int space)
{
	PrintTree(root, space);
}

void Tree::PrintMaxSubTree(int h)
{
	FindRoots(root, h);
	
	if (MaxSubtree == nullptr) {
		std::cout << "Нет поддерева с заданной высотой" << std::endl;
	}
	else {
		std::cout << "Самое широкое поддерево" << std::endl;
		PrintTree(MaxSubtree, 1);
	}
}

void Tree::PrintMinSubTree(int h)
{
	
	if (MinSubtree == nullptr) {
		std::cout << "Нет поддерева с заданной высотой" << std::endl;
	}
	else {
		std::cout << "Самое узкое поддерево" << std::endl;
		PrintTree(MinSubtree, 1);
	}
	
}

void Tree::AddValue(int num, int kol)
{
	root = AddTree(root, num, kol);
}
