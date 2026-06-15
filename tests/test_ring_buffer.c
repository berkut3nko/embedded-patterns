#include "tests.h"

void test_ring_buffer() {
    RingBuffer_t buffer;
    RingBuffer_Init(&buffer);
    
    // Test Case 1: Initial state
    assert(RingBuffer_IsEmpty(&buffer) == true);
    assert(RingBuffer_IsFull(&buffer) == false);

    // Test Case 2: Fill the buffer
    // Since we leave 1 slot empty, the usable capacity is (RING_BUFFER_SIZE - 1)
    const uint32_t usable_capacity = RING_BUFFER_SIZE - 1U;
    
    for (uint32_t i = 0U; i < usable_capacity; ++i) {
        assert(RingBuffer_Push(&buffer, (uint8_t)i) == true);
    }
    
    // Buffer must be full now, next push must fail
    assert(RingBuffer_IsFull(&buffer) == true);
    assert(RingBuffer_Push(&buffer, 100U) == false);

    // Test Case 3: Empty the buffer and verify values
    uint8_t read_data = 0U;
    for (uint32_t i = 0U; i < usable_capacity; ++i) {
        assert(RingBuffer_Pop(&buffer, &read_data) == true);
        assert(read_data == (uint8_t)i);
    }

    // Buffer must be empty now, next pop must fail
    assert(RingBuffer_IsEmpty(&buffer) == true);
    assert(RingBuffer_Pop(&buffer, &read_data) == false);
}