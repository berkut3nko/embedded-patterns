#ifndef PACKET_PARSER_H
#define PACKET_PARSER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define PARSER_STX          (0x02U)
#define PARSER_ETX          (0x03U)
#define PARSER_MAX_PAYLOAD  (64U)

/**
 * @brief States of the packet parsing Finite State Machine.
 */
typedef enum {
    STATE_WAIT_STX,    /**< Waiting for Start of Text byte */
    STATE_READ_LEN,    /**< Reading payload length */
    STATE_READ_PAYLOAD,/**< Reading payload data bytes */
    STATE_READ_CRC,    /**< Reading checksum byte */
    STATE_WAIT_ETX     /**< Verifying End of Text byte */
} ParserState_t;

/**
 * @brief Structure representing a fully parsed packet.
 */
typedef struct {
    uint8_t payload[PARSER_MAX_PAYLOAD];
    uint8_t length;
} Packet_t;

/**
 * @brief Parser context structure.
 */
typedef struct {
    ParserState_t state;
    Packet_t rx_packet;
    uint8_t payload_index;
    uint8_t calculated_crc;
    uint8_t received_crc;
} PacketParser_t;

/**
 * @brief Initializes the packet parser context.
 * @param parser Pointer to the parser instance.
 * @warning Must be called before feeding any bytes to the parser.
 */
void PacketParser_Init(PacketParser_t * const parser);

/**
 * @brief Feeds a single byte into the parser state machine.
 * @param parser Pointer to the parser instance.
 * @param byte Incoming byte to process.
 * @param out_packet Pointer to the packet structure where valid data will be copied.
 * @return true if a complete and valid packet has been successfully parsed, false otherwise.
 * @warning out_packet will only be modified if the function returns true.
 */
bool PacketParser_ParseByte(PacketParser_t * const parser, const uint8_t byte, Packet_t * const out_packet);

#endif /* PACKET_PARSER_H */