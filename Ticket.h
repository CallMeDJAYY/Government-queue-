#ifndef TICKET_H
#define TICKET_H

#include <string>

// Represents a citizen's virtual queue ticket for a government service
struct Ticket {
    int ticketNumber;       // Unique sequential ticket number
    std::string citizenName;  // Full name of the citizen
    std::string serviceType;  // Service requested (e.g., Licence, Passport, NIS)
    bool isPriority;        // True for elderly citizens or people with disabilities
};

#endif
