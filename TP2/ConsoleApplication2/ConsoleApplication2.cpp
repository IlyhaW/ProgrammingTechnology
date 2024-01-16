#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

bool isNumber(string value) {
	for (size_t i = 0; i < value.size(); i++) {
		if (!isdigit(value[i])) {
			return false;
		}
	}
	return true;
}

class Matrix {
private:
	float** matrix;
	int size; // Размер матрицы (количество строк или столбцов)
	int* values;
	int* rowIndices;
	int* colIndices;
	int nnz;
	int* AD;
	int* AU;
	int* AL;
	int* LJ;
	int* LI;
public:
	Matrix(const char* filename) {
		
		ifstream file(filename);

		if (!file.is_open()) {
			cout << "Не удается открыть файл." << endl;
			size = 0;
			matrix = nullptr;
			return;
		}

		string sizeStr = "";
		file >> sizeStr;
		if (!isNumber(sizeStr))
		{
			cout << "Введено неверное значение!" << endl;
			exit(-1);
		}
		size = atoi(sizeStr.c_str());
		if (size < 1)
		{
			cout << "Введено неверное значение!" << endl;
			exit(-1);
		}


		string value;

		// Выделяем память под матрицу
		matrix = new float* [size];
		for (int i = 0; i < size; ++i) {
			matrix[i] = new float[size];
		}


		// Считываем элементы матрицы из файла
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				file >> value;
				if (isNumber(value)) {
					matrix[i][j] = atoi(value.c_str());
				}
				else {
					cout << "Введено неверное значение!" << endl;
					exit(-1);
				}
			}
		}

		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if (i != j) {
					if ((matrix[i][j] != 0 && matrix[j][i] == 0) || (matrix[i][j] == 0 && matrix[j][i] != 0)) {
						cout << "Матрица не симметрична" << endl;
						exit(-1);
					}
				}
			}
		}

		file.close();

		countNonZeroElements();
		findElem();
		packMatrix();
	}
	Matrix()
	{
		
		string sizeStr = "";
		cin >> sizeStr;
		cout << "Введите значения элементов матрицы" << endl;
		if (!isNumber(sizeStr))
		{
			cout << "Введено неверное значение!" << endl;
			exit(-1);
		}
		size = atoi(sizeStr.c_str());
		if (size < 1)
		{
			cout << "Введено неверное значение!" << endl;
			exit(-1);
		}

		matrix = new float* [size] {0};
		for (int i = 0; i < size; ++i) {
			matrix[i] = new float[size] {0};
		}


		string value;
		// Считываем элементы матрицы из консоли
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				cin >> value;
				if (isNumber(value)) {
					matrix[i][j] = atoi(value.c_str());
				}
				else {
					cout << "Введено неверное значение!" << endl;
					exit(-1);
				}
			}
		}

		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if (i != j) {
					if ((matrix[i][j] != 0 && matrix[j][i] == 0) || (matrix[i][j] == 0 && matrix[j][i] != 0)) {
						cout << "Матрица не симметрична" << endl;
						exit(-1);
					}
				}
			}
		}

		countNonZeroElements();
		findElem();
		packMatrix();
	}

	~Matrix() {
		if (matrix != nullptr) {
			delete[] matrix;
		}
		delete[] values;
		delete[] rowIndices;
		delete[] colIndices;
		delete[] AD;
		delete[] AU;
		delete[] AL;
		delete[] LJ;
		delete[] LI;
	}

	//void gaussianElimination(std::vector<std::vector<double>>& matrix) {
	float gaussianElimination() {
		float solution = 1;

		int numRows = size;
		int numColumns = size;

		for (int i = 0; i < numRows; i++) {
			if (matrix[i][i] == 0)
			{
				// Find not null
				for (size_t j = i; j < size; j++)
				{
					if (matrix[j][i] != 0)
					{
						swapLines(j, i);
						break;
					}
					if (matrix[i][i] == 0)
					{
						solution = 0;
						return solution;
					}
				}
			}

			// Приведение матрицы к треугольному виду
			for (int j = i + 1; j < numRows; j++) {
				double coefficient = -matrix[j][i] / matrix[i][i];
				for (int k = i; k < numColumns + 1; k++) {
					if (i == k) {
						matrix[j][k] = 0;
					}
					else {
						matrix[j][k] += coefficient * matrix[i][k];
					}
				}
			}
		}


		// Обратный ход метода Гаусса
		//std::vector<double> solution(numColumns);
		for (size_t i = 0; i < size; i++)
		{
			solution *= matrix[i][i];
		}

		return solution;
	}

	void swapLines(int first, int second)
	{
		int temp = 0;
		for (size_t i = 0; i < size; i++)
		{
			temp = matrix[first][i];
			matrix[first][i] = matrix[second][i];
			matrix[second][i] = temp;
		}
	}

	void printMatrix() {

		if (matrix == nullptr) {
			cout << "Матрица не инициализирована." << endl;
			return;
		}

		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				cout << matrix[i][j] << " ";
			}
			cout << endl;
		}
	}

	void countNonZeroElements() {
		nnz = 0;
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if (matrix[i][j] != 0 && i != j) {
					nnz++;
				}
			}
		}
	}

	void findElem() {
		values = new int[nnz];
		rowIndices = new int[nnz];
		colIndices = new int[nnz];

		int index = 0;

		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if (matrix[i][j] != 0 && i != j) {
					values[index] = matrix[i][j];
					rowIndices[index] = i;
					colIndices[index] = j;
					index++;
				}
			}
		}
	}

	void packMatrix() {
		AD = new int[size];
		AU = new int[nnz / 2];
		AL = new int[nnz / 2];
		LJ = new int[size];
		LI = new int[size];

		int adIndex = 0;
		int auIndex = 0;
		int alIndex = 0;
		int ljIndex = 0;
		int liIndex = 0;

		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if (i == j) {
					AD[adIndex++] = matrix[i][j];
				}
				else if (i < j && matrix[i][j] != 0) {
					AU[auIndex++] = matrix[i][j];
				}
				else if (i > j && matrix[i][j] != 0) {
					AL[alIndex++] = matrix[i][j];
				}
			}
		}

		for (int j = 0; j < size; ++j) {
			LJ[j] = -1;
			for (int i = 0; i < size; ++i) {
				if (i < j && matrix[i][j] != 0) {
					LJ[ljIndex++] = j + 1;
					break;
				}
			}
		}

		int count2 = 1;
		for (int i = 0; i < size; i++) // Массив LI
		{
			LI[i] = count2;
			for (int j = 0; j < size; j++)
			{
				if (j > i && matrix[i][j] != 0)
				{
					count2 += 1;
				}
			}
		}
	}

	void printPackedMatrix() {
		// Выводим упакованные данные
		cout << "Упакованные данные" << endl;
		cout << "AD: ";
		for (int i = 0; i < size; ++i) {
			cout << AD[i] << " ";
		}
		cout << endl;

		cout << "AU: ";
		for (int i = 0; i < nnz / 2; ++i) {
			cout << AU[i] << " ";
		}
		cout << endl;

		cout << "AL: ";
		for (int i = 0; i < nnz / 2; ++i) {
			cout << AL[i] << " ";
		}
		cout << endl;
		cout << "LJ: ";
		for (int i = 0; i < nnz / 2; ++i) {
			cout << LJ[i] << " ";
		}
		cout << endl;
		cout << "LI: ";
		for (int i = 0; i < size; ++i) {
			cout << LI[i] << " ";
		}
		cout << endl;
		cout << endl;
	}

};


int main() {
	setlocale(LC_ALL, "Ru");

	const char* filename = "matrix.txt";

	int choise;
	cout << "Выберите метод ввода данных:  1 - КОНСОЛЬ, 2 - ФАЙЛ\n";
	string inputNumber = "";
	cin >> choise;

	if (choise == 1)                    // Ввод с консоли
	{
		cout << "Введите размерность матрицы (n*n)" << endl;
		Matrix matrix;
		matrix.printMatrix();
		matrix.printPackedMatrix();
		std::cout << "Определитель: " + to_string(matrix.gaussianElimination());
	}
	else if (choise == 2)                // Ввод с файла
	{

		Matrix matrix(filename);
		cout << endl;
		matrix.printMatrix();
		matrix.printPackedMatrix();
		std::cout << "Определитель: " + to_string(matrix.gaussianElimination());
	}
	else
	{
		cout << "Неправильный выбор\n";
	}


	return 0;

}