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
    // i - обход по первому подмассиву
    // j - обход по второму подмассиву
    int i = left;
    int j = right;

    // выбираем меньшие элементы, пока один из подмассивов не закончится
    while (i < right && j < end) {
        if ( cmp(array[i], array[j]) ) {
            buffer[bufIndex] = array[i];
            i++;
        } else {
            buffer[bufIndex] = array[j];
            j++;
        }
        bufIndex++;
    }

    // довставляем оставшийся массив
    // если остались элементы в левом/первом подмассиве...
    while (i < right) {
        buffer[bufIndex] = array[i];
        i++;
        bufIndex++;
    }
    // ...или втором/правом
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

    // разбиение на подмассивы 2^k
    for (int k = 1; k < size; k *= 2 ) {
        // определение границ подмассивов
        for (int left = 0; left + k < size; left += k * 2 ) {
            right = left + k;
            rightEnd = right + k;

            // не выходим за границы подмассива
            if (rightEnd > size) {
                rightEnd = size;
            }

            Merge(array, buffer, left, right, rightEnd, cmp);
        }
    }
}

// ------------------------------------- TASK -----------------------------------------
template <typename T>
void createArrayOfPaintedLines(Array<T> &array, Array<T> &paintedArray, int size) {
    int height = 0;

    for (int i = 0; i < size; i++) {
        if (array[i].delta == 1) {
            height++;
            // если это начало закрашенного отрезка - добавим координату
            if (height == 1) {
                paintedArray.insert(array[i]);
            }

        }
        if (array[i].delta == -1) {
            height--;
            // и добавим место, где закрашенная часть закончилась
            if (height == 0) {
                paintedArray.insert(array[i]);
            }
        }
    }
}

template <typename T>
int countLengthOfPaintedPart(Array<T> &paintedArray, int size) {
    int length = 0;

    if (!paintedArray.isEmpty()) {
        // рассматриваем по паре точек
        for (int i = 0; i < size; i += 2) {
            length += paintedArray[i + 1].coord - paintedArray[i].coord;
        }
    }

    return length;
}

int main() {
    int n;
    std::cin >> n;

    Array<Point> array;
    Array<Point> paintedArray;
    Point point{};

    int length = 0;

    // для каждого отрезка 2 точки (начало-конец) => 2n
    for (int i = 0; i < 2 * n; i++) {
        std::cin >> point;
        array.insert(point);
    }

    MergeSort(array, array.getSize(), cmpPoint);

    createArrayOfPaintedLines(array, paintedArray, array.getSize());

    length = countLengthOfPaintedPart(paintedArray, paintedArray.getSize());

    std::cout << length;

    return 0;
}