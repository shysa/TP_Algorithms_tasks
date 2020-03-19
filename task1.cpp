//
// Created by shysa on 06.03.2020.
//

#include <iostream>

struct Human {
    int index;
    Human* next;
    Human* prev;
};

class Queue {
    Human *head, *tail;
public:
    Queue(): head(nullptr), tail(nullptr){};
    ~Queue();

    void ShowQueue();
    void AddHuman(int index);
    void StartKilling(int k);
    Human* Kill(Human* current);
};

Queue::~Queue() {
    head->prev = nullptr;
    tail->next = nullptr;
    while (head) {
        tail = head->next;
        delete head;
        head = tail;
    }
}

void Queue::ShowQueue() {
    Human *current = head;

    do {
        std::cout << current->index << " ";
        current = current->next;
    } while (current != head);
}

void Queue::AddHuman(int index) {
    auto current = new Human;
    current->index = index;

    if (head != nullptr) {
        current->prev = tail;
        tail->next = current;
        tail = current;
        tail->next = head;
        head->prev = tail;
    } else {
        head = tail = current;
        current->next = head;
        current->prev = tail;
    }
}

void Queue::StartKilling(int k) {
    Human *current = head;
    int count = 0;

    do {
        count+=1;
        count%=k;
        if (count == 0) {
            current = Kill(current);

        }
        current = current->next;
    } while (head != tail);
}

Human* Queue::Kill(Human *current) {
    Human *tmp = nullptr;

    current->prev->next = current->next;
    current->next->prev = current->prev;
    tmp = current->prev;
    if (tail == current) {
        tail = tmp;
    }
    if (head == current) {
        head = tmp->next;
    }
    delete current;
    return tmp;
}


int main() {
    int N;
    int k;

    std::cin >> N >> k;

    Queue queue;

    for (int i = 1; i < N + 1; i++) {
        queue.AddHuman(i);
    }

    queue.StartKilling(k);
    queue.ShowQueue();

    return 0;
}

