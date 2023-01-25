#ifndef BYES_STATIC_ALLOCATOR_H
#define BYES_STATIC_ALLOCATOR_H

#include <new>
#include <stdexcept>

namespace byes {

    namespace memory
    {
        template<typename Type>
        class buffer_allocator
        {
        private:
            size_t allocation_position;
        public:

            char* buffer;
            size_t& allocation_position_ref;
            size_t size;

            buffer_allocator(void* buffer, size_t size) : allocation_position(0), buffer(static_cast<char*>(buffer)), allocation_position_ref(allocation_position), size(size) {}

            template<typename OtherType>
            buffer_allocator(const buffer_allocator<OtherType>& alloc) : allocation_position(0), buffer(alloc.buffer), allocation_position_ref(alloc.allocation_position_ref), size(alloc.size) {}



            typedef Type value_type;

            value_type* allocate(size_t n)
            {
                if (allocation_position_ref + n * sizeof(value_type) > size)
                    throw std::bad_alloc();

                value_type* res = reinterpret_cast<value_type*>(buffer + allocation_position_ref);
                allocation_position_ref += n * sizeof(value_type);
                return res;
            }

            void deallocate(value_type* ptr, size_t n)
            {
            }
        };

        template<typename Type, size_t size_in_bytes>
        class static_allocator : public buffer_allocator<Type>
        {
        private:
            char buffer[size_in_bytes];

        public:

            static_allocator() : buffer_allocator<Type>(buffer, size_in_bytes) {}

            template<typename OtherType>
            static_allocator(const buffer_allocator<OtherType>& alloc) : buffer_allocator<Type>(alloc) {}

            template <class TypeToAllocate>
            struct rebind {
                typedef buffer_allocator<TypeToAllocate> other;
            };

            template <>
            struct rebind<Type> {
                typedef static_allocator<Type, size_in_bytes> other; // 
            };

        };


        template<typename Type, size_t element_count, size_t service_memory_size = 2 * sizeof(void*), size_t vector_growth_coef = 4>
        class static_vector_allocator : public static_allocator<Type, vector_growth_coef* element_count * sizeof(Type) + service_memory_size>
        {
            static const size_t memory_size = vector_growth_coef * element_count * sizeof(Type) + service_memory_size;

        public:
            static_vector_allocator() : static_allocator<Type, memory_size>() {};
            static_vector_allocator(const static_allocator<Type, memory_size>& alloc) : static_allocator<Type, memory_size>(alloc) {}
        };
    }
}

#endif // BYES_STATIC_ALLOCATOR_H