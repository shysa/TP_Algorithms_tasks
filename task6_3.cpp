#include <iostream>
#include <random>

 // 6_3. Реализуйте стратегию выбора опорного элемента “случайный элемент”.
 // Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.

// возвращает индекс случайного элемента
int setPivotRandom(int left, int right) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(left, right);

    return distribution(generator);
}

// возвращает индекс элемента, на который тот встанет после сортировки
template <typename T, typename comparator>
T partition(T *array, int left, int right, comparator cmp = comparator()) {
    // берем рандомный элемент в качестве пивота и перекидываем в конец
    int index = setPivotRandom(left, right);
    std::swap(array[index], array[right]);

    int i = 0;
    int j = 0;

    // разделяем на подмассивы [ <= ][ > ]<pivot>
    while (j < right) {
        if ( cmp(array[j], array[right]) ) {
            j++;
        } else {
            std::swap(array[i], array[j]);
            i++;
            j++;
        }
    }
    std::swap(array[i], array[right]);

    return i;
}

template <typename T, typename comparator = std::greater<T>>
T searchKthStatistic(T *array, int left, int right, int indexSearch, comparator cmp = comparator()) {
    int pivotPosition = -1;

    int leftIndex = left;
    int rightIndex = right;

    while (pivotPosition != indexSearch) {
        pivotPosition = partition(array, leftIndex, rightIndex, cmp);

        // если искомая позиция k больше позиции пивота, то ищем в правой части массива
        if (pivotPosition < indexSearch) {
            leftIndex = pivotPosition + 1;
        } else {
            // или в левой
            rightIndex = pivotPosition - 1;
        }
    }

    return array[pivotPosition];
}

int main() {
    int n;
    int k;
    std::cin >> n >> k;

    int elementSearch;

    auto *array = new int[n];
    for (int i = 0; i < n; i++) {
        std::cin >> array[i];
    }

    elementSearch = searchKthStatistic(array, 0, n - 1, k);
    std::cout << elementSearch;

    delete[] array;

    return 0;
}