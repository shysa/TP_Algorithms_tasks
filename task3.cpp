#include <iostream>
//
// Created by shysa on 12.03.2020.
//
template <typename T>
class Queue {
public:
    Queue(): data(nullptr), head(0), tail(0), size(0), capacity(0) { Resize(); }
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
            for (unsigned int i = head; i < size || i + 1 != tail; i++) {
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
        printf("[%d:%d:%d]==========\n", head, tail, size);
        for(int i = head; i < size; i++) {
            if(i == tail) { // элемент следующий после последнего
                break;
            }
            printf("[%d] => %d\n", i, data[i]);
            if(i == size - 1) {
                i = -1;
            }
        }
        printf("/==========\n");
    }

    bool IsEmpty() {
        return head == tail;
    }

private:
    T *data;
    int head;
    int tail;
    unsigned int size;
    unsigned int capacity;
};


int main() {
    Queue <int>queue;

    int NumCommands = 0;
    std::cin >> NumCommands;

    int CommandCode = 0;
    int CommandValue = 0;

    for (int i = 0; i < NumCommands; i++) {
        std::cin >> CommandCode >> CommandValue;

        switch (CommandCode) {
            case 2:
                if (queue.IsEmpty()) {
                    CommandValue = -1;
                }
        }
    }



    return 0;
}