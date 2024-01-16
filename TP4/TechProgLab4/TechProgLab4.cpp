#include <iostream>
#include <fstream>
#include "Tree.h"

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
int ReadFromFile(Tree& tree) {
    std::ifstream file;
    file.open("Programm.txt");
    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла\n";
        return 0;
    }

    int kolson, n, temp;
    file >> kolson >> n;
    for (int i = 0; i < n; ++i) {
        file >> temp;
        tree.AddValue(temp, kolson);
    }

    int height;
    file >> height;

    tree.Print(1);
    std::cout << "\n\n";
    tree.PrintMaxSubTree(height);
    tree.PrintMinSubTree(height);

    file.close();
    return 1;
}
/*int InputUser(Tree& tree) {
    int kolson = 0;
    while (true) {
        std::cout << "Введите максимальное количество потомков(N)\n";
        //int kolson=0;
        if (!(std::cin >> kolson) || kolson < 1) {
            std::cout << "Ошибка ввода" << std::endl;
            //return 0;
        }
    }
        std::cout << "Введите количество элементов" << std::endl;
        int n = 0;
        if (!(std::cin >> n) || n < 1) {
            std::cout << "Ошибка ввода" << std::endl;
            return 0;
        }
        std::cout << "Введите числовые элементы" << std::endl;
        int temp = 0;
        for (int i = 0; i < n; i++) {
            if (std::cin >> temp) {
                tree.AddValue(temp, kolson);
            }
            else {
                std::cout << "Ошибка ввода" << std::endl;
                return 0;
            }
        }
   
    std::cout << "Введите высоту" << std::endl;
    int height;
    if (!(std::cin >> height) || height < 1) {
        std::cout << "Ошибка ввода" << std::endl;
        return 0;
    }
tree.Print(1);
std::cout << "\n\n";
tree.PrintMaxSubTree(height);
tree.PrintMinSubTree(height);
return 1;
}*/
int main()
{
    setlocale(LC_ALL, "Rus");
    Tree tree;
    bool m = true;
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
        string Kson = "";
        string countN = "";
        string tempS = "";
        string heightS = "";
        int kolson = 0;
        bool KsonEntered = false;
        bool Nentered = false;
        int n = 0;
        int temp = 0;
        int height = 0;
        bool heightEntered = false;
        bool tempEntered = false;

        std::cout << "Введите максимальное количество потомков(N)\n";
        while (!KsonEntered) {
            cin >> Kson;
            if (!isNumber(Kson) && atoi(Kson.c_str()) < 1) {
                std::cout << "Ошибка ввода" << std::endl;
                std::cout << "Введите максимальное количество потомков(N)\n";
                continue;

            }
            kolson = atoi(Kson.c_str());
            KsonEntered = true;
        }
        std::cout << "Введите количество элементов" << std::endl;
        while (!Nentered) {
            cin >> countN;
            if (!isNumber(countN) && atoi(countN.c_str()) <= 1) {
                std::cout << "Ошибка ввода" << std::endl;
                std::cout << "Введите количество элементов" << std::endl;
                continue;

            }
            n = atoi(countN.c_str());
            Nentered = true;
        }
        std::cout << "Введите числовые элементы" << std::endl;
        while (!tempEntered) {

            for (int i = 0; i < n; i++) {
                if (std::cin >> tempS) {
                    if (!isNumber(tempS)) {
                        cout << "Ошибка ввода" << endl;
                        std::cout << "Введите числовые элементы" << std::endl;
                        i--;
                        continue;

                    }
                    temp = atoi(tempS.c_str());
                    tree.AddValue(temp, kolson);
                    tempEntered = true;
                }
            }
        }

        std::cout << "Введите высоту" << std::endl;
        while (!heightEntered) {
            cin >> heightS;
            if (!isNumber(heightS) && atoi(heightS.c_str()) < 1) {
                cout << "Ошибка ввода!" << endl;
                cout << "Введите высоту" << std::endl;
                continue;
            }
            height = atoi(heightS.c_str());
            heightEntered = true;
        }
                tree.Print(1);
                std::cout << "\n\n";
                tree.PrintMaxSubTree(height);
                tree.PrintMinSubTree(height);
                return 1;
            }
            if (input == 2) {
                if (ReadFromFile(tree) == 0) return 0;
            }
            if(input == 3){
                exit(0);
            }
        }
        
