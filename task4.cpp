//
// Created by shysa on 11.03.2020.
//

#include <iostream>

// ----------------------- TRAIN -----------------------------
struct Train {
    int arrival;
    int departure;

    friend std::ostream& operator<<(std::ostream &os, const  Train& train) {
        return std::cout << train.arrival << " : " << train.departure << std::endl;
    }

};
bool cmpTrainDeparture(const Train &l, const Train &r) {
    return l.departure > r.departure;
}
bool cmpArrivalAndDepartureTrains(const Train &l, const Train &r) {
    return l.arrival > r.departure;
}

// -------------------- HEAP CLASS ---------------------------
template <class T>
class Heap {
public:
    Heap<T>(): array(nullptr), size (0), capacity(0) {}
    ~Heap() {
        delete[] array;
    }

    void insert(T element);
    T extractMin();
    T peekMin() const;
    bool isEmpty() const {
        return (size == 0);
    }
    int getSize() {
        return size;
    }

private:
    T * array;
    int size;
    int capacity;

    void siftDown(int index, bool (*cmp)(const Train&, const Train&));
    void siftUp(int index, bool (*cmp)(const Train&, const Train&));
    void resize();
};

template <typename T>
T Heap<T>::peekMin() const {
    if (!isEmpty()) {
        return array[0];
    }
}

template <typename T>
T Heap<T>::extractMin() {
    if (!isEmpty()) {
        T result = array[0];
        array[0] = array[size - 1];

        size--;

        if (!isEmpty()) {
            siftDown(0, cmpTrainDeparture);
        }
        return result;
    }
}

template <typename T>
void Heap<T>::resize() {
    int newCapacity = (capacity > 0)? capacity * 2 : 2;
    T *newArray = new T[newCapacity];

    for (int i = 0; i < size; i++) {
        newArray[i] = array[i];
    }
    delete[] array;

    array = newArray;
    capacity = newCapacity;

}

template <typename T>
void Heap<T>::insert(T element) {
    if (size == capacity) {
        resize();
    }

    array[size] = element;
    size++;
    siftUp(size - 1, cmpTrainDeparture);
}

template <typename T>
void Heap<T>::siftDown(int index, bool (*cmp)(const Train&, const Train&)) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    // больший потомок
    int largest = index;
    if ( left < size && !(cmp(array[left], array[right])) ) {
        largest = left;
    }
    if ( right < size && (cmp(array[left], array[right])) ) {
        largest = right;
    }

    if (largest != index) {
        std::swap(array[index], array[largest]);
        siftDown(largest, cmp);
    }
}

template <typename T>
void Heap<T>::siftUp(int index, bool (*cmp)(const Train&, const Train&)) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (cmp(array[index], array[parent])) {
            return;
        }
        std::swap(array[index], array[parent]);
        index = parent;
    }
}

// -------------------- TASK ---------------------------
int CountDeadlocks(int n) {
    int minDeadlocks = 0;

    Heap<Train> heap;
    auto * train = new Train;

    for (int i = 0; i < n; i++) {
        std::cin >> train->arrival >> train->departure;

        if (!heap.isEmpty()) {
            // извлекаем всех, кто уже уехал к моменту прибытия текущей электрички
            while (cmpArrivalAndDepartureTrains(*train, heap.peekMin())) {
                heap.extractMin();
            }
        }
        heap.insert(*train);

        if (heap.getSize() > minDeadlocks) {
            minDeadlocks = heap.getSize();
        }
    }

    delete train;
    return minDeadlocks;
}


int main() {
    int n = 0;
    std::cin >> n;

    int minDeadlocks = CountDeadlocks(n);

    std::cout << minDeadlocks;

    return 0;
}