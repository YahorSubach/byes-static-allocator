#ifndef BYES_STATIC_ALLOCATOR_H
#define BYES_STATIC_ALLOCATOR_H

#include <new>
#include <stdexcept>

namespace byes {

    namespace memory
    {
        template<typename Type>
        class BufferAllocator
        {
        private:
            size_t allocation_position;
        public:

            char* buffer;
            size_t& allocation_position_ref;
            size_t size_in_bytes;

            BufferAllocator(void* buffer, size_t size_in_bytes) : allocation_position(0), buffer(static_cast<char*>(buffer)), allocation_position_ref(allocation_position), size_in_bytes(size_in_bytes) {}

            template<typename OtherType>
            BufferAllocator(const BufferAllocator<OtherType>& alloc) : allocation_position(0), buffer(alloc.buffer), allocation_position_ref(alloc.allocation_position_ref), size_in_bytes(alloc.size_in_bytes) {}



            typedef Type value_type;

            value_type* allocate(size_t n)
            {
                if (allocation_position_ref + n * sizeof(value_type) > size_in_bytes)
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
        class StaticAllocator : public BufferAllocator<Type>
        {
        private:
            char buffer[size_in_bytes];

        public:

            StaticAllocator() : BufferAllocator<Type>(buffer, size_in_bytes) {}

            template<typename OtherType>
            StaticAllocator(const BufferAllocator<OtherType>& alloc) : BufferAllocator<Type>(alloc) {}

            template <class TypeToAllocate>
            struct rebind {
                typedef BufferAllocator<TypeToAllocate> other;
            };

            template <>
            struct rebind<Type> {
                typedef StaticAllocator<Type, size_in_bytes> other;
            };

        };


        // helper class to determine buffer size for vector. Based on msvc where vector allocates 2 pointers in addition to data and recalculates size as new_size = max(old_size + 1, old_size + old_size / 2) 
        template<typename Type, size_t max_items_cnt, size_t max_allocation_cnt = 24, size_t growth_denominator = 2, size_t extra_data_size = 2 * sizeof(void*)>
        class VectorStaticBufferSize : public VectorStaticBufferSize<Type, max_items_cnt, max_allocation_cnt - 1, growth_denominator, extra_data_size>
        {
        protected:

            static const size_t prev_allocated_items_cnt = VectorStaticBufferSize<Type, max_items_cnt, max_allocation_cnt - 1, growth_denominator, extra_data_size>::new_allocated_items_cnt;

            static const size_t old_value = VectorStaticBufferSize<Type, max_items_cnt, max_allocation_cnt - 1, growth_denominator, extra_data_size>::value;

            static const size_t new_allocated_items_cnt_via_denominator = prev_allocated_items_cnt + prev_allocated_items_cnt / growth_denominator;

            static const size_t new_allocated_items_cnt =
                prev_allocated_items_cnt != 0 && max_items_cnt > prev_allocated_items_cnt
                ?
                (new_allocated_items_cnt_via_denominator > prev_allocated_items_cnt + 1 ? new_allocated_items_cnt_via_denominator : prev_allocated_items_cnt + 1)
                :
                0;

        public:

            static const size_t value = old_value + sizeof(Type) * new_allocated_items_cnt;
        };

        template<typename Type, size_t max_items_cnt, size_t growth_denominator, size_t extra_data_size>
        class VectorStaticBufferSize<Type, max_items_cnt, 0, growth_denominator, extra_data_size>
        {
        protected:

            static const size_t new_allocated_items_cnt = max_items_cnt > 0 ? 1 : 0;

        public:

            static const size_t value = extra_data_size + sizeof(Type) * new_allocated_items_cnt;
        };


        template<typename Type, size_t max_items_cnt, size_t max_allocation_cnt = 24, size_t growth_denominator = 2, size_t extra_data_size = 2 * sizeof(void*)>
        class StaticVectorAllocator : public StaticAllocator<Type, VectorStaticBufferSize<Type, max_items_cnt, max_allocation_cnt, growth_denominator, extra_data_size>::value>
        {
            static const size_t memory_size = VectorStaticBufferSize<Type, max_items_cnt, max_allocation_cnt, growth_denominator, extra_data_size>::value;

        public:
            StaticVectorAllocator() : StaticAllocator<Type, memory_size>() {};
            StaticVectorAllocator(const StaticAllocator<Type, memory_size>& alloc) : StaticAllocator<Type, memory_size>(alloc) {}
        };
    }
}

#endif // BYES_STATIC_ALLOCATOR_H