# Dynamic Memory Allocator
## _Implicit list DMA with deferred coalescing written in C_

# Performance
- Allocations take O(n) where n is the number of blocks
- Deallocations are done just by flagging the block as unsued which is O(1) 
- Coalescing requires iterating over the heap which is O(n)

