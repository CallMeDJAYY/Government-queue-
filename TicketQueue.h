#ifndef TICKET_QUEUE_H
#define TICKET_QUEUE_H

#include "Ticket.h"

// Node in a singly linked FIFO queue of waiting tickets
struct QueueNode {
    Ticket data;     // Ticket stored at this position
    QueueNode* next; // Pointer to the next ticket behind it in line
};

// First-In, First-Out (FIFO) queue used for regular and priority citizen lines
class TicketQueue {
private:
    QueueNode* front; // Points to the next ticket to be called
    QueueNode* rear;  // Points to the last ticket added to the line
    int count;        // Tracks how many tickets are currently waiting

public:
    // Initializes an empty queue
    TicketQueue();

    // Frees all remaining dynamically allocated nodes when destroyed
    ~TicketQueue();

    // Adds a new ticket to the back of the queue
    void enqueue(const Ticket& ticket);

    // Removes and returns the ticket at the front of the queue
    bool dequeue(Ticket& removedTicket);

    // Copies the front ticket without removing it
    bool peek(Ticket& frontTicket) const;

    // Returns true if no tickets are waiting in the queue
    bool isEmpty() const;

    // Returns the number of tickets currently in the queue
    int size() const;

    // Prints all waiting tickets in order from front to rear
    void display() const;
};

#endif
