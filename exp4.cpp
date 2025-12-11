#include "MySTL/Vector.h"
#include "MySTL/list.h"
#include "MySTL/Stack.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
using namespace std;

// 生成随机数组（范围：[minVal, maxVal]，长度：n）
Vector<int> generateRandomArray(int n, int minVal = 0, int maxVal = 10000) {
    Vector<int> arr;
    srand((unsigned int)time(nullptr)); // 随机种子
    for (int i = 0; i < n; i++) {
        int val = minVal + rand() % (maxVal - minVal + 1);
        arr.push_back(val);
    }
    return arr;
}

// 复制数组（避免原数组被修改）
template <typename T>
Vector<T> copyArray(const Vector<T>& arr) {
    Vector<T> res;
    for (int i = 0; i < arr.size(); i++) {
        res.push_back(arr[i]);
    }
    return res;
}

// 打印数组前 10 个元素（避免大数据量输出冗余）
template <typename T>
void printArray(const Vector<T>& arr) {
    cout << "数组前 10 个元素：";
    int len = min(arr.size(), 10);
    for (int i = 0; i < len; i++) {
        cout << arr[i] << " ";
    }
    cout << (arr.size() > 10 ? "..." : "") << endl;
}

// 1. 冒泡排序（稳定，O(n2)）
template <typename T>
void bubbleSort(Vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false; // 优化：无交换则提前退出
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// 2. 选择排序（不稳定，O(n2)）
template <typename T>
void selectSort(Vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        // 找 [i, n) 中的最小值索引
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        swap(arr[i], arr[minIdx]);
    }
}

// 3. 插入排序（稳定，O(n2)）
template <typename T>
void insertSort(Vector<T>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        T temp = arr[i];
        int j = i - 1;
        // 移动比 temp 大的元素
        while (j >= 0 && arr[j] > temp) {
            arr.replace(j + 1, arr[j]);
            j--;
        }
        arr.replace(j + 1, temp);
    }
}

// 4. 快速排序（不稳定，O(n log n)）
template <typename T>
int partition(Vector<T>& arr, int low, int high) {
    T pivot = arr[high]; // 基准元素（选末尾）
    int i = low - 1;     // 小于基准的区域边界
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]); // 基准元素归位
    return i + 1;
}

template <typename T>
void quickSortHelper(Vector<T>& arr, int low, int high) {
    if (low < high) {
        int pivotIdx = partition(arr, low, high);
        quickSortHelper(arr, low, pivotIdx - 1);  // 左区间排序
        quickSortHelper(arr, pivotIdx + 1, high); // 右区间排序
    }
}

template <typename T>
void quickSort(Vector<T>& arr) {
    quickSortHelper(arr, 0, arr.size() - 1);
}

// 5. 归并排序（稳定，O(n log n)）
template <typename T>
void merge(Vector<T>& arr, int low, int mid, int high) {
    Vector<T> temp;
    int i = low, j = mid + 1;
    // 合并两个有序区间
    while (i <= mid && j <= high) {
        if (arr[i] <= arr[j]) {
            temp.push_back(arr[i++]);
        } else {
            temp.push_back(arr[j++]);
        }
    }
    // 复制剩余元素
    while (i <= mid) temp.push_back(arr[i++]);
    while (j <= high) temp.push_back(arr[j++]);
    // 复制回原数组
    for (int k = 0; k < temp.size(); k++) {
        arr.replace(low + k, temp[k]);
    }
}

template <typename T>
void mergeSortHelper(Vector<T>& arr, int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        mergeSortHelper(arr, low, mid);    // 左区间排序
        mergeSortHelper(arr, mid + 1, high); // 右区间排序
        merge(arr, low, mid, high);        // 合并
    }
}

template <typename T>
void mergeSort(Vector<T>& arr) {
    mergeSortHelper(arr, 0, arr.size() - 1);
}

// 6. 堆排序（不稳定，O(n log n)）
template <typename T>
void heapify(Vector<T>& arr, int n, int i) {
    int largest = i;       // 根节点
    int left = 2 * i + 1;  // 左子节点
    int right = 2 * i + 2; // 右子节点

    // 找出根、左、右中的最大值
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    // 最大值不是根节点则交换，并递归调整
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

template <typename T>
void heapSort(Vector<T>& arr) {
    int n = arr.size();
    // 构建大顶堆（从最后一个非叶子节点开始）
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    // 堆排序：逐个提取堆顶元素
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]); // 堆顶（最大值）移到末尾
        heapify(arr, i, 0);   // 调整剩余元素为大顶堆
    }
}

// 查找算法 1：顺序查找（O(n)）
template <typename T>
int sequentialSearch(const Vector<T>& arr, const T& target) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == target) {
            return i; // 找到返回索引
        }
    }
    return -1; // 未找到返回 -1
}

// 查找算法 2：二分查找（O(log n)，要求数组有序）
template <typename T>
int binarySearch(const Vector<T>& arr, const T& target) {
    int low = 0, high = arr.size() - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] == target) {
            return mid; // 找到返回索引
        } else if (arr[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1; // 未找到返回 -1
}

// 测试排序算法性能（返回耗时，单位：毫秒）
template <typename T>
double testSortTime(void (*sortFunc)(Vector<T>&), Vector<T> arr, string sortName) {
    clock_t start = clock();
    sortFunc(arr); // 执行排序
    clock_t end = clock();
    double time = 1000.0 * (end - start) / CLOCKS_PER_SEC;
    cout << left << setw(12) << sortName << "耗时：" << fixed << setprecision(3) << time << " ms" << endl;
    return time;
}

// 验证数组是否有序
template <typename T>
bool isSorted(const Vector<T>& arr) {
    for (int i = 0; i < arr.size() - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

int main() {
    cout << "===== 排序与查找算法综合实验（exp4）=====" << endl;

    // 1. 实验配置
    int n = 5000; // 数组长度（可修改：如 10000、50000）
    cout << "实验配置：数组长度 = " << n << "，元素范围 = [0, 10000]" << endl;

    // 2. 生成随机数组
    Vector<int> arr = generateRandomArray(n);
    cout << "\n原始数组：";
    printArray(arr);

    // 3. 测试 6 种排序算法
    cout << "\n=== 排序算法性能对比 ===" << endl;
    testSortTime(bubbleSort<int>, copyArray(arr), "冒泡排序");
    testSortTime(selectSort<int>, copyArray(arr), "选择排序");
    testSortTime(insertSort<int>, copyArray(arr), "插入排序");
    testSortTime(quickSort<int>, copyArray(arr), "快速排序");
    testSortTime(mergeSort<int>, copyArray(arr), "归并排序");
    testSortTime(heapSort<int>, copyArray(arr), "堆排序");

    // 4. 验证排序结果（以快速排序为例）
    Vector<int> sortedArr = copyArray(arr);
    quickSort(sortedArr);
    cout << "\n排序结果验证：" << (isSorted(sortedArr) ? "? 数组有序" : "? 数组无序") << endl;
    cout << "排序后数组：";
    printArray(sortedArr);

    // 5. 查找算法测试
    cout << "\n=== 查找算法测试 ===" << endl;
    int target = sortedArr[n / 2]; // 查找中间元素（确保存在）
    cout << "查找目标：" << target << endl;

    // 顺序查找
    clock_t seqStart = clock();
    int seqIdx = sequentialSearch(sortedArr, target);
    clock_t seqEnd = clock();
    double seqTime = 1000.0 * (seqEnd - seqStart) / CLOCKS_PER_SEC;
    cout << "顺序查找：索引 = " << seqIdx << "，耗时 = " << fixed << setprecision(6) << seqTime << " ms" << endl;

    // 二分查找（有序数组）
    clock_t binStart = clock();
    int binIdx = binarySearch(sortedArr, target);
    clock_t binEnd = clock();
    double binTime = 1000.0 * (binEnd - binStart) / CLOCKS_PER_SEC;
    cout << "二分查找：索引 = " << binIdx << "，耗时 = " << fixed << setprecision(6) << binTime << " ms" << endl;

    // 查找不存在的元素
    int invalidTarget = 10001;
    int invalidSeqIdx = sequentialSearch(sortedArr, invalidTarget);
    int invalidBinIdx = binarySearch(sortedArr, invalidTarget);
    cout << "\n查找不存在的目标（" << invalidTarget << "）：" << endl;
    cout << "顺序查找结果：" << (invalidSeqIdx == -1 ? "未找到" : to_string(invalidSeqIdx)) << endl;
    cout << "二分查找结果：" << (invalidBinIdx == -1 ? "未找到" : to_string(invalidBinIdx)) << endl;

    cout << "\n===== 实验结束 =====" << endl;
    return 0;
}

