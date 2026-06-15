#include "ring_buffer.h"

void RingBuffer_Init(RingBuffer_t * const rb)
{
    if(rb == NULL)
    {
        return;
    }
    else
    {
        rb->head = 0;
        rb->tail = 0;    
    }
    return;
}

bool RingBuffer_Push(RingBuffer_t * const rb, const uint8_t data)
{
    if(rb == NULL)
    {
        return false;
    }
    else
    {
        uint8_t temp = rb->head;
        if(rb->head == RING_BUFFER_SIZE)
        {
            rb->head = 0;
        }
        else
        {
            ++(rb->head);
        }
        if(rb->head == rb->tail)
        {
            rb->head = temp;
            return false;
        }
        else
        {
            rb->buffer[temp] = data;
        }
    }
    return true;
}

bool RingBuffer_Pop(RingBuffer_t * const rb, uint8_t * const data)
{
    if(rb == NULL || data == NULL)
    {
        return false;
    }
    else
    {
        *data = rb->buffer[rb->tail];
        if(rb->tail == RING_BUFFER_SIZE)
        {
            rb->tail = 0;
        }
        else
        {
            ++(rb->tail);
        }
    }
    return true;
}

bool RingBuffer_IsEmpty(const RingBuffer_t * const rb)
{
    if(rb == NULL)
    {
        return false;
    }
    else
    {
        if(rb->head == rb->tail)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool RingBuffer_IsFull(const RingBuffer_t * const rb)
{
    if(rb == NULL)
    {
        return false;
    }
    else
    {
        if(rb->head == RING_BUFFER_SIZE)
        {
            if(rb->tail == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if(rb->head + 1 == rb->tail)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}