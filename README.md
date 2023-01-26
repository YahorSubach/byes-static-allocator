# static vector allocator based on static linear allocator

Primitive linear allocator. No deallocation. Just for cases when we need container interface for small data without heap allocation. Originally written for vector. Therefore, it contains a wrapper that calculates the size of the linear buffer for the vector. Compatible with c++98/11/14/17/20.

## usage
   - with raw buffer
     ``` cpp
      void Fill(std::vector<int, byes::memory::BufferAllocator<int> >& to_fill)
      {
        for (int i = 0; i < 60; i++)
        {
          to_fill.push_back(i);
        }
      }

      int main()
      {
        int buffer[64];
        byes::memory::BufferAllocator<int> allocator(buffer, sizeof(buffer));

        std::vector<int, byes::memory::BufferAllocator<int> > values(allocator); 
        values.reserve(60);		// with msvc vector allocates some extra data, so we cannot allocate 64 items

        Fill(values);
        
        ...
      }
     ```
     
   - allocator containing a static buffer inside
   ```cpp
    void Fill(std::vector<int, byes::memory::StaticAllocator<int, 64 * sizeof(int)> >& to_fill)
    {
      for (int i = 0; i < 60; i++)
      {
        to_fill.push_back(i);
      }
    }

    int main()
    {
      std::vector<int, byes::memory::StaticAllocator<int, 64 * sizeof(int)> > values; // with msvc vector allocates some extra data, so we cannot allocate 64 items
      values.reserve(60);

      Fill(values);

      ...
    }
   ```

   - static vector allocator with items count as allocator parameter
   ```cpp
    void Fill(std::vector<int, byes::memory::StaticVecttorAllocator<int, 64> >& to_fill)
    {
      for (int i = 0; i < 64; i++)
      {
        to_fill.push_back(i);
      }
    }

    int main()
    {
      std::vector<int, byes::memory::StaticVecttorAllocator<int, 64> > values;

      Fill(values);

      ...
    }
   ```
