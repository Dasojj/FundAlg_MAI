#ifndef BLOCK_ALLOCATOR_H
#define BLOCK_ALLOCATOR_H

#include "logger.h"
#include "allocator.h"

class block_allocator : public memory {
public:
    enum allocation_method { first, best, worst };
private:
    void* m_data;

    void* first_fit(size_t target_size) const {
        void* current = m_data;
        current += (sizeof(allocation_method) + sizeof(memory*) + sizeof(logger*));
        size_t m_size = *reinterpret_cast<size_t*>(current);
        current += sizeof(size_t);
        while (current < m_data + m_size) {
            bool* is_free = reinterpret_cast<bool*>(current);
            size_t* b_size = reinterpret_cast<size_t*>(current + sizeof(bool));
            if ((*is_free) && *b_size >= target_size) {
                *is_free = false;
                void* ok_ptr = current + sizeof(bool) + sizeof(size_t);
                block_split(ok_ptr, target_size);
                return ok_ptr;
            }
            current += *b_size + sizeof(bool) + sizeof(size_t);
        }
        return nullptr;
    }

    void* best_fit(size_t target_size) const {
        long min_ok = INT64_MAX;
        void* ok_ptr = nullptr;
        void* current = m_data;
        current += (sizeof(allocation_method) + sizeof(memory*) + sizeof(logger*));
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
            current += sizeof(bool) + sizeof(size_t) + *b_size;
        }
        bool* is_free = reinterpret_cast<bool*>(ok_ptr - (sizeof(bool) + sizeof(size_t)));
        *is_free = false;
        block_split(ok_ptr, target_size);
        return ok_ptr;
    }

    void* worst_fit(size_t target_size) const {
        long max_ok = -1;
        void* ok_ptr = nullptr;
        void* current = m_data;
        current += (sizeof(allocation_method) + sizeof(memory*) + sizeof(logger*));
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
            current += sizeof(bool) + sizeof(size_t) + *b_size;
        }
        bool* is_free = reinterpret_cast<bool*>(ok_ptr - (sizeof(bool) + sizeof(size_t)));
        *is_free = false;
        block_split(ok_ptr, target_size);
        return ok_ptr;
    }

    void merge_free() const {
        void* current = m_data;
        current += (sizeof(allocation_method) + sizeof(memory*) + sizeof(logger*));
        size_t m_size = *(reinterpret_cast<size_t*>(current));
        current += sizeof(size_t);
        bool* prev_is_free = reinterpret_cast<bool*>(current);
        size_t* prev_b_size = reinterpret_cast<size_t*>(current + sizeof(bool));   
        if(current + sizeof(size_t) + *prev_b_size + sizeof(bool) < m_data + m_size) current += (*prev_b_size + sizeof(bool) + sizeof(size_t));
        else return;
        bool* is_free = reinterpret_cast<bool*>(current);
        size_t* b_size = reinterpret_cast<size_t*>(current + sizeof(bool));
        current += (sizeof(bool) + sizeof(size_t) + *b_size);
        while (current + sizeof(bool) < m_data + m_size) {
            bool* next_is_free = reinterpret_cast<bool*>(current);
            size_t* next_b_size = reinterpret_cast<size_t*>(current + sizeof(bool));
            if(*is_free && (*prev_is_free || *next_is_free)){
                if(*next_is_free){
                    *b_size += *next_b_size + sizeof(size_t) + sizeof(bool);
                }
                if(*prev_is_free){
                    *prev_b_size += *b_size + sizeof(size_t) + sizeof(bool);
                    b_size = next_b_size;
                    is_free = next_is_free;
                }
                if(*next_is_free && *prev_is_free){
                    b_size = reinterpret_cast<size_t*>(current + 2*sizeof(bool) + sizeof(size_t) + *next_b_size);
                    is_free = reinterpret_cast<bool*>(current + sizeof(bool) + sizeof(size_t) + *next_b_size);
                    *next_b_size += (sizeof(size_t) + sizeof(bool) + *b_size);
                }
            }
            else{
                prev_b_size = b_size;
                prev_is_free = is_free;
                b_size = next_b_size;
                is_free = next_is_free;
            }
            current += *next_b_size + sizeof(size_t) + sizeof(bool);
        }
    }

    void block_split(void* block_ptr, size_t first_size) const {
        if(block_ptr != nullptr){
            size_t* b_size = reinterpret_cast<size_t*>(block_ptr - sizeof(size_t));
            if(*b_size > first_size + sizeof(bool) + sizeof(size_t)){
                bool* next_is_free = reinterpret_cast<bool*>(block_ptr + first_size);
                *next_is_free = true;
                size_t* next_size = reinterpret_cast<size_t*>(block_ptr + first_size + sizeof(bool));
                *next_size = *b_size - first_size - sizeof(bool) - sizeof(size_t);
                *b_size = first_size;
            }
        }
    }

    void log(std::string const & str, logger::severity severity) const {
        void* current = m_data;
        current += (sizeof(allocation_method) + sizeof(memory*));
        logger* _log = *reinterpret_cast<logger**>(current);
        if (_log == nullptr) return;
        _log->log(str, severity);
    }

public:
    block_allocator(size_t data_size, allocation_method method, memory* alloc = nullptr, logger *log = nullptr) {
        if(alloc == nullptr) {
            try { m_data = reinterpret_cast<void*>(::operator new(data_size)); }
            catch(...){
                this->log("Error allocating memory", logger::error);
                throw;
            }
        }
        else m_data = alloc->allocate(data_size);

        void* current = m_data;

        allocation_method* m_method = reinterpret_cast<allocation_method*>(current);
        *m_method = method;
        current += sizeof(allocation_method);

        memory** m_alloc = reinterpret_cast<memory**>(current);
        *m_alloc = alloc;
        current += sizeof(memory*);

        logger** _log = reinterpret_cast<logger**>(current);
        *_log = log;
        current += sizeof(logger*);

        size_t* m_size = reinterpret_cast<size_t*>(current);
        *m_size = (data_size - (sizeof(allocation_method) + sizeof(memory*) + sizeof(logger*) + sizeof(size_t)));
        current += sizeof(size_t);
        
        bool* is_free = reinterpret_cast<bool*>(current);
        *is_free = true;
        current += sizeof(bool);

        size_t* b_size = reinterpret_cast<size_t*>(current);
        *b_size = *m_size - (sizeof(bool) + sizeof(size_t));
        current += sizeof(size_t);

        std::ostringstream info_stream;
        info_stream << "Allocator initialized with free size " << *b_size << " and with first block address < " << reinterpret_cast<char*>(current) - reinterpret_cast<char*>(m_data) << " >";
        this->log(info_stream.str(), logger::information);
        
    }
    
    ~block_allocator() {
        void* current = m_data;
        current += sizeof(allocation_method);
        memory* m_alloc = *reinterpret_cast<memory**>(current);
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
        bool* is_free = reinterpret_cast<bool*>(current - (sizeof(size_t) + sizeof(bool)));
        size_t b_size = *reinterpret_cast<size_t*>(current - sizeof(size_t));
        *is_free = true;

        std::ostringstream info_stream;
        info_stream << "Deallocated memory with adress < " << reinterpret_cast<char*>(target_to_dealloc) - reinterpret_cast<char*>(m_data) <<" > and bytes collection < ";
        unsigned char* bytes = static_cast<unsigned char*>(target_to_dealloc);
        for(int i = 0; i < b_size; i++) info_stream << (int)bytes[i] << " ";
        info_stream << ">";
        log(info_stream.str(), logger::information);
        merge_free();
    }
    void* operator+=(size_t target_size) const {
        return allocate(target_size);
    }
    void operator-=(void* target_to_dealloc) const {
        deallocate(target_to_dealloc);
    }
};

#endif //BLOCK_ALLOCATOR_H