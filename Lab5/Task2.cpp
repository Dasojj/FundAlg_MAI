#include <iostream>
#include <cstring>
#include <sstream>
#include "Task0.h"
#include "logger.h"

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
        for(int i = 0; i < 255; i++) info_stream << std::hex << std::setfill('0') << std::setw(2) << (int)bytes[i] << " ";
        info_stream << ">";
        ::operator delete(target_to_dealloc);
        log(info_stream.str(), logger::information);
    }
};

int main() {
    logger_builder builder;
    builder.add_stream("console", logger::error).add_stream("allocator_log", logger::information);
    allocator alloc(builder.build());
    int* int_ptr = static_cast<int*>(alloc.allocate(sizeof(int)));
    *int_ptr = 42;
    std::cout << "int: " << *int_ptr << std::endl;
    alloc.deallocate(int_ptr);
    char* str_ptr = static_cast<char*>(alloc.allocate(sizeof(char) * 6));
    std::strcpy(str_ptr, "hello");
    std::cout << "string: " << str_ptr << std::endl;
    alloc.deallocate(str_ptr);
    class my_class {
    public:
        int x, y;
        my_class(int x, int y) : x(x), y(y) {}
    };
    my_class* my_class_ptr = static_cast<my_class*>(alloc.allocate(sizeof(my_class)));
    *my_class_ptr = my_class(1, 2);
    std::cout << "my_class: (" << my_class_ptr->x << ", " << my_class_ptr->y << ")" << std::endl;
    alloc.deallocate(my_class_ptr);
    return 0;
}
