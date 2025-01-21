#include "Queue.h"
#include <iostream>

// Constructor to initialize an empty queue
Queue::Queue() {
    front = -1;
    rear = -1;
}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {

    // Implement circular structure


    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        std::cout << "Queue is full" << std::endl;
        return;
    }

    if (isEmpty()) {
        front = 0;
        rear = 0;
    } else {
        rear = (rear + 1) % MAX_QUEUE_SIZE;
    }

    // Add the province
    data[rear] = province;

}


// Removes and returns the front province from the queue
int Queue::dequeue() {

    if (isEmpty()) {
        std::cerr << "Queue is empty" << std::endl;
        return -1;
    }

    int province = data[front];

    if (front == rear) {
        front = -1;
        rear = -1;
    } else {
        front = (front + 1) % MAX_QUEUE_SIZE;
    }

    return province;

}


// Returns the front province without removing it
int Queue::peek() const {

    if (isEmpty()) {
        std::cerr << "Queue is empty! Cannot peek.\n";
        return -1;
    }
    return data[front];

}


// Checks if the queue is empty
bool Queue::isEmpty() const {

    return front == -1;

}

bool Queue::contains(int value) const {

    for (int i = front; i != rear; i = (i + 1) % MAX_QUEUE_SIZE) {
        if (data[i] == value) {
            return true;
        }
    }
    return false;

}




// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {

    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        std::cerr << "Queue Overflow! Cannot enqueue priority province " << province << ".\n";
        return;
    }

    if (isEmpty()) {
        front = 0;
        rear = 0;
        data[front] = province;
    } else {
        front = (front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
        data[front] = province;
    }
}

