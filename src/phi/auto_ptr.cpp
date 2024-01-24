#include "auto_ptr.hpp"

std::map<void*, phi::RefCount::counter_t>* phi::RefCount::data_ptr;