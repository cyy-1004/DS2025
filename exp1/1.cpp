#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include "Vector.h"

using namespace std;

// 复数类定义
class Complex {
private:
    double real;  // 实部
    double imag;  // 虚部

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    double getReal() const { return real; }
    double getImag() const { return imag; }

    void setReal(double r) { real = r; }
    void setImag(double i) { imag = i; }

    double modulus() const { return sqrt(real * real + imag * imag); }

    bool operator==(const Complex& other) const {
        return (real == other.real) && (imag == other.imag);
    }

    friend ostream& operator<<(ostream& os, const Complex& c) {
        os << "(" << c.real << ", " << c.imag << ")";
        return os;
    }
};

// 生成随机复数向量
Vector<Complex> generateRandomComplexVector(int size, double minVal, double maxVal) {
    Vector<Complex> vec;
    srand(time(0));
    for (int i = 0; i < size; ++i) {
        double real = minVal + (maxVal - minVal) * rand() / RAND_MAX;
        double imag = minVal + (maxVal - minVal) * rand() / RAND_MAX;
        vec.push_back(Complex(real, imag));
    }
    return vec;
}

// 向量置乱
void shuffleVector(Vector<Complex>& vec) {
    srand(time(0));
    for (int i = vec.size() - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(vec[i], vec[j]);
    }
}

// 向量唯一化
void uniqueVector(Vector<Complex>& vec) {
    Vector<Complex> temp;
    for (int i = 0; i < vec.size(); ++i) {
        bool isUnique = true;
        for (int j = 0; j < temp.size(); ++j) {
            if (vec[i] == temp[j]) {
                isUnique = false;
                break;
            }
        }
        if (isUnique) {
            temp.push_back(vec[i]);
        }
    }
    vec = temp;
}

// 起泡排序（模为基准，模相同实部为基准）
void bubbleSort(Vector<Complex>& vec) {
    int n = vec.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            double modJ = vec[j].modulus();
            double modJ1 = vec[j + 1].modulus();
            if (modJ > modJ1 || (modJ == modJ1 && vec[j].getReal() > vec[j + 1].getReal())) {
                swap(vec[j], vec[j + 1]);
            }
        }
    }
}

// 归并排序辅助函数
void merge(Vector<Complex>& vec, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Vector<Complex> L, R;
    for (int i = 0; i < n1; ++i)
        L.push_back(vec[left + i]);
    for (int j = 0; j < n2; ++j)
        R.push_back(vec[mid + 1 + j]);

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        double modL = L[i].modulus();
        double modR = R[j].modulus();
        if (modL < modR || (modL == modR && L[i].getReal() <= R[j].getReal())) {
            vec[k] = L[i];
            i++;
        } else {
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
void mergeSort(Vector<Complex>& vec, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(vec, left, mid);
        mergeSort(vec, mid + 1, right);
        merge(vec, left, mid, right);
    }
}

// 区间查找（模介于[m1, m2)的元素）
Vector<Complex> rangeSearch(const Vector<Complex>& sortedVec, double m1, double m2) {
    Vector<Complex> result;
    for (int i = 0; i < sortedVec.size(); ++i) {
        double mod = sortedVec[i].modulus();
        if (mod >= m1 && mod < m2) {
            result.push_back(sortedVec[i]);
        }
    }
    return result;
}

// 打印向量
void printVector(const Vector<Complex>& vec, const string& msg = "") {
    if (!msg.empty()) {
        cout << msg << endl;
    }
    for (int i = 0; i < vec.size(); ++i) {
        cout << vec[i] << " ";
    }
    cout << endl << "向量大小: " << vec.size() << endl << endl;
}

