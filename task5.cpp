#include <iostream>
// ----------------------------------- POINT PAIR ---------------------------------------
struct Point {
    int coord{};
    int delta = -1;

    friend std::ostream& operator<<(std::ostream &os, const  Point& point) {
        return std::cout << "[" << point.coord << ":" << point.delta << "]" << " ";
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
void Merge(Array<T> &array, T *buffer, int left, int right, int end, bool (*cmp)(const T&, const T&)) {
    int bufIndex = left;
    int i = left;
    int j = right;

    while (i < right && j < end) {
        //if (array[i] <= array[j]) {
        if ( cmp(array[i], array[j]) ) {
            buffer[bufIndex] = array[i];
            i++;
        } else {
            buffer[bufIndex] = array[j];
            j++;
        }
        bufIndex++;
    }

    while (i < right) {
        buffer[bufIndex] = array[i];
        i++;
        bufIndex++;
    }
    while (j < end) {
        buffer[bufIndex] = array[j];
        j++;
        bufIndex++;
    }

    for (bufIndex = left; bufIndex < end; bufIndex++) {
        array[bufIndex] = buffer[bufIndex];
    }
}

template <typename T>
void MergeSort(Array<T> &array, int size, bool (*cmp)(const T&, const T&)) {
    T *buffer = new T[size];

    int right;
    int rightEnd;

    for (int k = 1; k < size; k *= 2 ) {
        //разбиение на подмассивы 2^k
        for (int left = 0; left + k < size; left += k * 2 ) {
            right = left + k;
            rightEnd = right + k;

            //не выходим за границы подмассива
            if (rightEnd > size) {
                rightEnd = size;
            }

            Merge(array, buffer, left, right, rightEnd, cmp);
        }
    }
}

int main() {
    int n;

    Array<Point> array;
    Point point{};

    /*
    n = 2;
    auto * arr = new Point[2*n];
    //std::cin >> n;

    //у каждого отрезка по 2 координаты-точки => 2n
    for (int i = 0; i < 2 * n; i++) {
        std::cin >> point;
        arr[i] = point;
        array.insert(point);
    }

    array.show();
    std::cout<<std::endl;

    arr = MergeSort(arr, 2*n, cmpPoint);

    for (int i = 0; i < 2 * n; i++) {
        std::cout << arr[i];
    }
*/

    for (int i = 0; i < 5; i++) {
        std::cin >> point;
        array.insert(point);
    }

    array.show();
    std::cout<<std::endl;

    MergeSort(array, 5, cmpPoint);

    array.show();

    return 0;
}