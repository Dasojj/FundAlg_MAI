#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <iostream>
#include <cstring>
#include <sstream>
#include "logger.h"
#include "memory.h"

class allocator : public memory {
private:
    logger* _log;
    void log(std::string const & str, logger::severity severity) const {
        if (_log == nullptr) return;
        _log->log(str, severity);
    }
public:
    allocator(logger *log = nullptr){ _log = log; }
    allocator(const allocator& obj) = delete;
    allocator& operator=(const allocator& obj) = delete;

    void* allocate(size_t target_size) const override {
        void* ptr = nullptr;
        try { ptr = ::operator new(target_size); }
        catch(...){
            log("Error allocating memory", logger::error);
            throw;
        }
        std::ostringstream info_stream;
        info_stream << "Allocated memory with adress < " << ptr << " >";
        log(info_stream.str(), logger::information);
        return ptr;
    }
    
    void deallocate(void* target_to_dealloc) const override {
        std::ostringstream info_stream;
        info_stream << "Deallocated memory with adress < " << target_to_dealloc <<" > and bytes collection < ";
        unsigned char* bytes = static_cast<unsigned char*>(target_to_dealloc);
        for(int i = 0; i < 255; i++) info_stream << (int)bytes[i] << " ";
        info_stream << ">";
        ::operator delete(target_to_dealloc);
        log(info_stream.str(), logger::information);
    }
};

#endif //ALLOCATOR_H