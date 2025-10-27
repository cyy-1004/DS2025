#ifndef STACK_H
#define STACK_H
#include <stdexcept>

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val) : data(val), next(nullptr) {}
    };
    Node* topNode;
    int _size;

public:
    Stack() : topNode(nullptr), _size(0) {}

    ~Stack() {
        while (!empty()) {
            pop();
        }
    }

    void push(const T& val) {
        Node* newNode = new Node(val);
        newNode->next = topNode;
        topNode = newNode;
        _size++;
    }

    T pop() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        Node* temp = topNode;
        T val = temp->data;
        topNode = topNode->next;
        delete temp;
        _size--;
        return val;
    }

    T top() const {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return topNode->data;
    }

    bool empty() const {
        return topNode == nullptr;
    }

    int size() const {
        return _size;
    }
};

#endif // STACK_H
