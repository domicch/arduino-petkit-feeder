#ifndef QUEUE_H
#define QUEUE_H

class Queue {
private:
    static const int SIZE = 10;
    int arr[SIZE];   // Array to store queue elements
    int front;       // Index of front element
    int rear;        // Index of rear element

public:
    Queue();
    bool isFull();

    bool isEmpty();

    bool enqueue(int value);

    int dequeue();
};

#endif