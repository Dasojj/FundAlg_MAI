#include <vector>
#include <algorithm>
#include <memory>
#include <sstream>
#include "logger.h"
#include "block_allocator.h"

int main(){
    logger_builder builder;
    builder.add_stream("console", logger::error).add_stream("allocator_log", logger::information);
    logger* logger = builder.build();
    allocator* allocator1 = new allocator(logger);
    block_allocator* alloc = new block_allocator(1024, block_allocator::first, allocator1, logger);
    int* int_ptr = static_cast<int*>(alloc->allocate(sizeof(int)));
    *int_ptr = 42;
    std::cout << "int: " << *int_ptr << std::endl;
    char* str_ptr = static_cast<char*>(alloc->allocate(sizeof(char) * 6));
    std::strcpy(str_ptr, "hello");
    std::cout << "string: " << str_ptr << std::endl;
    class my_class {
    public:
        int x, y;
        my_class(int x, int y) : x(x), y(y) {}
    };
    my_class* my_class_ptr = static_cast<my_class*>(alloc->allocate(sizeof(my_class)));
    *my_class_ptr = my_class(1, 2);
    std::cout << "my_class: (" << my_class_ptr->x << ", " << my_class_ptr->y << ")" << std::endl;

    bool* tmp_ptr = reinterpret_cast<bool*>(alloc->allocate(sizeof(bool)));
    *tmp_ptr = true;
    alloc->deallocate(tmp_ptr);


    alloc->deallocate(str_ptr);
    alloc->deallocate(int_ptr);
    alloc->deallocate(my_class_ptr);

    delete alloc;
    delete allocator1;
    delete logger;
    return 0;
}