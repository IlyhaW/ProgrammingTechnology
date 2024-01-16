#pragma once
template <typename T>
class Vector {
private:
	T* arr;
	int capacity;
	int currentsize;
public:
	Vector() : capacity(1), currentsize(0) {
		arr = new T[1];
	}
	~Vector() { delete[] arr; }
	void Push(T value) {
		if (currentsize == capacity) {
			capacity *= 2;
			T* temp = new T[capacity];
			for (int i = 0; i < currentsize; ++i) {
				temp[i] = arr[i];
			}
			delete[] arr;
			temp[currentsize] = value;
			currentsize++;
			arr = temp;
		}
		else {
			arr[currentsize] = value;
			currentsize++;
		}
	}
	bool Empty() {
		return currentsize == 0;
	}
	int Size() {
		return currentsize;
	}
	T& operator[](int index) {
		if (index >= 0 && index <= currentsize) {
			return arr[index];
		}
		return arr[0];
	}
};