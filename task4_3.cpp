//
// Created by shysa on 11.03.2020.
//

#include <iostream>
#include <functional>

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
template <class T, typename comparator = std::less<T>>
class Heap {
public:
    Heap<T, comparator>(): array(nullptr), size (0), capacity(0) {}
    explicit Heap<T, comparator>(comparator compareFunc): array(nullptr), size (0), capacity(0), cmp(compareFunc) {}
    ~Heap() {
        delete[] array;
    }

    void insert(T& element);
    T extract();
    const T& peek() const;
    bool isEmpty() const {
        return size == 0;
    }
    int getSize() {
        return size;
    }

private:
    T * array;
    int size;
    int capacity;
    comparator cmp;

    void siftDown(int index);
    void siftUp(int index);
    void resize();
};

template <typename T, typename comparator>
const T& Heap<T, comparator>::peek() const {
    if (!isEmpty()) {
        return array[0];
    }
}

template <typename T, typename comparator>
T Heap<T, comparator>::extract() {
    if (!isEmpty()) {
        T result = array[0];
        array[0] = array[size - 1];

        size--;

        if (!isEmpty()) {
            siftDown(0);
        }
        return result;
    }
}

template <typename T, typename comparator>
void Heap<T, comparator>::resize() {
    int newCapacity = (capacity > 0)? capacity * 2 : 10;
    T *newArray = new T[newCapacity];

    for (int i = 0; i < size; i++) {
        newArray[i] = array[i];
    }
    delete[] array;

    array = newArray;
    capacity = newCapacity;

}

template <typename T, typename comparator>
void Heap<T, comparator>::insert(T& element) {
    if (size == capacity) {
        resize();
    }

    array[size] = element;
    size++;
    siftUp(size - 1);
}

template <typename T, typename comparator>
void Heap<T, comparator>::siftDown(int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    //меньший потомок
    int siftedSon = index;

    if ( left < size && !(cmp(array[left], array[index])) ) {
        siftedSon = left;
    }
    if ( right < size && (cmp(array[siftedSon], array[right])) ) {
        siftedSon = right;
    }

    if (siftedSon != index) {
        std::swap(array[index], array[siftedSon]);
        siftDown(siftedSon);
    }
}

template <typename T, typename comparator>
void Heap<T, comparator>::siftUp(int index) {
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

    Heap<Train, std::function<bool(const Train &l, const Train &r)>> heap(cmpTrainDeparture);
    Train train{};

    for (int i = 0; i < n; i++) {
        std::cin >> train.arrival >> train.departure;

        if (!heap.isEmpty()) {
            // извлекаем всех, кто уже уехал к моменту прибытия текущей электрички
            while (cmpArrivalAndDepartureTrains(train, heap.peek()) && !heap.isEmpty()) {
                heap.extract();
            }
        }
        heap.insert(train);

        if (heap.getSize() > minDeadlocks) {
            minDeadlocks = heap.getSize();
        }
    }

    return minDeadlocks;
}


int main() {
    int n = 0;
    std::cin >> n;

    int minDeadlocks = CountDeadlocks(n);

    std::cout << minDeadlocks;

    return 0;
}