#pragma once

#include <iostream>
#include <algorithm>  // 用于 std::max
#include <stdexcept>  // 用于异常处理

template <typename T>
class Vector {
private:
    T* _elem;   // 数据存储指针
    int _size;  // 当前元素数量
    int _cap;   // 容量

    // 扩容函数
    void expand() {
        if (_size < _cap) return;
        // 确保初始容量至少为1，容量翻倍
        int new_cap = std::max(1, _cap) * 2;
        T* new_elem = new T[new_cap];
        // 复制旧元素
        for (int i = 0; i < _size; ++i) {
            new_elem[i] = _elem[i];
        }
        // 释放旧内存
        delete[] _elem;
        _elem = new_elem;
        _cap = new_cap;
    }

public:
    // 构造函数
    Vector(int capacity = 10) : _size(0), _cap(capacity) {
        if (_cap < 1) _cap = 1;  // 容量至少为1
        _elem = new T[_cap];
    }

    // 复制构造函数
    Vector(const Vector<T>& other) : _size(other._size), _cap(other._cap) {
        _elem = new T[_cap];
        for (int i = 0; i < _size; ++i) {
            _elem[i] = other._elem[i];
        }
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
        // 元素后移
        for (int i = _size; i > pos; --i) {
            _elem[i] = _elem[i - 1];
        }
        _elem[pos] = val;
        ++_size;
    }

    // 删除元素
    T remove(int pos) {
        if (pos < 0 || pos >= _size) {
            throw std::out_of_range("Remove position out of range");
        }
        T res = _elem[pos];
        // 元素前移
        for (int i = pos; i < _size - 1; ++i) {
            _elem[i] = _elem[i + 1];
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
    void sort() {
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

    // 打印元素
    void print() const {
        std::cout << "Vector(" << _size << "/" << _cap << "): ";
        for (int i = 0; i < _size; ++i) {
            std::cout << _elem[i] << " ";
        }
        std::cout << std::endl;
    }
};