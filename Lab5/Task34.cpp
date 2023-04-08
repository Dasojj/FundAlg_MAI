#include <iostream>
#include <list>
#include "block_allocator.h"
#include "border_block_allocator.h"

int main(){
    logger_builder builder;
    builder.add_stream("console", logger::error).add_stream("allocator_log_3", logger::information);
    logger* logger = builder.build();
    memory *allocator1 = new allocator(logger);
    memory *allocator2 = new block_allocator(1000000, block_allocator::best, allocator1, logger);
    memory *allocator3 = new border_block_allocator(999900, border_block_allocator::first, allocator2, logger);

    std::list<void*> allocated_blocks;

    srand((unsigned)time(nullptr));

    for (size_t i = 0; i < 20000; ++i)
    {
        void * ptr;

        switch (rand() % 2)
        {
            case 0:
                try
                {
                    ptr = reinterpret_cast<void *>(allocator3->allocate(rand() % 81 + 20)); // разность макс и мин с включенными границами + минимальное
                    allocated_blocks.push_back(ptr);
                }
                catch (std::exception const &ex)
                {
                    std::cout << ex.what() << std::endl;
                }
                break;
            case 1:

                if (allocated_blocks.empty())
                {
                    break;
                }

                try
                {
                    auto iter = allocated_blocks.begin();
                    std::advance(iter, rand() % allocated_blocks.size());
                    allocator3->deallocate(*iter);
                    allocated_blocks.erase(iter);
                }
                catch (std::exception const &ex)
                {
                    std::cout << ex.what() << std::endl;
                }
                break;
        }

        //std::cout << "iter # " << i + 1 << " finish" << std::endl;
    }

    while (!allocated_blocks.empty())
    {
        try
        {
            auto iter = allocated_blocks.begin();
            allocator3->deallocate(*iter);
            allocated_blocks.erase(iter);
        }
        catch (std::exception const &ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }

    delete allocator3;
    delete allocator2;
    delete allocator1;
    delete logger;
    return 0;
}