#include <iostream>
#include <string>
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
// Структура для хранения информации о человеке
struct Person {
    std::string name;
    std::string surname;
    std::string patronymic;
};

// Хеш-функция умножения для строки
size_t hashFunction(const std::string& str, size_t tableSize) {
    const int p = 31; // Простое число для умножения
    size_t hashValue = 0;
    for (char c : str) {
        hashValue = (hashValue * p + c) % tableSize;
    }
    return hashValue;
}

// Структура для хранения бинарного дерева
struct TreeNode {
    Person person;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const Person& person) : person(person), left(nullptr), right(nullptr) {}
};

// Класс для хэш-таблицы с методом разрешения коллизий через бинарные деревья
class HashTable {
private:
    TreeNode** table;
    size_t tableSize;

public:
    HashTable(size_t size) : tableSize(size) {
        table = new TreeNode * [tableSize]();
    }

    // Метод добавления человека в таблицу
    void insertPerson(const Person& person) {
        size_t hashValue = hashFunction(person.name + person.surname + person.patronymic, tableSize);
        TreeNode* newNode = new TreeNode(person);

        // Если в ячейке таблицы уже есть элемент, добавляем его в бинарное дерево
        if (table[hashValue] != nullptr) {
            TreeNode* currentNode = table[hashValue];
            while (true) {
                if (person.name + person.surname + person.patronymic < currentNode->person.name + currentNode->person.surname + currentNode->person.patronymic) {
                    if (currentNode->left == nullptr) {
                        currentNode->left = newNode;
                        break;
                    }
                    currentNode = currentNode->left;
                }
                else {
                    if (currentNode->right == nullptr) {
                        currentNode->right = newNode;
                        break;
                    }
                    currentNode = currentNode->right;
                }
            }
        }
        else {
            table[hashValue] = newNode;
        }
    }

    // Метод поиска человека в таблице
    Person findPerson(const std::string& name, const std::string& surname, const std::string& patronymic) {
        size_t hashValue = hashFunction(name + surname + patronymic, tableSize);
        TreeNode* currentNode = table[hashValue];
        while (currentNode != nullptr) {
            if (currentNode->person.name == name && currentNode->person.surname == surname && currentNode->person.patronymic == patronymic) {
                return currentNode->person;
            }
            if (name + surname + patronymic < currentNode->person.name + currentNode->person.surname + currentNode->person.patronymic) {
                currentNode = currentNode->left;
            }
            else {
                currentNode = currentNode->right;
            }
        }
        return { "", "", "" }; // Если человек не найден, возвращаем пустую информацию
    }
    

    // Деструктор для освобождения памяти
    ~HashTable() {
        for (size_t i = 0; i < tableSize; i++) {
            if (table[i] != nullptr) {
                deleteTree(table[i]);
            }
        }
        delete[] table;
    }

private:
    // Метод для освобождения памяти из поддерева
    void deleteTree(TreeNode* node) {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
};

bool readFromFile() {
    HashTable hashTableFile(100);
    string Fname;
    string Fsurname;
    string Fpatronymic;
    string NameOfFile;

    NameOfFile ="programm.txt";
    ifstream inputFile(NameOfFile);
    if (inputFile.is_open()) {
        
        string FileName, FileSurname, FilePatronymic;
        while (inputFile >> FileName >> FileSurname >> FilePatronymic) {
            hashTableFile.insertPerson({ FileName, FileSurname, FilePatronymic });
        }
        inputFile.close();
    }
    else {
        cout << "Ошибка открытия файла!" << endl;
        return false;
        
    }
    
    cout << "Введите данные для поиска" << endl;
    cout << "Имя: ";
    cin >> Fname;
    cout << "Фамилия: ";
    cin >> Fsurname;
    cout << "Отчество: ";
    cin >> Fpatronymic;
    Person person = hashTableFile.findPerson(Fname, Fsurname, Fpatronymic);
    if (person.name != "") {
        std::cout << "Человек найден: " << person.name << " " << person.surname << " " << person.patronymic << " " << std::endl;
    }
    else {
        cout << "Такого человека нет!" << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    HashTable hashTable(100); // Создаем хэш-таблицу размером 100
    string MenuNumber = "";

    std::cout << "Выберете ввод: для ввода из консоли нажмите 1, для чтения из файла нажмите 2, для выхода 3" << "\n";
    int input = 0;

    bool MenuEntered = false;
    while (!MenuEntered) {
        cin >> MenuNumber;
        if (!isNumber(MenuNumber)) {
            cout << "Ошибка!Повторите ввод." << endl;
            cout << "Выберете ввод: для ввода из консоли нажмите 1, для чтения из файла нажмите 2, для выхода 3" << "\n";
            continue;
        }

        input = atoi(MenuNumber.c_str());
        if (input != 1 && input != 2 && input != 3) {
            cout << "Ошибка!Повторите ввод." << endl;
            cout << "Выберете ввод: для ввода из консоли нажмите 1, для чтения из файла нажмите 2, для выхода 3" << "\n";
            continue;
        }

        else
        {
            MenuEntered = true;
        }
    }

    if (input == 1) {
        while(true){
        string InName, InSurname, InPatronymic;
        string Fname, Fsurname, Fpatronymic;
        string Vmenu;
        int Vmenu1;
        bool VmenuEntered = false;
        
            cout << "Что будем делать? 1. Добавить человека в таблицу, 2. Найти человека, 3.Выход." << endl;
            while (!VmenuEntered) {
                cin >> Vmenu;
                if (!isNumber(Vmenu)) {
                    std::cout << "Ошибка ввода" << std::endl;
                    std::cout << "Что будем делать? 1. Добавить человека в таблицу, 2. Найти человека, 3.Выход.\n";
                    continue;

                }
                Vmenu1 = atoi(Vmenu.c_str());
                if (Vmenu1 != 1 && Vmenu1 != 2 && Vmenu1 != 3) {
                    std::cout << "Ошибка ввода" << std::endl;
                    std::cout << "Что будем делать? 1. Добавить человека в таблицу, 2. Найти человека, 3.Выход.\n";
                    continue;

                }
                VmenuEntered = true;
            }
            if (atoi(Vmenu.c_str()) == 1) {
                cout << "Введите данные:" << endl;
                cout << "Имя: ";
                cin >> InName;
                cout << "Фамилия: ";
                cin >> InSurname;
                cout << "Отчество: ";
                cin >> InPatronymic;
                hashTable.insertPerson({ InName, InSurname, InPatronymic });
                cout << "Готово!" << endl;
            }
            if (atoi(Vmenu.c_str()) == 2) {
                cout << "Введите данные:" << endl;
                cout << "Имя: ";
                cin >> Fname;
                cout << "Фамилия: ";
                cin >> Fsurname;
                cout << "Отчество: ";
                cin >> Fpatronymic;
                Person person = hashTable.findPerson(Fname, Fsurname, Fpatronymic);
                if (person.name != "") {
                    std::cout << "Человек найден: " << person.name << " " << person.surname << " " << person.patronymic << " " << std::endl;
                }
                else {
                    cout << "Такого человека нет!" << endl;
                }
            }
            if (atoi(Vmenu.c_str()) == 3) {
                exit(0);
            }
        }
    }
    if (input == 2) {
        
        bool GoodRead;
        
            GoodRead = readFromFile();
           
            while (GoodRead == false) {
                
                readFromFile();
            }
        }
    
    if (input == 3) {
        exit(0);
    }
    }
    