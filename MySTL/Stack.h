#include <iostream>
#include <stdexcept>
#include <string>

// 栈模板类（基于动态数组实现）
template <typename T>
class Stack {
private:
    T* elements;     // 存储元素的数组
    int capacity;    // 当前容量
    int topIdx;      // 栈顶索引（-1表示空栈）
    const int MIN_CAPACITY = 10;  // 最小容量

    // 扩容操作（翻倍）
    void expand() {
        int newCapacity = (capacity == 0) ? MIN_CAPACITY : capacity * 2;
        T* newElements = new T[newCapacity];
        for (int i = 0; i <= topIdx; ++i) {
            newElements[i] = elements[i];
        }
        delete[] elements;
        elements = newElements;
        capacity = newCapacity;
    }

    // 缩容操作（减半，不小于最小容量）
    void shrink() {
        if (capacity <= MIN_CAPACITY || topIdx + 1 > capacity / 2) return;
        int newCapacity = capacity / 2;
        T* newElements = new T[newCapacity];
        for (int i = 0; i <= topIdx; ++i) {
            newElements[i] = elements[i];
        }
        delete[] elements;
        elements = newElements;
        capacity = newCapacity;
    }

public:
    // 构造函数
    Stack() : elements(nullptr), capacity(0), topIdx(-1) {}

    // 析构函数
    ~Stack() {
        delete[] elements;
    }

    // 拷贝构造函数
    Stack(const Stack& other) : capacity(other.capacity), topIdx(other.topIdx) {
        elements = new T[capacity];
        for (int i = 0; i <= topIdx; ++i) {
            elements[i] = other.elements[i];
        }
    }

    // 赋值运算符
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            delete[] elements;
            capacity = other.capacity;
            topIdx = other.topIdx;
            elements = new T[capacity];
            for (int i = 0; i <= topIdx; ++i) {
                elements[i] = other.elements[i];
            }
        }
        return *this;
    }

    // 获取栈大小
    int size() const {
        return topIdx + 1;
    }

    // 判断栈是否为空
    bool empty() const {
        return topIdx == -1;
    }

    // 获取栈顶元素（不弹出）
    T& top() {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        return elements[topIdx];
    }

    const T& top() const {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        return elements[topIdx];
    }

    // 入栈操作
    void push(const T& elem) {
        if (size() >= capacity) {
            expand();
        }
        elements[++topIdx] = elem;
    }

    // 出栈操作
    void pop() {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        --topIdx;
        shrink();  // 可能触发缩容
    }

    // 清空栈
    void clear() {
        topIdx = -1;
        shrink();  // 缩容至最小容量
    }
};

// 队列节点结构（用于链式队列）
template <typename T>
struct QueueNode {
    T data;
    QueueNode* next;
    QueueNode(const T& d) : data(d), next(nullptr) {}
};

// 队列模板类（基于单链表实现）
template <typename T>
class Queue {
private:
    QueueNode<T>* head;  // 队头指针（避免与front()函数冲突）
    QueueNode<T>* rear;  // 队尾指针
    int count;           // 元素数量

public:
    // 构造函数
    Queue() : head(nullptr), rear(nullptr), count(0) {}

    // 析构函数
    ~Queue() {
        clear();
    }

    // 拷贝构造函数
    Queue(const Queue& other) : head(nullptr), rear(nullptr), count(0) {
        QueueNode<T>* cur = other.head;
        while (cur) {
            enqueue(cur->data);
            cur = cur->next;
        }
    }

    // 赋值运算符
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            clear();
            QueueNode<T>* cur = other.head;
            while (cur) {
                enqueue(cur->data);
                cur = cur->next;
            }
        }
        return *this;
    }

    // 获取队列大小
    int size() const {
        return count;
    }

    // 判断队列是否为空
    bool empty() const {
        return count == 0;
    }

    // 获取队头元素（不弹出）
    T& front() {
        if (empty()) {
            throw std::underflow_error("Queue is empty");
        }
        return head->data;
    }

    const T& front() const {
        if (empty()) {
            throw std::underflow_error("Queue is empty");
        }
        return head->data;
    }

    // 获取队尾元素（不弹出）
    T& back() {
        if (empty()) {
            throw std::underflow_error("Queue is empty");
        }
        return rear->data;
    }

    const T& back() const {
        if (empty()) {
            throw std::underflow_error("Queue is empty");
        }
        return rear->data;
    }

    // 入队操作
    void enqueue(const T& elem) {
        QueueNode<T>* newNode = new QueueNode<T>(elem);
        if (empty()) {
            head = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
        ++count;
    }

    // 出队操作
    void dequeue() {
        if (empty()) {
            throw std::underflow_error("Queue is empty");
        }
        QueueNode<T>* temp = head;
        head = head->next;
        delete temp;
        --count;
        if (empty()) {  // 若队列为空，重置队尾指针
            rear = nullptr;
        }
    }

    // 清空队列
    void clear() {
        while (!empty()) {
            dequeue();
        }
    }
};

// 双端队列节点结构
template <typename T>
struct DequeNode {
    T data;
    DequeNode* prev;
    DequeNode* next;
    DequeNode(const T& d) : data(d), prev(nullptr), next(nullptr) {}
};

// 双端队列模板类（基于双向链表实现）
template <typename T>
class Deque {
private:
    DequeNode<T>* head;  // 队头指针（避免与front()函数冲突）
    DequeNode<T>* rear;  // 队尾指针
    int count;           // 元素数量

public:
    // 构造函数
    Deque() : head(nullptr), rear(nullptr), count(0) {}

    // 析构函数
    ~Deque() {
        clear();
    }

    // 拷贝构造函数
    Deque(const Deque& other) : head(nullptr), rear(nullptr), count(0) {
        DequeNode<T>* cur = other.head;
        while (cur) {
            push_back(cur->data);
            cur = cur->next;
        }
    }

    // 赋值运算符
    Deque& operator=(const Deque& other) {
        if (this != &other) {
            clear();
            DequeNode<T>* cur = other.head;
            while (cur) {
                push_back(cur->data);
                cur = cur->next;
            }
        }
        return *this;
    }

    // 获取双端队列大小
    int size() const {
        return count;
    }

    // 判断双端队列是否为空
    bool empty() const {
        return count == 0;
    }

    // 获取队头元素
    T& front() {
        if (empty()) {
            throw std::underflow_error("Deque is empty");
        }
        return head->data;
    }

    const T& front() const {
        if (empty()) {
            throw std::underflow_error("Deque is empty");
        }
        return head->data;
    }

    // 获取队尾元素
    T& back() {
        if (empty()) {
            throw std::underflow_error("Deque is empty");
        }
        return rear->data;
    }

    const T& back() const {
        if (empty()) {
            throw std::underflow_error("Deque is empty");
        }
        return rear->data;
    }

    // 从队头入队
    void push_front(const T& elem) {
        DequeNode<T>* newNode = new DequeNode<T>(elem);
        if (empty()) {
            head = rear = newNode;
        }
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        ++count;
    }

    // 从队尾入队
    void push_back(const T& elem) {
        DequeNode<T>* newNode = new DequeNode<T>(elem);
        if (empty()) {
            head = rear = newNode;
        }
        else {
            newNode->prev = rear;
            rear->next = newNode;
            rear = newNode;
        }
        ++count;
    }

    // 从队头出队
    void pop_front() {
        if (empty()) {
            throw std::underflow_error("Deque is empty");
        }
        DequeNode<T>* temp = head;
        head = head->next;
        if (head) {
            head->prev = nullptr;
        }
        else {
            rear = nullptr;  // 若队列变空，重置队尾指针
        }
        delete temp;
        --count;
    }

    // 从队尾出队
    void pop_back() {
        if (empty()) {
            throw std::underflow_error("Deque is empty");
        }
        DequeNode<T>* temp = rear;
        rear = rear->prev;
        if (rear) {
            rear->next = nullptr;
        }
        else {
            head = nullptr;  // 若队列变空，重置队头指针
        }
        delete temp;
        --count;
    }

    // 清空双端队列
    void clear() {
        while (!empty()) {
            pop_front();
        }
    }
};

// 使用示例
using namespace std;

int main() {
    // 栈（Stack）使用示例
    cout << "=== 栈（Stack）操作示例 ===" << endl;
    Stack<int> stack;
    stack.push(10);
    stack.push(20);
    stack.push(30);
    cout << "栈大小: " << stack.size() << endl;
    cout << "栈顶元素: " << stack.top() << endl;

    stack.pop();
    cout << "弹出后栈顶元素: " << stack.top() << endl;

    while (!stack.empty()) {
        cout << "弹出元素: " << stack.top() << endl;
        stack.pop();
    }
    cout << "栈是否为空: " << (stack.empty() ? "是" : "否") << endl << endl;

    // 队列（Queue）使用示例
    cout << "=== 队列（Queue）操作示例 ===" << endl;
    Queue<string> queue;
    queue.enqueue("First");
    queue.enqueue("Second");
    queue.enqueue("Third");
    cout << "队列大小: " << queue.size() << endl;
    cout << "队头元素: " << queue.front() << endl;
    cout << "队尾元素: " << queue.back() << endl;

    queue.dequeue();
    cout << "出队后队头元素: " << queue.front() << endl;

    while (!queue.empty()) {
        cout << "出队元素: " << queue.front() << endl;
        queue.dequeue();
    }
    cout << "队列是否为空: " << (queue.empty() ? "是" : "否") << endl << endl;

    // 双端队列（Deque）使用示例
    cout << "=== 双端队列（Deque）操作示例 ===" << endl;
    Deque<double> deque;
    deque.push_front(1.1);  // 队头入队
    deque.push_back(2.2);   // 队尾入队
    deque.push_front(0.5);  // 队头入队
    deque.push_back(3.3);   // 队尾入队

    cout << "双端队列大小: " << deque.size() << endl;
    cout << "队头元素: " << deque.front() << endl;
    cout << "队尾元素: " << deque.back() << endl;

    deque.pop_front();  // 队头出队
    deque.pop_back();   // 队尾出队
    cout << "两端各出队一个元素后:" << endl;
    cout << "新队头元素: " << deque.front() << endl;
    cout << "新队尾元素: " << deque.back() << endl;

    deque.clear();
    cout << "清空后双端队列是否为空: " << (deque.empty() ? "是" : "否") << endl;

    return 0;
}