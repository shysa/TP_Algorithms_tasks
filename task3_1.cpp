#include <iostream>
//
// Created by shysa on 11.03.2020.
//
template <typename T>
class Queue {
public:
    Queue(): data(nullptr), head(0), tail(0), size(0) { Resize(); }
    ~Queue() {
        if (data) {
            delete[] data;
        }
    }

    T PopFront() {
        if (head != tail) {
            T value = data[head];

            if (head == size - 1) {
                head = 0;
            } else {
                head++;
            }

            return value;
        }

        return -1;
    }

    void PushBack(T value) {
        if ((tail + 1) % size == head) {
            Resize();
            PushBack(value);
        } else {
            data[tail % size] = value;
            tail = (tail + 1) % size;
        }
    }

    void Resize() {
        unsigned int newSize = (size > 0)? size*2 : 2;
        T *newData = new T[newSize];

        if (head != tail) {
            unsigned int j = 0;
            for (unsigned int i = head; i < size; i++) {
                if (i == tail) {
                    break;
                }
                newData[j++] = data[i];

                if (i == size - 1){
                    i = -1;
                }
            }
            head = 0;
            tail = j;
        }

        if (data) {
            delete[] data;
        }

        data = newData;
        size = newSize;
    }

    void ShowQueue() {
        std::cout << "[head " << head << " : tail " << tail << " : size " << size << "]\n";
        for (int i = head; i < size; i++) {
            if (i == tail) {
                break;
            }
            std::cout << i << " : " << data[i] << std::endl;
            if (i == size - 1) {
                i = -1;
            }
        }
    }

    bool IsEmpty() {
        return head == tail;
    }

private:
    T *data;
    int head;
    int tail;
    unsigned int size;
};


int main() {
    Queue <int>queue;

    int n = 0;
    std::cin >> n;

    // a - код команды, b - значение
    int a = 0;
    int b = 0;

    bool isOK = true;

    for (int i = 0; i < n; i++) {
        std::cin >> a >> b;

        switch (a) {
            case 2:
                if (queue.IsEmpty()) {
                    b = -1;
                }
                isOK = (queue.PopFront() == b) && isOK;
                break;
            case 3:
                queue.PushBack(b);
                break;
            default:
                break;
        }
    }

    std::cout << (isOK ? "YES" : "NO");

    return 0;
}