#include <iostream>
#include <vector>
#include <algorithm>

int partition(std::vector<int>& arr, int left, int right, int pivot) {
    // Mou el pivot al final
    for (int i = left; i <= right; ++i) {
        if (arr[i] == pivot) {
            std::swap(arr[i], arr[right]);
            break;
        }
    }

    int storeIndex = left;
    for (int i = left; i < right; ++i) {
        if (arr[i] < pivot) {
            std::swap(arr[i], arr[storeIndex]);
            storeIndex++;
        }
    }
    std::swap(arr[storeIndex], arr[right]);
    return storeIndex;
}

int select(std::vector<int>& arr, int left, int right, int k) {
    if (left == right)
        return arr[left];

    // Divideix en grups de 5 i troba les medianes
    std::vector<int> medians;
    for (int i = left; i <= right; i += 5) {
        int subRight = std::min(i + 4, right);
        std::sort(arr.begin() + i, arr.begin() + subRight + 1);
        int median = arr[i + (subRight - i) / 2];
        medians.push_back(median);
    }

    // Troba la mediana de les medianes recursivament
    int pivot;
    if (medians.size() <= 5) {
        std::sort(medians.begin(), medians.end());
        pivot = medians[medians.size() / 2];
    } else {
        pivot = select(medians, 0, medians.size() - 1, medians.size() / 2);
    }

    // Partició amb el pivot
    int pivotIndex = partition(arr, left, right, pivot);

    int length = pivotIndex - left + 1;
    if (k == length)
        return arr[pivotIndex];
    else if (k < length)
        return select(arr, left, pivotIndex - 1, k);
    else
        return select(arr, pivotIndex + 1, right, k - length);
}

int medianOfMedians(std::vector<int>& arr) {
    int n = arr.size();
    int k = (n + 1) / 2;
    return select(arr, 0, n - 1, k);
}

// Exemple d'ús
int main() {
    std::vector<int> dades = {7, 2, 1, 6, 8, 5, 3, 4, 9};
    int mediana = medianOfMedians(dades);
    std::cout << "La mediana es: " << mediana << std::endl;
    return 0;
}
