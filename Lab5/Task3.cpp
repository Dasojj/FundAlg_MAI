#include <vector>
#include <algorithm>
#include <memory>
#include "allocator.h"

class block_allocator : public memory {
public:
    enum allocation_method { first, best, worst };
private:
    allocator* m_alloc;
    char* m_data;
    size_t m_size;
    allocation_method m_method;
    struct block_info {
        size_t size;
        bool is_free;
    };
    
    void* first_fit(size_t target_size) const {
        char* current = m_data;
        while (current < m_data + m_size) {
            block_info* info = reinterpret_cast<block_info*>(current);
            if (info->is_free && info->size >= target_size) {
                info->is_free = false;
                if (info->size > target_size) {
                    block_info* next_info = reinterpret_cast<block_info*>(current + target_size + sizeof(block_info));
                    next_info->is_free = true;
                    next_info->size = info->size - target_size - sizeof(block_info);
                    info->size = target_size;
                }
                return current + sizeof(block_info);
            }
            current += info->size + sizeof(block_info);
        }
        return nullptr;
    }

    void* best_fit(size_t target_size) const {
        std::vector<std::pair<size_t, char*>> blocks;
        char* current = m_data;
        while (current < m_data + m_size) {
            block_info* info = reinterpret_cast<block_info*>(current);
            if (info->is_free && info->size >= target_size) {
                blocks.emplace_back(info->size, current);
            }
            current += info->size + sizeof(block_info);
        }
        std::sort(blocks.begin(), blocks.end());
        for (auto& block : blocks) {
            block_info* info = reinterpret_cast<block_info*>(block.second);
            if (info->size > target_size) {
                block_info* next_info = reinterpret_cast<block_info*>(block.second + target_size + sizeof(block_info));
                next_info->is_free = true;
                next_info->size = info->size - target_size - sizeof(block_info);
                info->size = target_size;
            }
            info->is_free = false;
            return block.second + sizeof(block_info);
        }
        return nullptr;
    }

    void* worst_fit(size_t target_size) const {
        std::vector<std::pair<size_t, char*>> blocks;
        char* current = m_data;
        while (current < m_data + m_size) {
            block_info* info = reinterpret_cast<block_info*>(current);
            if (info->is_free && info->size >= target_size) {
                blocks.emplace_back(info->size, current);
            }
            current += info->size + sizeof(block_info);
        }
        std::sort(blocks.rbegin(), blocks.rend());
        for (auto& block : blocks) {
            block_info* info = reinterpret_cast<block_info*>(block.second);
            if (info->size > target_size) {
                block_info* next_info = reinterpret_cast<block_info*>(block.second + target_size + sizeof(block_info));
                next_info->is_free = true;
                next_info->size = info->size - target_size - sizeof(block_info);
                info->size = target_size;
            }
            info->is_free = false;
            return block.second + sizeof(block_info);
        }
        return nullptr;
    }

    void merge_free() const {
        char* current = m_data;
        block_info* prev_info = reinterpret_cast<block_info*>(current);
        if(current + prev_info->size + sizeof(block_info) < m_data + m_size) current += prev_info->size + sizeof(block_info);
        else return;
        while (current < m_data + m_size) {
            block_info* current_info = reinterpret_cast<block_info*>(current);
            if(current_info->is_free) prev_info->size += current_info->size + sizeof(block_info);
            else prev_info = reinterpret_cast<block_info*>(current);
            current += current_info->size + sizeof(block_info);
        }
    }

public:
    block_allocator(size_t data_size, allocation_method method, allocator* alloc = new allocator()) {
        m_size = data_size;
        m_method = method;
        m_alloc = alloc;
        m_data = static_cast<char*>(m_alloc->allocate(m_size));
        
    }
    ~block_allocator() {
        m_alloc->deallocate(m_data);
    }
    void* allocate(size_t target_size) const override {
        switch (m_method)
        {
        case first:
            return first_fit(target_size);
            break;
        case best:
            return best_fit(target_size);
            break;
        case worst:
            return worst_fit(target_size);
            break;
        default:
            break;
        }
    }
    void deallocate(void* target_to_dealloc) const override {
        if(target_to_dealloc == nullptr) return;
        char* current = reinterpret_cast<char*>(target_to_dealloc) - sizeof(block_info);
        block_info* info = reinterpret_cast<block_info*>(current);
        info->is_free = true;
        merge_free();
    }
    void* operator+=(size_t target_size) const {
        return allocate(target_size);
    }
    void operator-=(void* target_to_dealloc) const {
        deallocate(target_to_dealloc);
    }
};