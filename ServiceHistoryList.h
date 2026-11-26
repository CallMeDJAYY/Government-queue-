#ifndef SERVICE_HISTORY_LIST_H
#define SERVICE_HISTORY_LIST_H

#include "Ticket.h"
#include <vector>

// Node in a singly linked list of served tickets
struct HistoryNode {
    Ticket data;      // Completed ticket record
    HistoryNode* next; // Pointer to the next served ticket
};

// Linked list storing completed service tickets in order
class ServiceHistoryList {
private:
    HistoryNode* head; // First served ticket in the list
    HistoryNode* tail; // Last served ticket in the list

public:
    // Initializes an empty linked list
    ServiceHistoryList();

    // Frees all nodes in the list on destruction
    ~ServiceHistoryList();

    // Appends a newly served ticket to the end of the list
    void append(const Ticket& ticket);

    // Removes a ticket by ticket number (used when undoing a served action)
    bool removeByTicketNumber(int ticketNumber, Ticket& removedTicket);

    // Returns true if no tickets have been served
    bool isEmpty() const;

    // Copies all served tickets into a vector for sorting/reporting
    std::vector<Ticket> toVector() const;

    // Displays a list of tickets in a readable format
    static void displayTickets(const std::vector<Ticket>& tickets);

    // Displays all served tickets in service order
    void display() const;
};

#endif
