#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include "ring_buffer.h"
#include "packet_parser.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Structure representing the UART driver context.
 */
typedef struct {
    RingBuffer_t rx_buffer;
    PacketParser_t parser;
    volatile bool is_initialized;
} UartDriver_t;

/**
 * @brief Initializes the UART driver and its internal components.
 * @param driver Pointer to the driver instance.
 * @warning Must be called before enabling global interrupts.
 */
void UartDriver_Init(UartDriver_t * const driver);

/**
 * @brief Simulated Hardware UART RX Interrupt Service Routine (ISR).
 * @param driver Pointer to the driver instance.
 * @param incoming_byte Byte received by the hardware UART peripheral.
 * @warning In real hardware, this runs in Handler Mode (Interrupt context).
 * Must be extremely fast and must not block.
 */
void UartDriver_RxISR(UartDriver_t * const driver, const uint8_t incoming_byte);

/**
 * @brief Safely reads a processed packet from the driver.
 * @param driver Pointer to the driver instance.
 * @param out_packet Pointer to the destination packet structure.
 * @return true if a valid packet was retrieved, false otherwise.
 * @warning This function is thread-safe and can be called from Thread Mode.
 */
bool UartDriver_GetPacket(UartDriver_t * const driver, Packet_t * const out_packet);

/**
 * @brief Simulated system call to enter a critical section (disables interrupts).
 */
void sys_enter_critical_section(void);

/**
 * @brief Simulated system call to exit a critical section (re-enables interrupts).
 */
void sys_exit_critical_section(void);

#endif /* UART_DRIVER_H */