//
// Created by shysa on 11.03.2020.
//

#include <iostream>

struct Train {
    int arrival;
    int departure;
};

// Куча построена на основе динамического массива
template <typename T>
class Array {
public:
    Array() : data(nullptr), size(0), tail(0) { Resize(); }
    ~Array() {
        if (data) {
            delete[] data;
        }
    }

    void Add(T value) {
        if (tail == size) {
            Resize();
            Add(value);
        } else {
            data[tail] = value;
            tail++;
        }
    }

    void Resize() {
        int newSize = (size > 0)? size*2 : 2;
        T *newData = new T[newSize];

        int j = 0;
        for (int i = 0; i < size; i++) {
            newData[j++] = data[i];
        }
        tail = j;

        if (data) {
            delete[] data;
        }

        data = newData;
        size = newSize;
    }

    void ShowArray() {
        for (int i = 0; i < size; i++) {
            std::cout << data[i] << " ";
        }
    }

    int Size() const {
        return size;
    }

    bool isEmpty() {
        return (size > 0);
    }

    T &operator[] (int i) {
        return data[i];
    }

private:
    T *data;
    int tail;
    int size;
};


template <class T>
class Heap {
public:
    Heap<T>(): array() {}
    //explicit Heap(const T& _arr);
    ~Heap() {}

    void Insert(T element);
    void Show() {
        for (int i = 0; i < array.Size(); i++) {
            std::cout << array[i] << " ";
        }
    }
    //T ExtractMax();
    //T PeekMax() const;

private:
    Array<T> array;

    void siftDown(int index);
    void siftUp(int index);
};

template <typename T>
void Heap<T>::siftDown(int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    // больший потомок
    int largest = index;
    if (left < array.Size() && array[left] > array[index]) {
        largest = left;
    }
    if (right < array.Size() && array[right] > array[largest]) {
        largest = right;
    }

    if (largest != index) {
        std::swap(array[index], array[largest]);
        siftDown(largest);
    }
}

template <typename T>
void Heap<T>::siftUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (array[index] <= array[parent]) {
            return;
        }
        std::swap(array[index], array[parent]);
        index = parent;
    }
}

template <typename T>
void Heap<T>::Insert(T element) {
    array.Add(element);
    siftUp(array.Size() - 1);
}




int main() {
    int n = 0;
    std::cin >> n;

    Heap<int> heap;

    for (int i = 0; i < n; i++) {

    }



    return 0;
}