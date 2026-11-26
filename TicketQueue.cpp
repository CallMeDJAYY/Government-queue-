#include "TicketQueue.h"
#include <iostream>

// Initializes front and rear pointers to nullptr and count to zero
TicketQueue::TicketQueue() : front(nullptr), rear(nullptr), count(0) {}

// Cleans up all nodes in the queue to prevent memory leaks
TicketQueue::~TicketQueue() {
    Ticket temp;
    // Repeatedly dequeue until the queue is empty
    while (dequeue(temp)) {
    }
}

// Appends a ticket to the rear of the FIFO queue
void TicketQueue::enqueue(const Ticket& ticket) {
    // Allocate a new node holding the incoming ticket
    QueueNode* newNode = new QueueNode{ticket, nullptr};

    if (isEmpty()) {
        // When empty, both front and rear point to the new node
        front = newNode;
        rear = newNode;
    } else {
        // Link the current rear node to the new node and advance rear
        rear->next = newNode;
        rear = newNode;
    }
    ++count;
}

// Removes the front ticket and writes it to removedTicket
bool TicketQueue::dequeue(Ticket& removedTicket) {
    if (isEmpty()) {
        return false;
    }

    // Save the front node so its memory can be freed
    QueueNode* temp = front;
    removedTicket = front->data;

    // Move front forward to the next waiting ticket
    front = front->next;

    // If the queue is now empty, reset rear to nullptr as well
    if (front == nullptr) {
        rear = nullptr;
    }

    delete temp;
    --count;
    return true;
}

// Reads the front ticket without modifying the queue
bool TicketQueue::peek(Ticket& frontTicket) const {
    if (isEmpty()) {
        return false;
    }
    frontTicket = front->data;
    return true;
}

// Checks whether the queue has zero tickets
bool TicketQueue::isEmpty() const {
    return front == nullptr;
}

// Returns the current ticket count
int TicketQueue::size() const {
    return count;
}

// Displays all tickets currently waiting in line
void TicketQueue::display() const {
    if (isEmpty()) {
        std::cout << "  [No tickets waiting]\n";
        return;
    }

    QueueNode* current = front;
    int position = 1;
    // Traverse from front to rear and print each ticket
    while (current != nullptr) {
        std::cout << "  " << position << ". Ticket #" << current->data.ticketNumber
                  << " | " << current->data.citizenName
                  << " | " << current->data.serviceType
                  << (current->data.isPriority ? " [PRIORITY]" : "") << "\n";
        current = current->next;
        ++position;
    }
}
