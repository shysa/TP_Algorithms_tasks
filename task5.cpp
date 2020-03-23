#include <iostream>

struct pair {
    int coord;
    int delta;
};



template <typename T>
void Merge (T *array, T *bufArray, int left, int right, int end)
{
    int bufIndex = left;
    int leftIndex = left;
    int rightIndex = right;

    while (leftIndex < right && rightIndex < end) {
        if (array[leftIndex] <= array[rightIndex]) {
            bufArray[bufIndex] = array[leftIndex];
            leftIndex++;
        } else {
            bufArray[bufIndex] = array[rightIndex];
            rightIndex++;
        }
        bufIndex++;
    }

    // если закончился какой-то массив, доливаем оставшийся
    // остались элементы в левом массиве
    while (leftIndex < right) {
        bufArray[bufIndex] = array[leftIndex];
        leftIndex++;
        bufIndex++;
    }
    // или в правом
    while (rightIndex < end) {
        bufArray[bufIndex] = array[rightIndex];
        rightIndex++;
        bufIndex++;
    }
}

template <typename T>
T* MergeSort(T * array, int size) {
    if (size <= 1) {
        return array;
    }

    T* result = new T[size];
    int right;
    int rightEnd;
    int k;

    for (k = 1; k < size; k += 2) {
        //разбиение на подмассивы 2^k
        for (int left = 0; left + k < size; left += k * 2) {
            right = left + k;
            rightEnd = right + k;

            //не выходим за пределы подмассива
            if (rightEnd > size) {
                rightEnd = size;
            }

            Merge(array, result, left, right, rightEnd);
        }
    }
    return result;
}



int main() {

    auto * array = new int[5];

    for (int i = 0; i < 5; i++) {
        std::cin >> array[i];
    }

    array = MergeSort(array, 5);

    for (int i = 0; i < 5; i++) {
        std::cout << array[i] << " ";
    }


    return 0;
}