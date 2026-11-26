#ifndef UNDO_STACK_H
#define UNDO_STACK_H

#include "Ticket.h"

// Types of staff actions that can be recorded and undone
enum ActionType {
    ACTION_CALLED_TICKET, // Staff called the next ticket from priority or regular queue
    ACTION_SERVED_TICKET  // Staff marked the currently called ticket as served
};

// Stores enough information to reverse one staff action
struct ActionRecord {
    ActionType type;       // Kind of action performed
    Ticket ticket;         // Ticket affected by the action
    bool fromPriorityQueue; // True if the ticket came from the priority queue
};

// Node for a linked stack of staff actions
struct StackNode {
    ActionRecord action; // Recorded action
    StackNode* next;     // Pointer to the previous action below it
};

// Last-In, First-Out (LIFO) stack used to undo recent staff mistakes
class UndoStack {
private:
    StackNode* topNode; // Points to the most recent action

public:
    // Initializes an empty stack
    UndoStack();

    // Frees all remaining stack nodes on destruction
    ~UndoStack();

    // Pushes a new staff action onto the top of the stack
    void push(const ActionRecord& action);

    // Pops the most recent action and stores it in poppedAction
    bool pop(ActionRecord& poppedAction);

    // Returns true if there are no actions to undo
    bool isEmpty() const;
};

#endif
