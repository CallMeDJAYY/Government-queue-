#include "ServiceHistoryList.h"
#include <iostream>

// Initializes empty head and tail pointers
ServiceHistoryList::ServiceHistoryList() : head(nullptr), tail(nullptr) {}

// Frees every node in the linked list
ServiceHistoryList::~ServiceHistoryList() {
    HistoryNode* current = head;
    while (current != nullptr) {
        HistoryNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
    tail = nullptr;
}

// Adds a served ticket at the end of the linked list
void ServiceHistoryList::append(const Ticket& ticket) {
    HistoryNode* newNode = new HistoryNode{ticket, nullptr};

    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
}

// Searches for a ticket number and removes its node from the list
bool ServiceHistoryList::removeByTicketNumber(int ticketNumber, Ticket& removedTicket) {
    HistoryNode* current = head;
    HistoryNode* prev = nullptr;

    // Walk through the list until a matching ticket number is found
    while (current != nullptr) {
        if (current->data.ticketNumber == ticketNumber) {
            removedTicket = current->data;

            // Unlink the node from head, middle, or tail
            if (prev == nullptr) {
                head = current->next;
            } else {
                prev->next = current->next;
            }

            if (current == tail) {
                tail = prev;
            }

            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }

    return false;
}

// Checks whether the served ticket list is empty
bool ServiceHistoryList::isEmpty() const {
    return head == nullptr;
}

// Copies linked list tickets into a std::vector for sorting
std::vector<Ticket> ServiceHistoryList::toVector() const {
    std::vector<Ticket> result;
    HistoryNode* current = head;
    while (current != nullptr) {
        result.push_back(current->data);
        current = current->next;
    }
    return result;
}

// Prints a given vector of tickets
void ServiceHistoryList::displayTickets(const std::vector<Ticket>& tickets) {
    if (tickets.empty()) {
        std::cout << "  [No tickets to display]\n";
        return;
    }

    for (size_t i = 0; i < tickets.size(); ++i) {
        std::cout << "  " << (i + 1) << ". Ticket #" << tickets[i].ticketNumber
                  << " | " << tickets[i].citizenName
                  << " | " << tickets[i].serviceType
                  << (tickets[i].isPriority ? " [PRIORITY]" : "") << "\n";
    }
}

// Prints all completed tickets in service order
void ServiceHistoryList::display() const {
    if (isEmpty()) {
        std::cout << "  [No tickets served yet]\n";
        return;
    }

    HistoryNode* current = head;
    int index = 1;
    while (current != nullptr) {
        std::cout << "  " << index << ". Ticket #" << current->data.ticketNumber
                  << " | " << current->data.citizenName
                  << " | " << current->data.serviceType
                  << (current->data.isPriority ? " [PRIORITY]" : "") << "\n";
        current = current->next;
        ++index;
    }
}
