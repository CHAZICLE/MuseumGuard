#ifndef DISABLE_DEBUG

#include <iostream>

#define _IOSTREAM_HEADER "DEBUG:[" << __FILE__ << ":" << __LINE__ << " " << __func__ << "] "

#define PRINT_CONTROL(message, control, action) PRINT_DEBUG(message << " (Control:" << control << ", Action:" << action << ")")

#define PRINT_DEBUG(msg) std::cout << _IOSTREAM_HEADER << msg << std::endl;

#endif
