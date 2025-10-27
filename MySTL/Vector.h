#pragma once
#include <iostream>
#include <algorithm>
#include <stdexcept>

template <typename T>
class Vector {
private:
    T* _elem;   // 数据存储指针
    int _size;  // 当前元素数量
    int _cap;   // 容量

    // 扩容函数：确保容量足够
    void expand() {
        if (_size < _cap) return;
        int new_cap = (_cap == 0) ? 1 : _cap * 2;  // 初始容量为1，之后翻倍
        T* new_elem = new T[new_cap];
        
        // 复制元素（使用移动语义提高效率）
        for (int i = 0; i < _size; ++i) {
            new_elem[i] = std::move(_elem[i]);
        }
        
        delete[] _elem;
        _elem = new_elem;
        _cap = new_cap;
    }

public:
    // 构造函数
    Vector(int capacity = 10) : _size(0), _cap(capacity) {
        if (_cap < 1) _cap = 1;
        _elem = new T[_cap];
    }
    // 支持初始化列表构造（例如 Vector<int> v = {1,2,3};）
    Vector(std::initializer_list<T> init) : _size(init.size()), _cap(init.size()) {
    _elem = new T[_cap];
    int i = 0;
    for (const auto& val : init) {
        _elem[i++] = val;
    }
}

    // 复制构造函数
    Vector(const Vector<T>& other) : _size(other._size), _cap(other._cap) {
        _elem = new T[_cap];
        for (int i = 0; i < _size; ++i) {
            _elem[i] = other._elem[i];
        }
    }

    // 移动构造函数（新增，避免不必要的复制）
    Vector(Vector<T>&& other) noexcept 
        : _elem(other._elem), _size(other._size), _cap(other._cap) {
        other._elem = nullptr;
        other._size = 0;
        other._cap = 0;
    }

    // 赋值运算符
    Vector<T>& operator=(const Vector<T>& other) {
        if (this != &other) {
            delete[] _elem;
            _size = other._size;
            _cap = other._cap;
            _elem = new T[_cap];
            for (int i = 0; i < _size; ++i) {
                _elem[i] = other._elem[i];
            }
        }
        return *this;
    }

    // 移动赋值运算符（新增）
    Vector<T>& operator=(Vector<T>&& other) noexcept {
        if (this != &other) {
            delete[] _elem;
            _elem = other._elem;
            _size = other._size;
            _cap = other._cap;
            other._elem = nullptr;
            other._size = 0;
            other._cap = 0;
        }
        return *this;
    }

    // 析构函数
    ~Vector() {
        delete[] _elem;
    }

    // 基本属性
    int size() const { return _size; }
    int capacity() const { return _cap; }
    bool empty() const { return _size == 0; }

    // 元素访问
    T& operator[](int idx) {
        if (idx < 0 || idx >= _size) {
            throw std::out_of_range("Vector index out of range");
        }
        return _elem[idx];
    }

    const T& operator[](int idx) const {
        if (idx < 0 || idx >= _size) {
            throw std::out_of_range("Vector index out of range");
        }
        return _elem[idx];
    }

    // 尾部插入
    void push_back(const T& val) {
        expand();
        _elem[_size++] = val;
    }

    // 尾部插入（移动版本）
    void push_back(T&& val) {
        expand();
        _elem[_size++] = std::move(val);
    }

    // 尾部删除
    void pop_back() {
        if (empty()) {
            throw std::runtime_error("Cannot pop from empty Vector");
        }
        --_size;
    }

    // 插入元素
    void insert(int pos, const T& val) {
        if (pos < 0 || pos > _size) {
            throw std::out_of_range("Insert position out of range");
        }
        expand();
        for (int i = _size; i > pos; --i) {
            _elem[i] = std::move(_elem[i - 1]);  // 移动元素提高效率
        }
        _elem[pos] = val;
        ++_size;
    }

    // 删除元素
    T remove(int pos) {
        if (pos < 0 || pos >= _size) {
            throw std::out_of_range("Remove position out of range");
        }
        T res = std::move(_elem[pos]);  // 移动语义
        for (int i = pos; i < _size - 1; ++i) {
            _elem[i] = std::move(_elem[i + 1]);
        }
        --_size;
        return res;
    }

    // 查找元素
    int find(const T& val) const {
        for (int i = 0; i < _size; ++i) {
            if (_elem[i] == val) {
                return i;
            }
        }
        return -1;
    }

    // 选择排序
    void selection_sort() {
        for (int i = 0; i < _size - 1; ++i) {
            int min_idx = i;
            for (int j = i + 1; j < _size; ++j) {
                if (_elem[j] < _elem[min_idx]) {
                    min_idx = j;
                }
            }
            std::swap(_elem[i], _elem[min_idx]);
        }
    }

    // 归并排序（新增）
    void merge_sort() {
        if (_size <= 1) return;
        merge_sort_recursive(0, _size - 1);
    }

    // 打印元素
    void print() const {
        std::cout << "Vector(" << _size << "/" << _cap << "): ";
        for (int i = 0; i < _size; ++i) {
            std::cout << _elem[i] << " ";
        }
        std::cout << std::endl;
    }

    // 清空向量
    void clear() {
        _size = 0;  // 逻辑清空，不释放内存
    }

    // 预留容量
    void reserve(int new_cap) {
        if (new_cap > _cap) {
            T* new_elem = new T[new_cap];
            for (int i = 0; i < _size; ++i) {
                new_elem[i] = std::move(_elem[i]);
            }
            delete[] _elem;
            _elem = new_elem;
            _cap = new_cap;
        }
    }

private:
    // 归并排序递归辅助函数
    void merge_sort_recursive(int left, int right) {
        if (left >= right) return;
        int mid = left + (right - left) / 2;  // 避免溢出
        merge_sort_recursive(left, mid);
        merge_sort_recursive(mid + 1, right);
        merge(left, mid, right);
    }

    // 归并操作
    void merge(int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        // 临时存储
        T* L = new T[n1];
        T* R = new T[n2];

        // 复制数据
        for (int i = 0; i < n1; ++i)
            L[i] = std::move(_elem[left + i]);
        for (int j = 0; j < n2; ++j)
            R[j] = std::move(_elem[mid + 1 + j]);

        // 合并
        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                _elem[k++] = std::move(L[i++]);
            } else {
                _elem[k++] = std::move(R[j++]);
            }
        }

        // 处理剩余元素
        while (i < n1)
            _elem[k++] = std::move(L[i++]);
        while (j < n2)
            _elem[k++] = std::move(R[j++]);

        delete[] L;
        delete[] R;
    }
};
