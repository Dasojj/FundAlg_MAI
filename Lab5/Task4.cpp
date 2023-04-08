#include <vector>
#include <algorithm>
#include <memory>
#include <sstream>
#include "logger.h"
#include "allocator.h"

class block_allocator : public memory {
public:
    enum allocation_method { first, best, worst };
private:
    void* m_data;

    void* first_fit(size_t target_size) const {
        void* current = m_data;
        current += (sizeof(allocation_method) + sizeof(allocator*) + sizeof(logger*));
        size_t m_size = *reinterpret_cast<size_t*>(current);
        current += sizeof(size_t);
        while (current < m_data + m_size) {
            bool* is_free = reinterpret_cast<bool*>(current);
            size_t* b_size = reinterpret_cast<size_t*>(current + sizeof(bool));
            if ((*is_free) && *b_size >= target_size) {
                void* ok_ptr = current + sizeof(bool) + sizeof(size_t);
                block_init(ok_ptr, target_size);
                return ok_ptr;
            }
            current += (2*(sizeof(bool) + sizeof(size_t)) + *b_size);
        }
        return nullptr;
    }

    void* best_fit(size_t target_size) const {
        long min_ok = INT64_MAX;
        void* ok_ptr = nullptr;
        void* current = m_data;
        current += (sizeof(allocation_method) + sizeof(allocator*) + sizeof(logger*));
        size_t m_size = *reinterpret_cast<size_t*>(current);
        current += sizeof(size_t);
        while (current < m_data + m_size) {
            bool* is_free = reinterpret_cast<bool*>(current);
            size_t* b_size = reinterpret_cast<size_t*>(current + sizeof(bool));
            if (*is_free && *b_size >= target_size) {
                if(min_ok > *b_size) {
                    min_ok = *b_size;
                    ok_ptr = current + sizeof(bool) + sizeof(size_t);
                }
            }
            current += (2*(sizeof(bool) + sizeof(size_t)) + *b_size);
        }
        block_init(ok_ptr, target_size);
        return ok_ptr;
    }

    void* worst_fit(size_t target_size) const {
        long max_ok = -1;
        void* ok_ptr = nullptr;
        void* current = m_data;
        current += (sizeof(allocation_method) + sizeof(allocator*) + sizeof(logger*));
        size_t m_size = *reinterpret_cast<size_t*>(current);
        current += sizeof(size_t);
        while (current < m_data + m_size) {
            bool* is_free = reinterpret_cast<bool*>(current);
            size_t* b_size = reinterpret_cast<size_t*>(current + sizeof(bool));
            if (*is_free && *b_size >= target_size) {
                if(max_ok < *b_size) {
                    max_ok = *b_size;
                    ok_ptr = current + sizeof(bool) + sizeof(size_t);
                }
            }
            current += (2*(sizeof(bool) + sizeof(size_t)) + *b_size);
        }
        block_init(ok_ptr, target_size);
        return ok_ptr;
    }

    void block_init(void* block_ptr, size_t first_size) const {
        if(block_ptr != nullptr){
            bool* is_free = reinterpret_cast<bool*>(block_ptr - (sizeof(bool) + sizeof(size_t)));
            *is_free = false;
            size_t* b_size = reinterpret_cast<size_t*>(block_ptr - sizeof(size_t));
            bool* last_is_free = reinterpret_cast<bool*>(block_ptr + *b_size);
            *last_is_free = false;
            if(*b_size > first_size + 2*(sizeof(bool) + sizeof(size_t))){
                bool* end_is_free = reinterpret_cast<bool*>(block_ptr + first_size);
                *end_is_free = false;
                size_t* end_b_size = reinterpret_cast<size_t*>(block_ptr + first_size + sizeof(bool));
                *end_b_size = first_size;
                bool* next_is_free = reinterpret_cast<bool*>(block_ptr + first_size + sizeof(bool) + sizeof(size_t));
                *next_is_free = true;
                size_t* next_size = reinterpret_cast<size_t*>(block_ptr + first_size + 2*sizeof(bool) + sizeof(size_t));
                *next_size = *b_size - (first_size + 2*(sizeof(bool) + sizeof(size_t)));
                size_t* last_b_size = reinterpret_cast<size_t*>(block_ptr + *b_size + sizeof(bool));
                *last_b_size = *next_size;
                *last_is_free = true;
                *b_size = first_size;
            }
        }
    }

    void log(std::string const & str, logger::severity severity) const {
        void* current = m_data;
        current += (sizeof(allocation_method) + sizeof(allocator*));
        logger* _log = *reinterpret_cast<logger**>(current);
        if (_log == nullptr) return;
        _log->log(str, severity);
    }

public:
    block_allocator(size_t data_size, allocation_method method, allocator* alloc = nullptr, logger *log = nullptr) {
        if(alloc == nullptr) {
            try { m_data = reinterpret_cast<void*>(::operator new(data_size)); }
            catch(...){
                this->log("Error allocating memory", logger::error);
                throw;
            }
        }
        else m_data = reinterpret_cast<void*>(alloc->allocate(data_size));

        void* current = m_data;

        allocation_method* m_method = reinterpret_cast<allocation_method*>(current);
        *m_method = method;
        current += sizeof(allocation_method);

        allocator** m_alloc = reinterpret_cast<allocator**>(current);
        *m_alloc = alloc;
        current += sizeof(allocator*);

        logger** _log = reinterpret_cast<logger**>(current);
        *_log = log;
        current += sizeof(logger*);

        size_t* m_size = reinterpret_cast<size_t*>(current);
        *m_size = (data_size - (sizeof(allocation_method) + sizeof(allocator*) + sizeof(logger*) + sizeof(size_t)));
        current += sizeof(size_t);
        
        bool* is_free = reinterpret_cast<bool*>(current);
        *is_free = true;
        current += sizeof(bool);

        size_t* b_size = reinterpret_cast<size_t*>(current);
        *b_size = *m_size - 2*(sizeof(bool) + sizeof(size_t));
        current += sizeof(size_t);
        void* first_block_ptr = current;
        current += *b_size;

        is_free = reinterpret_cast<bool*>(current);
        *is_free = true;
        current += sizeof(bool);

        b_size = reinterpret_cast<size_t*>(current);
        *b_size = *m_size - 2*(sizeof(bool) + sizeof(size_t));

        std::ostringstream info_stream;
        info_stream << "Allocator initialized with free size " << *b_size << " and with first block address < " << reinterpret_cast<char*>(first_block_ptr) - reinterpret_cast<char*>(m_data) << " >";
        this->log(info_stream.str(), logger::information);
        
    }
    
    ~block_allocator() {
        void* current = m_data;
        current += sizeof(allocation_method);
        allocator* m_alloc = *reinterpret_cast<allocator**>(current);
        if(m_alloc == nullptr) ::operator delete(m_data);
        else m_alloc->deallocate(m_data);
    }

    void* allocate(size_t target_size) const override {
        void* current = m_data;
        allocation_method m_method = *reinterpret_cast<allocation_method*>(current);
        void* ptr;
        std::ostringstream info_stream;
        if(m_method == first){
            ptr = first_fit(target_size);
            info_stream << "Allocated first fit memory with address < " << reinterpret_cast<char*>(ptr) - reinterpret_cast<char*>(m_data) << " >";
            log(info_stream.str(), logger::information);
            return ptr;
        }
        else if(m_method == best){
            ptr = best_fit(target_size);
            info_stream << "Allocated best fit memory with address < " << reinterpret_cast<char*>(ptr) - reinterpret_cast<char*>(m_data) << " >";
            log(info_stream.str(), logger::information);
            return ptr;
        }
        else{
            ptr = worst_fit(target_size);
            info_stream << "Allocated worst fit memory with address < " << reinterpret_cast<char*>(ptr) - reinterpret_cast<char*>(m_data) << " >";
            log(info_stream.str(), logger::information);
            return ptr;
        }
    }

    void deallocate(void* target_to_dealloc) const override {
        if(target_to_dealloc == nullptr) return;
        void* current = target_to_dealloc;

        size_t* b_size = reinterpret_cast<size_t*>(current - sizeof(size_t));
        size_t start_size = *b_size;

        bool* is_free = reinterpret_cast<bool*>(current - (sizeof(bool) + sizeof(size_t)));
        *is_free = true;

        bool* end_is_free = reinterpret_cast<bool*>(current + *b_size);
        *end_is_free = true;

        bool* next_is_free = reinterpret_cast<bool*>(current + *b_size + sizeof(bool) + sizeof(size_t));
        bool* prev_end_is_free = reinterpret_cast<bool*>(current - 2*(sizeof(bool) + sizeof(size_t)));

        if(*next_is_free){
            size_t* next_b_size = reinterpret_cast<size_t*>(current + *b_size + 2*sizeof(bool) + sizeof(size_t));
            *b_size += *next_b_size + 2*(sizeof(size_t) + sizeof(bool));
            size_t* end_b_size = reinterpret_cast<size_t*>(current + *b_size + sizeof(bool));
            *end_b_size = *b_size;
        }
        if(*prev_end_is_free){
            size_t* prev_end_b_size = reinterpret_cast<size_t*>(current - (sizeof(bool) + 2*sizeof(size_t)));
            size_t* prev_b_size = reinterpret_cast<size_t*>(current - (2*(sizeof(bool) + sizeof(size_t)) + *prev_end_b_size + sizeof(size_t)));
            *prev_b_size += *b_size + 2*(sizeof(bool) + sizeof(size_t));
            size_t* end_b_size = reinterpret_cast<size_t*>(current + *b_size + sizeof(bool));
            *end_b_size = *prev_b_size;
        }

        std::ostringstream info_stream;
        info_stream << "Deallocated memory with adress < " << reinterpret_cast<char*>(target_to_dealloc) - reinterpret_cast<char*>(m_data) <<" > and bytes collection < ";
        unsigned char* bytes = static_cast<unsigned char*>(target_to_dealloc);
        for(int i = 0; i < start_size; i++) info_stream << (int)bytes[i] << " ";
        info_stream << ">";
        log(info_stream.str(), logger::information);
    }

    void* operator+=(size_t target_size) const {
        return allocate(target_size);
    }
    void operator-=(void* target_to_dealloc) const {
        deallocate(target_to_dealloc);
    }
};

int main(){
    logger_builder builder;
    builder.add_stream("console", logger::error).add_stream("allocator_log_2", logger::information);
    block_allocator alloc(1024, block_allocator::first, nullptr, builder.build());
    int* int_ptr = static_cast<int*>(alloc.allocate(sizeof(int)));
    *int_ptr = 42;
    std::cout << "int: " << *int_ptr << std::endl;
    char* str_ptr = static_cast<char*>(alloc.allocate(sizeof(char) * 6));
    std::strcpy(str_ptr, "hello");
    std::cout << "string: " << str_ptr << std::endl;
    class my_class {
    public:
        int x, y;
        my_class(int x, int y) : x(x), y(y) {}
    };
    my_class* my_class_ptr = static_cast<my_class*>(alloc.allocate(sizeof(my_class)));
    *my_class_ptr = my_class(1, 2);
    std::cout << "my_class: (" << my_class_ptr->x << ", " << my_class_ptr->y << ")" << std::endl;

    bool* tmp_ptr = reinterpret_cast<bool*>(alloc.allocate(sizeof(bool)));
    *tmp_ptr = true;
    alloc.deallocate(tmp_ptr);


    alloc.deallocate(str_ptr);
    alloc.deallocate(int_ptr);
    alloc.deallocate(my_class_ptr);
    return 0;
}