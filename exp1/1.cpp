#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>

using namespace std;

// 复数类定义
class Complex {
private:
    double real;  // 实部
    double imag;  // 虚部

public:
    // 构造函数
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    // 获取实部
    double getReal() const { return real; }

    // 获取虚部
    double getImag() const { return imag; }

    // 设置实部
    void setReal(double r) { real = r; }

    // 设置虚部
    void setImag(double i) { imag = i; }

    // 计算复数的模
    double modulus() const { return sqrt(real * real + imag * imag); }

    // 重载相等运算符（实部和虚部均相同才相等）
    bool operator==(const Complex& other) const {
        return (real == other.real) && (imag == other.imag);
    }

    // 重载不等运算符
    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }

    // 重载输出运算符
    friend ostream& operator<<(ostream& os, const Complex& c) {
        os << "(" << c.real << ", " << c.imag << ")";
        return os;
    }
};

// 生成随机复数向量
vector<Complex> generateRandomComplexVector(int size, double minVal, double maxVal) {
    vector<Complex> vec;
    srand(time(0));  // 初始化随机数种子

    for (int i = 0; i < size; ++i) {
        double real = minVal + (maxVal - minVal) * rand() / RAND_MAX;
        double imag = minVal + (maxVal - minVal) * rand() / RAND_MAX;
        vec.push_back(Complex(real, imag));
    }
    return vec;
}

// 向量置乱
void shuffleVector(vector<Complex>& vec) {
    srand(time(0));
    for (int i = vec.size() - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(vec[i], vec[j]);
    }
}

// 查找元素（返回第一个匹配元素的索引，未找到返回-1）
int findElement(const vector<Complex>& vec, const Complex& target) {
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] == target) {
            return i;
        }
    }
    return -1;
}

// 插入元素到指定位置
void insertElement(vector<Complex>& vec, int pos, const Complex& elem) {
    if (pos >= 0 && pos <= vec.size()) {
        vec.insert(vec.begin() + pos, elem);
    }
}

// 删除指定位置的元素
void deleteElement(vector<Complex>& vec, int pos) {
    if (pos >= 0 && pos < vec.size()) {
        vec.erase(vec.begin() + pos);
    }
}

// 向量唯一化（删除重复元素）
void uniqueVector(vector<Complex>& vec) {
    vector<Complex> temp;
    for (const auto& elem : vec) {
        if (findElement(temp, elem) == -1) {
            temp.push_back(elem);
        }
    }
    vec = temp;
}

// 起泡排序（以模为基准，模相同则以实部为基准）
void bubbleSort(vector<Complex>& vec) {
    int n = vec.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (vec[j].modulus() > vec[j + 1].modulus() ||
                (vec[j].modulus() == vec[j + 1].modulus() && vec[j].getReal() > vec[j + 1].getReal())) {
                swap(vec[j], vec[j + 1]);
            }
        }
    }
}

// 归并排序辅助函数
void merge(vector<Complex>& vec, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Complex> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = vec[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = vec[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].modulus() < R[j].modulus() ||
            (L[i].modulus() == R[j].modulus() && L[i].getReal() <= R[j].getReal())) {
            vec[k] = L[i];
            i++;
        }
        else {
            vec[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        vec[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        vec[k] = R[j];
        j++;
        k++;
    }
}

// 归并排序主函数
void mergeSort(vector<Complex>& vec, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(vec, left, mid);
        mergeSort(vec, mid + 1, right);

        merge(vec, left, mid, right);
    }
}

// 区间查找（模介于[m1, m2)的所有元素）
vector<Complex> rangeSearch(const vector<Complex>& sortedVec, double m1, double m2) {
    vector<Complex> result;
    for (const auto& elem : sortedVec) {
        double mod = elem.modulus();
        if (mod >= m1 && mod < m2) {
            result.push_back(elem);
        }
    }
    return result;
}

// 打印向量
void printVector(const vector<Complex>& vec, const string& msg = "") {
    if (!msg.empty()) {
        cout << msg << endl;
    }
    for (const auto& elem : vec) {
        cout << elem << " ";
    }
    cout << endl << "向量大小: " << vec.size() << endl << endl;
}
