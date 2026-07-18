#include <iostream>
#include <vector>

void quicksort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        int pi = i + 1;
        
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

int main() {
    std::vector<int> arr = {64, 34, 25, 12, 22, 11, 90};
    
    std::cout << "Original array: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";
    
    if (!arr.empty()) {
        quicksort(arr, 0, static_cast<int>(arr.size()) - 1);
    }
    
    std::cout << "Sorted array:   ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";
    
    return 0;
}
