#include <iostream>
#define WORD 63

bool bitValueOnIndex(unsigned long long num, int index) {
    // умножение на маску
    return (num >> index) & 1;
}

void binaryMSD(unsigned long long *array, int left, int right, int bit) {
    int i = left;
    int j = right;

    if (right <= left || bit < 0) {
        return;
    }

    while (i != j) {
        // идем с разных концов массива
        // как только нашли элементы массива не на своих местах - меняем их
        while ( bitValueOnIndex(array[i], bit) == 0 && (i < j) ) {
            i++;
        }
        while ( bitValueOnIndex(array[j], bit) == 1 && (i < j) ) {
            j--;
        }
        std::swap(array[i], array[j]);
    }
    // j указывает на начало подмассива с 1
    if ( bitValueOnIndex(array[right], bit) == 0 ) {
        j++;
    }

    binaryMSD(array, left, j - 1, bit - 1);
    binaryMSD(array, j, right, bit - 1);
}

int main() {
    int n;
    std::cin >> n;

    auto *array = new unsigned long long[n];
    for (int i = 0; i < n; i++) {
        std::cin >> array[i];
    }

    binaryMSD(array, 0, n - 1, WORD);

    for (int i = 0; i < n; i++) {
        std::cout << array[i] << " ";
    }

    delete[] array;

    return 0;
}