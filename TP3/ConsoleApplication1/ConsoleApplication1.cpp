
#include <iostream>
#include <fstream>
using namespace std;

// Проверка является ли строка числом
bool isNumber(string str)
{
    for (size_t i = 0; i < str.size(); i++) {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

struct Node {
    int data;
    Node* left;
    Node* right;
};

// Функция для создания нового узла
Node* createNode(int data) {
    Node* newNode = new Node();
    if (!newNode) {
        cout << "Ошибка при создании нового узла!\n";
        return nullptr;
    }
    newNode->data = data;
    newNode->left = newNode->right = nullptr;
    return newNode;
}

// Функция для добавления узла в дерево
Node* insertNode(Node* root, int data) {
    if (root == nullptr)
        return createNode(data);
    else if (data < root->data)
        root->left = insertNode(root->left, data);
    else if (data > root->data)
        root->right = insertNode(root->right, data);
    return root;
}

// Функция для подсчета количества листьев в поддереве
int countLeaves(Node* root) {
    if (root == nullptr)
        return 0;
    if (root->left == nullptr && root->right == nullptr)
        return 1;
    else
        return countLeaves(root->left) + countLeaves(root->right);
}

// Функция для поиска самого высокого (низкого) поддерева с заданным числом листьев
Node* findSubtreeWithLeaves(Node* root, int leavesToFind, int& maxHeight) {
    if (root == nullptr)
        return nullptr;

    int leavesCount = countLeaves(root);
    if (leavesCount == leavesToFind) {
        // Поддерево с заданным числом листьев найдено
        if (root->left == nullptr && root->right == nullptr && maxHeight == 0)
            maxHeight = 1;  // Обновляем высоту поддерева до 1 (для случая дерева с одним узлом)
        return root;
    }

    Node* leftSubtree = findSubtreeWithLeaves(root->left, leavesToFind, maxHeight);
    Node* rightSubtree = findSubtreeWithLeaves(root->right, leavesToFind, maxHeight);

    // Возвращаем поддерево с более высокой (низкой) высотой
    if (leftSubtree == nullptr)
        return rightSubtree;
    else if (rightSubtree == nullptr)
        return leftSubtree;
    else if (maxHeight == 0)
        maxHeight = 1;  // Обновляем высоту поддерева до 1 (для случая дерева с одним узлом)
    else if (maxHeight == 1)
        return leftSubtree;  // Возвращаем левое поддерево (корень с самой низкой высотой)
    else
        return rightSubtree;  // Возвращаем правое поддерево (корень с самой высокой высотой)
}

// Функция для печати пути от корня до листа
void printPath(Node* node) {
    if (node == nullptr)
        return;
    printPath(node->left);
    cout << node->data << " ";
    printPath(node->right);
}

// Функция для освобождения памяти, выделенной для дерева
void deleteTree(Node* node) {
    if (node == nullptr)
        return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

// Функция для чтения дерева из файла
Node* readTreeFromFile(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cout << "Ошибка при открытии файла!\n";
        return nullptr;
    }

    Node* root = nullptr;
    int number;
    string inputNumber = "";
    while (inputFile >> inputNumber) {
        if (isNumber(inputNumber)) {
            number = atoi(inputNumber.c_str());
            root = insertNode(root, number);
        }
        else
        {
            cout << "Ошибка при чтении дерева!\n";
            return nullptr;
        }
    }

    inputFile.close();
    return root;
}

int main() {
    setlocale(LC_ALL, "Russian");
    // Создание дерева из консоли
    int number;
    int menu;
    Node* root = nullptr;
    int leavesToFind;
    Node* subtree;
    int maxHeight = 0;
    cout << "Выберите тип ввода: 1. Консоль 2. Файл 3. Выход." << endl;
    cin >> menu;
    switch (menu)
    {
    case 1:
    {
        string inputNumber = "";
        cout << "Введите элементы дерева (0 для конца ввода): ";
        bool treeEntered = false;

        while (!treeEntered) {
            cin >> inputNumber;
            if (!isNumber(inputNumber)) {
                cout << "Введено неверное число, попробуйте еще раз\n";
                continue;
            }
            number = atoi(inputNumber.c_str());
            if (number != 0)
            {
                if (root == nullptr)
                    root = createNode(number);
                else
                    insertNode(root, number);
            }
            else
            {
                treeEntered = true;
            }
        }

        // Печатаем дерево
        cout << "Дерево:\n";
        printPath(root);
        cout << "\n";

        // Поиск и печать самого высокого (низкого) поддерева с заданным числом листьев
        int leavesToFind;
        cout << "Введите количество листьев для поиска: ";
        cin >> leavesToFind;
        maxHeight = 0;
        Node* subtree = findSubtreeWithLeaves(root, leavesToFind, maxHeight);
        cout << "Самое высокое (низкое) поддерево с " << leavesToFind << " листьев: ";
        printPath(subtree);
        cout << "\n";

        // Освобождаем память, выделенную для дерева
        deleteTree(root);
        break;
    }
    case 2:
    {
        // Чтение дерева из файла
        string filename;
        cout << "Введите имя файла с деревом: ";
        cin >> filename;
        root = readTreeFromFile(filename);
        if (root == nullptr) {
            cout << "Не удалось прочитать дерево из файла!\n";
            return 0;
        }

        // Печатаем считанное дерево
        cout << "Дерево из файла:\n";
        printPath(root);
        cout << "\n";

        // Поиск и печать самого высокого (низкого) поддерева с заданным числом листьев
        cout << "Введите количество листьев для поиска: ";
        cin >> leavesToFind;
        maxHeight = 0;
        subtree = findSubtreeWithLeaves(root, leavesToFind, maxHeight);
        cout << "Самое высокое (низкое) поддерево с " << leavesToFind << " листьев: ";
        printPath(subtree);
        cout << "\n";

        // Освобождаем память, выделенную для дерева
        deleteTree(root);
        break;

    }
    case 3:
        exit(0);
        break;
    }





    return 0;
}
