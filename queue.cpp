#include "queue.h"

Queue::Queue() {
    front = -1;
    rear = -1;
}

bool Queue::isFull() {
    return rear == SIZE - 1;
}

bool Queue::isEmpty() {
    return front == -1 || front > rear;
}

bool Queue::enqueue(int value) {
    if (isFull()) {
        return false;
    } else {
        if (front == -1) front = 0;  // Initialize front for the first element
        arr[++rear] = value;
        return true;
    }
}

int Queue::dequeue() {
    if (isEmpty()) {
        return -1;
    } else {
        int value = arr[front++];
        if (front > rear) {
            // Reset queue if all elements are dequeued
            front = rear = -1;
        }
        return value;
    }
}