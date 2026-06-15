#include "ring_buffer.h"

void RingBuffer_Init(RingBuffer_t * const rb) {
    if (rb == NULL) {
        return;
    }

    rb->head = 0U;
    rb->tail = 0U;
}

bool RingBuffer_Push(RingBuffer_t * const rb, const uint8_t data) {
    if (rb == NULL) {
        return false;
    }

    if (RingBuffer_IsFull(rb)) {
        return false;
    }

    rb->buffer[rb->head] = data;
    
    rb->head = (rb->head + 1U) & (RING_BUFFER_SIZE - 1U);

    return true;
}

bool RingBuffer_Pop(RingBuffer_t * const rb, uint8_t * const data) {
    if ((rb == NULL) || (data == NULL)) {
        return false;
    }

    if (RingBuffer_IsEmpty(rb)) {
        return false;
    }

    *data = rb->buffer[rb->tail];
    
    rb->tail = (rb->tail + 1U) & (RING_BUFFER_SIZE - 1U);

    return true;
}

bool RingBuffer_IsEmpty(const RingBuffer_t * const rb) {
    if (rb == NULL) {
        return true; 
    }

    return (rb->head == rb->tail);
}

bool RingBuffer_IsFull(const RingBuffer_t * const rb) {
    if (rb == NULL) {
        return true;
    }

    return (((rb->head + 1U) & (RING_BUFFER_SIZE - 1U)) == rb->tail);
}