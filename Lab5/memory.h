#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>

class memory {
public:
    virtual void* allocate(size_t target_size) const = 0;
    virtual void deallocate(void* target_to_dealloc) const = 0;
    void* operator+=(size_t target_size) const {
        return allocate(target_size);
    }
    void operator-=(void* target_to_dealloc) const {
        deallocate(target_to_dealloc);
    }
    virtual ~memory() noexcept = default;
};

#endif //MEMORY_H