#include <iostream>
#include <stdexcept>
#include <string>

// ջģ���ࣨ���ڶ�̬����ʵ�֣�
template <typename T>
class Stack {
private:
    T* elements;     // �洢Ԫ�ص�����
    int capacity;    // ��ǰ����
    int topIdx;      // ջ��������-1��ʾ��ջ��
    const int MIN_CAPACITY = 10;  // ��С����

    // ���ݲ�����������
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

    // ���ݲ��������룬��С����С������
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
    // ���캯��
    Stack() : elements(nullptr), capacity(0), topIdx(-1) {}

    // ��������
    ~Stack() {
        delete[] elements;
    }

    // �������캯��
    Stack(const Stack& other) : capacity(other.capacity), topIdx(other.topIdx) {
        elements = new T[capacity];
        for (int i = 0; i <= topIdx; ++i) {
            elements[i] = other.elements[i];
        }
    }

    // ��ֵ�����
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

    // ��ȡջ��С
    int size() const {
        return topIdx + 1;
    }

    // �ж�ջ�Ƿ�Ϊ��
    bool empty() const {
        return topIdx == -1;
    }

    // ��ȡջ��Ԫ�أ���������
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

    // ��ջ����
    void push(const T& elem) {
        if (size() >= capacity) {
            expand();
        }
        elements[++topIdx] = elem;
    }

    // ��ջ����
    void pop() {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        --topIdx;
        shrink();  // ���ܴ�������
    }

    // ���ջ
    void clear() {
        topIdx = -1;
        shrink();  // ��������С����
    }
};

// ���нڵ�ṹ��������ʽ���У�
template <typename T>
struct QueueNode {
    T data;
    QueueNode* next;
    QueueNode(const T& d) : data(d), next(nullptr) {}
};

// ����ģ���ࣨ���ڵ�����ʵ�֣�
template <typename T>
class Queue {
private:
    QueueNode<T>* head;  // ��ͷָ�루������front()������ͻ��
    QueueNode<T>* rear;  // ��βָ��
    int count;           // Ԫ������

public:
    // ���캯��
    Queue() : head(nullptr), rear(nullptr), count(0) {}

    // ��������
    ~Queue() {
        clear();
    }

    // �������캯��
    Queue(const Queue& other) : head(nullptr), rear(nullptr), count(0) {
        QueueNode<T>* cur = other.head;
        while (cur) {
            enqueue(cur->data);
            cur = cur->next;
        }
    }

    // ��ֵ�����
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

    // ��ȡ���д�С
    int size() const {
        return count;
    }

    // �ж϶����Ƿ�Ϊ��
    bool empty() const {
        return count == 0;
    }

    // ��ȡ��ͷԪ�أ���������
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

    // ��ȡ��βԪ�أ���������
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

    // ��Ӳ���
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

    // ���Ӳ���
    void dequeue() {
        if (empty()) {
            throw std::underflow_error("Queue is empty");
        }
        QueueNode<T>* temp = head;
        head = head->next;
        delete temp;
        --count;
        if (empty()) {  // ������Ϊ�գ����ö�βָ��
            rear = nullptr;
        }
    }

    // ��ն���
    void clear() {
        while (!empty()) {
            dequeue();
        }
    }
};

// ˫�˶��нڵ�ṹ
template <typename T>
struct DequeNode {
    T data;
    DequeNode* prev;
    DequeNode* next;
    DequeNode(const T& d) : data(d), prev(nullptr), next(nullptr) {}
};

// ˫�˶���ģ���ࣨ����˫������ʵ�֣�
template <typename T>
class Deque {
private:
    DequeNode<T>* head;  // ��ͷָ�루������front()������ͻ��
    DequeNode<T>* rear;  // ��βָ��
    int count;           // Ԫ������

public:
    // ���캯��
    Deque() : head(nullptr), rear(nullptr), count(0) {}

    // ��������
    ~Deque() {
        clear();
    }

    // �������캯��
    Deque(const Deque& other) : head(nullptr), rear(nullptr), count(0) {
        DequeNode<T>* cur = other.head;
        while (cur) {
            push_back(cur->data);
            cur = cur->next;
        }
    }

    // ��ֵ�����
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

    // ��ȡ˫�˶��д�С
    int size() const {
        return count;
    }

    // �ж�˫�˶����Ƿ�Ϊ��
    bool empty() const {
        return count == 0;
    }

    // ��ȡ��ͷԪ��
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

    // ��ȡ��βԪ��
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

    // �Ӷ�ͷ���
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

    // �Ӷ�β���
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

    // �Ӷ�ͷ����
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
            rear = nullptr;  // �����б�գ����ö�βָ��
        }
        delete temp;
        --count;
    }

    // �Ӷ�β����
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
            head = nullptr;  // �����б�գ����ö�ͷָ��
        }
        delete temp;
        --count;
    }

    // ���˫�˶���
    void clear() {
        while (!empty()) {
            pop_front();
        }
    }
};

// ʹ��ʾ��
using namespace std;

int main() {
    // ջ��Stack��ʹ��ʾ��
    cout << "=== ջ��Stack������ʾ�� ===" << endl;
    Stack<int> stack;
    stack.push(10);
    stack.push(20);
    stack.push(30);
    cout << "ջ��С: " << stack.size() << endl;
    cout << "ջ��Ԫ��: " << stack.top() << endl;

    stack.pop();
    cout << "������ջ��Ԫ��: " << stack.top() << endl;

    while (!stack.empty()) {
        cout << "����Ԫ��: " << stack.top() << endl;
        stack.pop();
    }
    cout << "ջ�Ƿ�Ϊ��: " << (stack.empty() ? "��" : "��") << endl << endl;

    // ���У�Queue��ʹ��ʾ��
    cout << "=== ���У�Queue������ʾ�� ===" << endl;
    Queue<string> queue;
    queue.enqueue("First");
    queue.enqueue("Second");
    queue.enqueue("Third");
    cout << "���д�С: " << queue.size() << endl;
    cout << "��ͷԪ��: " << queue.front() << endl;
    cout << "��βԪ��: " << queue.back() << endl;

    queue.dequeue();
    cout << "���Ӻ��ͷԪ��: " << queue.front() << endl;

    while (!queue.empty()) {
        cout << "����Ԫ��: " << queue.front() << endl;
        queue.dequeue();
    }
    cout << "�����Ƿ�Ϊ��: " << (queue.empty() ? "��" : "��") << endl << endl;

    // ˫�˶��У�Deque��ʹ��ʾ��
    cout << "=== ˫�˶��У�Deque������ʾ�� ===" << endl;
    Deque<double> deque;
    deque.push_front(1.1);  // ��ͷ���
    deque.push_back(2.2);   // ��β���
    deque.push_front(0.5);  // ��ͷ���
    deque.push_back(3.3);   // ��β���

    cout << "˫�˶��д�С: " << deque.size() << endl;
    cout << "��ͷԪ��: " << deque.front() << endl;
    cout << "��βԪ��: " << deque.back() << endl;

    deque.pop_front();  // ��ͷ����
    deque.pop_back();   // ��β����
    cout << "���˸�����һ��Ԫ�غ�:" << endl;
    cout << "�¶�ͷԪ��: " << deque.front() << endl;
    cout << "�¶�βԪ��: " << deque.back() << endl;

    deque.clear();
    cout << "��պ�˫�˶����Ƿ�Ϊ��: " << (deque.empty() ? "��" : "��") << endl;

    return 0;
}