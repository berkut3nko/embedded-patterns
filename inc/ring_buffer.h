#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define RING_BUFFER_SIZE (256U)

/**
 * @brief Ring buffer context structure.
 */
typedef struct {
    uint8_t buffer[RING_BUFFER_SIZE];
    volatile uint32_t head;
    volatile uint32_t tail;
} RingBuffer_t;

/**
 * @brief Initializes the ring buffer context.
 * @param rb Pointer to the ring buffer instance.
 * @warning Must be called before any other ring buffer operations.
 */
void RingBuffer_Init(RingBuffer_t * const rb);

/**
 * @brief Pushes a single byte into the ring buffer.
 * @param rb Pointer to the ring buffer instance.
 * @param data The byte to write.
 * @return true if successful, false if the buffer is full.
 */
bool RingBuffer_Push(RingBuffer_t * const rb, const uint8_t data);

/**
 * @brief Pops a single byte from the ring buffer.
 * @param rb Pointer to the ring buffer instance.
 * @param data Pointer to the variable where the read byte will be stored.
 * @return true if successful, false if the buffer is empty.
 * @warning Does not check if data pointer is NULL in release mode, ensure safety in caller.
 */
bool RingBuffer_Pop(RingBuffer_t * const rb, uint8_t * const data);

/**
 * @brief Checks if the ring buffer is empty.
 * @param rb Constant pointer to a constant ring buffer instance.
 * @return true if empty, false otherwise.
 */
bool RingBuffer_IsEmpty(const RingBuffer_t * const rb);

/**
 * @brief Checks if the ring buffer is full.
 * @param rb Constant pointer to a constant ring buffer instance.
 * @return true if full, false otherwise.
 */
bool RingBuffer_IsFull(const RingBuffer_t * const rb);
#endif