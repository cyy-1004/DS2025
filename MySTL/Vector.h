#pragma once

#include <iostream>
#include <algorithm>  // ���� std::max
#include <stdexcept>  // �����쳣����

template <typename T>
class Vector {
private:
    T* _elem;   // ���ݴ洢ָ��
    int _size;  // ��ǰԪ������
    int _cap;   // ����

    // ���ݺ���
    void expand() {
        if (_size < _cap) return;
        // ȷ����ʼ��������Ϊ1����������
        int new_cap = std::max(1, _cap) * 2;
        T* new_elem = new T[new_cap];
        // ���ƾ�Ԫ��
        for (int i = 0; i < _size; ++i) {
            new_elem[i] = _elem[i];
        }
        // �ͷž��ڴ�
        delete[] _elem;
        _elem = new_elem;
        _cap = new_cap;
    }

public:
    // ���캯��
    Vector(int capacity = 10) : _size(0), _cap(capacity) {
        if (_cap < 1) _cap = 1;  // ��������Ϊ1
        _elem = new T[_cap];
    }

    // ���ƹ��캯��
    Vector(const Vector<T>& other) : _size(other._size), _cap(other._cap) {
        _elem = new T[_cap];
        for (int i = 0; i < _size; ++i) {
            _elem[i] = other._elem[i];
        }
    }

    // ��������
    ~Vector() {
        delete[] _elem;
    }

    // ��������
    int size() const { return _size; }
    int capacity() const { return _cap; }
    bool empty() const { return _size == 0; }

    // Ԫ�ط���
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

    // β������
    void push_back(const T& val) {
        expand();
        _elem[_size++] = val;
    }

    // β��ɾ��
    void pop_back() {
        if (empty()) {
            throw std::runtime_error("Cannot pop from empty Vector");
        }
        --_size;
    }

    // ����Ԫ��
    void insert(int pos, const T& val) {
        if (pos < 0 || pos > _size) {
            throw std::out_of_range("Insert position out of range");
        }
        expand();
        // Ԫ�غ���
        for (int i = _size; i > pos; --i) {
            _elem[i] = _elem[i - 1];
        }
        _elem[pos] = val;
        ++_size;
    }

    // ɾ��Ԫ��
    T remove(int pos) {
        if (pos < 0 || pos >= _size) {
            throw std::out_of_range("Remove position out of range");
        }
        T res = _elem[pos];
        // Ԫ��ǰ��
        for (int i = pos; i < _size - 1; ++i) {
            _elem[i] = _elem[i + 1];
        }
        --_size;
        return res;
    }

    // ����Ԫ��
    int find(const T& val) const {
        for (int i = 0; i < _size; ++i) {
            if (_elem[i] == val) {
                return i;
            }
        }
        return -1;
    }

    // ѡ������
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

    // ��ӡԪ��
    void print() const {
        std::cout << "Vector(" << _size << "/" << _cap << "): ";
        for (int i = 0; i < _size; ++i) {
            std::cout << _elem[i] << " ";
        }
        std::cout << std::endl;
    }
};