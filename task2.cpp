//
// Created by shysa on 07.03.2020.
//

#include <iostream>

void FindK(const int* massive, const int n, const int reqNum, int& leftIndex, int& rightIndex) {
    int i = 0;

    while ((1 << i) < n && massive[1 << i] < reqNum) {
        i++;
    }
    leftIndex = (i == 0) ? 0 : 1 << (i - 1);
    rightIndex = ((1 << i) < n) ? 1 << i : n;
}

int BinarySearch(const int* massive, int leftIndex, int rightIndex, const int reqNum) {
    int first = leftIndex;
    int last = rightIndex;
    int middle;

    while(first < last) {
        middle = (first + last) / 2;
        if (massive[middle] >= reqNum) {
            last = middle;
        } else {
            first = middle + 1;
        }
    }

    if (massive[last] >= reqNum) {
        return last;
    } else {
        return rightIndex;
    }
}

void ReadMassive(int* massive, const int size) {
    for (int i = 0; i < size; i++) {
        std::cin >> massive[i];
    }
}

int main() {
    int n;
    int m;

    int leftIndex = 0;
    int rightIndex = 0;
    int reqIndex;

    std::cin >> n >> m;

    int *A = new int[n];
    int *B = new int[m];

    ReadMassive(A, n);
    ReadMassive(B, m);

    for (int i = 0; i < m; i++) {
        leftIndex = 0;
        rightIndex = n;

        FindK(A, n, B[i], leftIndex, rightIndex);
        reqIndex = BinarySearch(A, leftIndex, rightIndex, B[i]);

        std::cout << reqIndex << " ";
    }

    delete[] A;
    delete[] B;

    return 0;
}