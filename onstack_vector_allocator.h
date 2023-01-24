#ifndef BYES_ONSTACK_VECTOR_ALLOCATOR_H
#define BYES_ONSTACK_VECTOR_ALLOCATOR_H

#include <new>
#include <stdexcept>

#define DEPRECATED_ALLOCATOR_MEMBERS 1

namespace byes {

    template<typename Type, size_t size>
    class onstack_vector_allocator {

    private:

        Type stack_memory_[size];
        size_t allocated_from_begin_;
        size_t allocated_from_end_;

    public:

        typedef Type value_type;
        
#if DEPRECATED_ALLOCATOR_MEMBERS == 1

        template <class _Other>
        struct rebind {
            using other = onstack_vector_allocator<_Other, size>;
        };

#endif //  DEPRECATED_ALLOCATOR_MEMBERS

        struct DummyConstructParam {};
        onstack_vector_allocator(DummyConstructParam) : allocated_from_begin_(0), allocated_from_end_(0) {}

        template <class _Other>
        onstack_vector_allocator(const onstack_vector_allocator<_Other, size>& other) : allocated_from_begin_(0), allocated_from_end_(0) {}

        static onstack_vector_allocator Construct()
        {
            return onstack_vector_allocator<Type, size>(DummyConstructParam());
        }



        value_type* allocate(size_t n) 
        { 
            size_t memory_allocated = allocated_from_begin_ + allocated_from_end_;
            if (n + memory_allocated > size)
                throw std::bad_alloc();

            if (allocated_from_begin_ == 0)
            {
                allocated_from_begin_ = n;
                return stack_memory_;
            }

            allocated_from_end_ = n;
            return &stack_memory_[size - n];
            
        }

        void deallocate(value_type* ptr, size_t n)
        {
            if (ptr == stack_memory_)
                allocated_from_begin_ = 0;
            else if (ptr == &stack_memory_[size - allocated_from_end_])
                allocated_from_end_ = 0;
            else throw std::invalid_argument("invalid pointer");
        }
    };

}

#endif // BYES_ONSTACK_VECTOR_ALLOCATOR_H