#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "Ticket.h"
#include "TicketQueue.h"
#include "UndoStack.h"
#include "ServiceHistoryList.h"

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string readNonEmptyLine(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        if (!input.empty()) {
            return input;
        }
        std::cout << "Input cannot be empty. Please try again.\n";
    }
}

int readIntInRange(const std::string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minVal && value <= maxVal) {
            clearInputBuffer();
            return value;
        }
        std::cout << "Please enter a valid number between " << minVal << " and " << maxVal << ".\n";
        clearInputBuffer();
    }
}

std::string chooseServiceType() {
    std::cout << "Select service:\n";
    std::cout << "  1. Driver's Licence\n";
    std::cout << "  2. Passport\n";
    std::cout << "  3. NIS\n";
    int choice = readIntInRange("Choice (1-3): ", 1, 3);
    if (choice == 1) return "Licence";
    if (choice == 2) return "Passport";
    return "NIS";
}

void insertionSortByTicketNumber(std::vector<Ticket>& tickets) {
    for (size_t i = 1; i < tickets.size(); ++i) {
        Ticket key = tickets[i];
        size_t j = i;
        while (j > 0 && tickets[j - 1].ticketNumber > key.ticketNumber) {
            tickets[j] = tickets[j - 1];
            --j;
        }
        tickets[j] = key;
    }
}

void printBanner(const TicketQueue& priorityQueue,
                 const TicketQueue& regularQueue,
                 const ServiceHistoryList& history,
                 bool hasActiveTicket,
                 const Ticket& activeTicket) {
    int servedCount = static_cast<int>(history.toVector().size());
    std::cout << "\n======================================================\n";
    std::cout << "     GRENADA GOVERNMENT SERVICES DIGITAL QUEUE (C++)\n";
    std::cout << "======================================================\n";
    std::cout << " Priority Waiting: " << priorityQueue.size()
              << " | Regular Waiting: " << regularQueue.size()
              << " | Served: " << servedCount << "\n";
    std::cout << " Currently at Counter: ";
    if (hasActiveTicket) {
        std::cout << "#" << activeTicket.ticketNumber << " (" << activeTicket.citizenName
                  << " - " << activeTicket.serviceType
                  << (activeTicket.isPriority ? ", PRIORITY" : "") << ")\n";
    } else {
        std::cout << "[None]\n";
    }
    std::cout << "------------------------------------------------------\n";
    std::cout << " 1. Take a Virtual Ticket (Citizen)\n";
    std::cout << " 2. Call Next Ticket (Elderly/PWD Priority First)\n";
    std::cout << " 3. Mark Current Ticket as Served\n";
    std::cout << " 4. Undo Last Staff Action\n";
    std::cout << " 5. View Waiting Queues\n";
    std::cout << " 6. View Served History (Sorted by Ticket #)\n";
    std::cout << " 0. Exit\n";
}

int main() {
    TicketQueue priorityQueue;
    TicketQueue regularQueue;
    UndoStack undoStack;
    ServiceHistoryList history;

    int nextTicketNumber = 101;
    bool hasActiveTicket = false;
    Ticket activeTicket{};

    priorityQueue.enqueue(Ticket{nextTicketNumber++, "Ethel Joseph", "NIS", true});
    regularQueue.enqueue(Ticket{nextTicketNumber++, "Devon Baptiste", "Licence", false});
    regularQueue.enqueue(Ticket{nextTicketNumber++, "Marcus Charles", "Passport", false});
    priorityQueue.enqueue(Ticket{nextTicketNumber++, "Gloria Thomas", "Licence", true});

    while (true) {
        printBanner(priorityQueue, regularQueue, history, hasActiveTicket, activeTicket);
        int choice = readIntInRange("Select an option: ", 0, 6);

        if (choice == 0) {
            std::cout << "Exiting system. Goodbye!\n";
            break;
        } else if (choice == 1) {
            std::string name = readNonEmptyLine("Citizen full name: ");
            std::string service = chooseServiceType();
            int prio = readIntInRange("Elderly/PWD priority? (0 = Regular, 1 = Priority): ", 0, 1);
            bool isPriority = (prio == 1);

            Ticket t{nextTicketNumber++, name, service, isPriority};
            if (isPriority) {
                priorityQueue.enqueue(t);
            } else {
                regularQueue.enqueue(t);
            }
            std::cout << "Issued Ticket #" << t.ticketNumber << " for " << t.citizenName << ".\n";
        } else if (choice == 2) {
            if (hasActiveTicket) {
                std::cout << "A ticket is already at the counter. Mark it as served first (Option 3).\n";
            } else {
                Ticket nextTicket{};
                if (!priorityQueue.isEmpty() && priorityQueue.dequeue(nextTicket)) {
                    activeTicket = nextTicket;
                    hasActiveTicket = true;
                    undoStack.push(ActionRecord{ACTION_CALLED_TICKET, activeTicket, true});
                    std::cout << "Calling PRIORITY Ticket #" << activeTicket.ticketNumber
                              << " (" << activeTicket.citizenName << ").\n";
                } else if (!regularQueue.isEmpty() && regularQueue.dequeue(nextTicket)) {
                    activeTicket = nextTicket;
                    hasActiveTicket = true;
                    undoStack.push(ActionRecord{ACTION_CALLED_TICKET, activeTicket, false});
                    std::cout << "Calling REGULAR Ticket #" << activeTicket.ticketNumber
                              << " (" << activeTicket.citizenName << ").\n";
                } else {
                    std::cout << "No citizens waiting in either queue.\n";
                }
            }
        } else if (choice == 3) {
            if (!hasActiveTicket) {
                std::cout << "No active ticket at the counter to mark as served.\n";
            } else {
                history.append(activeTicket);
                undoStack.push(ActionRecord{ACTION_SERVED_TICKET, activeTicket, activeTicket.isPriority});
                std::cout << "Marked Ticket #" << activeTicket.ticketNumber << " as served.\n";
                hasActiveTicket = false;
            }
        } else if (choice == 4) {
            if (undoStack.isEmpty()) {
                std::cout << "Nothing to undo.\n";
            } else {
                ActionRecord lastAction{};
                if (undoStack.pop(lastAction)) {
                    if (lastAction.type == ACTION_CALLED_TICKET) {
                        if (lastAction.fromPriorityQueue) {
                            priorityQueue.enqueue(lastAction.ticket);
                        } else {
                            regularQueue.enqueue(lastAction.ticket);
                        }
                        if (hasActiveTicket && activeTicket.ticketNumber == lastAction.ticket.ticketNumber) {
                            hasActiveTicket = false;
                        }
                        std::cout << "Undid call for Ticket #" << lastAction.ticket.ticketNumber << ".\n";
                    } else if (lastAction.type == ACTION_SERVED_TICKET) {
                        Ticket removed{};
                        history.removeByTicketNumber(lastAction.ticket.ticketNumber, removed);
                        activeTicket = lastAction.ticket;
                        hasActiveTicket = true;
                        std::cout << "Undid serve for Ticket #" << lastAction.ticket.ticketNumber
                                  << " (returned to counter).\n";
                    }
                }
            }
        } else if (choice == 5) {
            std::cout << "\n--- PRIORITY QUEUE (Elderly / Disability) ---\n";
            priorityQueue.display();
            std::cout << "\n--- REGULAR QUEUE ---\n";
            regularQueue.display();
        } else if (choice == 6) {
            std::cout << "\n--- SERVED HISTORY (Sorted by Ticket #) ---\n";
            std::vector<Ticket> servedTickets = history.toVector();
            insertionSortByTicketNumber(servedTickets);
            ServiceHistoryList::displayTickets(servedTickets);
        }
    }

    return 0;
}
