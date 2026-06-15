#include "ring_buffer.h"
#include <assert.h>
#include <stdio.h>

int main()
{
    RingBuffer_t buffer;
    RingBuffer_Init(&buffer);
    assert(RingBuffer_IsEmpty(&buffer) == true);
    for(int i = 0; i < RING_BUFFER_SIZE; ++i)
    {
        assert(RingBuffer_Push(&buffer, i) == true);
    }
    assert(RingBuffer_IsFull(&buffer) == true);
    printf("Executed Successful!\n");
    return 0;
}