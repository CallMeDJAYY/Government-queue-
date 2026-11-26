#include "UndoStack.h"

// Starts with an empty stack
UndoStack::UndoStack() : topNode(nullptr) {}

// Cleans up any remaining nodes so memory is not leaked
UndoStack::~UndoStack() {
    ActionRecord temp;
    while (pop(temp)) {
    }
}

// Inserts a new action record at the top of the stack
void UndoStack::push(const ActionRecord& action) {
    // Allocate a new node pointing to the previous top
    StackNode* newNode = new StackNode{action, topNode};
    topNode = newNode;
}

// Removes and returns the top action from the stack
bool UndoStack::pop(ActionRecord& poppedAction) {
    if (isEmpty()) {
        return false;
    }

    // Save current top node, advance topNode, and delete old node
    StackNode* temp = topNode;
    poppedAction = topNode->action;
    topNode = topNode->next;
    delete temp;
    return true;
}

// Checks whether the stack has any recorded actions
bool UndoStack::isEmpty() const {
    return topNode == nullptr;
}
