#include "uart_driver.h"
#include <stddef.h>

// Simulated hardware mutex to represent CPU global interrupt lock
#if defined(__unix__) || defined(__APPLE__) || defined(_POSIX_VERSION)
#include <pthread.h>
static pthread_mutex_t global_irq_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

void sys_enter_critical_section(void) {
#if defined(__unix__) || defined(__APPLE__) || defined(_POSIX_VERSION)
    (void)pthread_mutex_lock(&global_irq_mutex);
#endif
}

void sys_exit_critical_section(void) {
#if defined(__unix__) || defined(__APPLE__) || defined(_POSIX_VERSION)
    (void)pthread_mutex_unlock(&global_irq_mutex);
#endif
}

void UartDriver_Init(UartDriver_t * const driver) {
    if (driver == NULL) {
        return;
    }

    RingBuffer_Init(&driver->rx_buffer);
    PacketParser_Init(&driver->parser);
    driver->is_initialized = true;
}

void UartDriver_RxISR(UartDriver_t * const driver, const uint8_t incoming_byte) {
    // Defensive check: MISRA C compliance
    if (driver == NULL || !driver->is_initialized) {
        return;
    }

    (void)RingBuffer_Push(&(driver->rx_buffer), incoming_byte);
}

bool UartDriver_GetPacket(UartDriver_t * const driver, Packet_t * const out_packet) {
    bool packet_parsed = false;

    if ((driver == NULL) || (out_packet == NULL) || (!driver->is_initialized)) {
        return false;
    }

    bool pop_success = false;

    /* Process bytes from the buffer until a full packet is parsed or the buffer is empty */
    do {
        uint8_t out_byte = 0U;

        /* Keep critical section as short as possible: protect only the shared buffer operation */
        sys_enter_critical_section();
        pop_success = RingBuffer_Pop(&(driver->rx_buffer), &out_byte);
        sys_exit_critical_section();

        if (pop_success) {
            packet_parsed = PacketParser_ParseByte(&(driver->parser), out_byte, out_packet);
        }
    } while (pop_success && (!packet_parsed));

    return packet_parsed;
}