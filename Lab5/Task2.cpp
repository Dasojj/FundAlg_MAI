#include <iostream>
#include <cstring>
#include <sstream>
#include "Task0.h"
#include "logger.h"

class allocator : public memory {
private:
    logger_builder builder;
    logger* log;
public:
    allocator(){
        builder.add_stream("console", logger::error).add_stream("task2log.txt", logger::information);
        log = builder.build();
    }
    allocator(const allocator& obj) {
        builder = obj.builder;
    }
    allocator& operator=(const allocator& obj){
        builder = obj.builder;
        return *this;
    }
    void* allocate(size_t target_size) const override {
        void* ptr = nullptr;
        try { ptr = ::operator new(target_size); }
        catch(...){
            log->log("Error allocating memory", logger::error);
        }
        std::string memory_info;
        std::ostringstream info_stream(memory_info);
        info_stream << "Allocated memory with adress <" << ptr << ">";
        log->log(memory_info, logger::information);
        return ptr;
    }
    void deallocate(void* target_to_dealloc) const override {
        std::string memory_info;
        std::ostringstream info_stream(memory_info);
        info_stream << "Deallocated memory with adress < " << target_to_dealloc << " > and bytes collection < ";
        unsigned char* bytes = static_cast<unsigned char*>(target_to_dealloc);
        while(bytes) info_stream << *(bytes++) << " ";
        info_stream << ">";
        ::operator delete(target_to_dealloc);
        log->log(memory_info, logger::information);
    }
};

int main() {
    allocator alloc;
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
