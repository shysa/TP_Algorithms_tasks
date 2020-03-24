#include <iostream>
// ----------------------------------- POINT PAIR ---------------------------------------
struct Point {
    int coord{};
    int delta = -1;

    friend std::ostream& operator<<(std::ostream &os, const  Point& point) {
        return std::cout << "[" << point.coord << ":" << point.delta << "]" <<std::endl;
    }
    friend std::istream& operator>>(std::istream &in, Point& point) {
        point.delta *= -1;
        return std::cin >> point.coord;
    }
};
bool cmpPoint(const Point &l, const Point &r) {
    return l.coord <= r.coord;
}

// ----------------------------------- DYNAMIC ARRAY ---------------------------------------
template <typename T>
class Array {
public:
    Array<T>(): array(nullptr), size (0), capacity(0) {}
    explicit Array<T>(int n): size (n), capacity(n) {
        array = new T[n];
    }
    ~Array() {
        delete[] array;
    }

    T& operator [](int index) {
        return array[index];
    }

    void insert(T& element);
    void show() {
        for (int i = 0; i < size; i++) {
            std::cout << array[i];
        }
    }
    bool isEmpty() const {
        return size == 0;
    }
    int getSize() {
        return size;
    }

private:
    T *array;
    int size;
    int capacity;

    void resize();
};

template <typename T>
void Array<T>::insert(T &element){
    if (size == capacity) {
        resize();
    }
    array[size] = element;
    size++;
}

template <typename T>
void Array<T>::resize() {
    int newCapacity = (capacity > 0)? capacity * 2 : 2;
    T *newArray = new T[newCapacity];

    for (int i = 0; i < size; i++) {
        newArray[i] = array[i];
    }
    delete[] array;

    array = newArray;
    capacity = newCapacity;
}

// ----------------------------------- MERGE SORT ---------------------------------------
template <typename T>
void Merge (Array<T> &array, Array<T> &bufArray, int left, int right, int end, bool (*cmp)(const T&, const T&)) {
    int bufIndex = left;
    int leftIndex = left;
    int rightIndex = right;


    while (leftIndex < right && rightIndex < end) {
        if (cmp(array[leftIndex], array[rightIndex])) {
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

template <class T>
void MergeSort(Array<T> &array, int size, bool (*cmp)(const T&, const T&)) {

    Array<T> result(size);

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

            Merge(array, result, left, right, rightEnd, cmp);
        }
    }
    result.show();
}


int main() {
    int n;

    Array<Point> array;
    Point point{};

    n = 2;
    //std::cin >> n;

    //у каждого отрезка по 2 координаты-точки => 2n
    for (int i = 0; i < 2 * n; i++) {
        std::cin >> point;
        array.insert(point);
    }

    array.show();
    std::cout<<std::endl;

    MergeSort(array, 2*n, cmpPoint);
    std::cout<<std::endl;

    array.show();



    return 0;
}