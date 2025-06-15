#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <cassert>

// Partition function for Quickselect
template <typename T>
int partition(std::vector<T>& arr, int left, int right, int pivotIndex) {
    //std::cerr << "Partition" << arr << " " << left << " " << right << " " << pivotIndex << std::endl;
    T pivotValue = arr[pivotIndex];
    std::swap(arr[pivotIndex], arr[right]); // Move pivot to the end
    int storeIndex = left;

    for (int i = left; i < right; ++i) {
        //std::cerr << "Comparing arr["<<i<<"] "  << arr[i] << " with " << pivotValue << std::endl;
        if (arr[i] < pivotValue) {
            //std::cerr << "Swapping " << i << " with " << storeIndex << std::endl;
            std::swap(arr[i], arr[storeIndex]);
            ++storeIndex;
            //std::cerr << "Array after swap: " << arr << std::endl;
        }
    }
    std::swap(arr[storeIndex], arr[right]); // Move pivot to its final place
    assert(storeIndex >= left && storeIndex <= right);
    assert(arr[storeIndex] == pivotValue); // Check that the pivot is in the right place
    return storeIndex;
}

template <typename T>
int choosePivot(const std::vector<T>& arr, int left, int right, int tries=3) {
    std::vector<std::pair<T, int>> candidates(tries);
    for (int i = 0; i < tries; i++) {
        int pivot = left + (std::rand() % (right - left + 1)); // Random pivot
        candidates[i] = {arr[pivot], pivot};
    }
    std::sort(candidates.begin(), candidates.end());
    return candidates[tries / 2].second; // Return the median of the candidates
}

// Quickselect function
template <typename T>
T quickselect(std::vector<T>& arr, int left, int right, int k) {
    if (left == right) {
        return arr[left]; // Only one element
    }

    int pivotIndex = choosePivot(arr, left, right);
    pivotIndex = partition(arr, left, right, pivotIndex);

    if (k == pivotIndex) {
        return arr[k]; // Found the k-th smallest element
    } else if (k < pivotIndex) {
        return quickselect(arr, left, pivotIndex - 1, k);
    } else {
        return quickselect(arr, pivotIndex + 1, right, k);
    }
}

template <typename T>
T quickselect(std::vector<T>& arr, int k) {
    return quickselect(arr, 0, arr.size() - 1, k);
}


template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    os << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i < vec.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

int main(int argc, char* argv[]) {
    std::srand(time(0));
    std::vector<float> arr(atoi(argv[1]));
    std::generate(arr.begin(), arr.end(), []() { return static_cast<float>(std::rand()) / RAND_MAX; });
    std::cout << "pre-median: " << arr << std::endl;
    float result = quickselect(arr, arr.size() / 2);
    std::cout << "post-median: " << arr << std::endl;
    std::cout << "median: " << result << std::endl;
    

}