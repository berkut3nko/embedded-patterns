#include "packet_parser.h"
#include <string.h>

void PacketParser_Init(PacketParser_t * const parser) {
    if (parser == NULL) {
        return;
    }

    parser->state = STATE_WAIT_STX;
    parser->payload_index = 0U;
    parser->calculated_crc = 0U;
    parser->received_crc = 0U;
    parser->rx_packet.length = 0U;
    (void)memset(parser->rx_packet.payload, 0, PARSER_MAX_PAYLOAD);
}

bool PacketParser_ParseByte(PacketParser_t * const parser, const uint8_t byte, Packet_t * const out_packet) {
    bool packet_ready = false;

    if ((parser == NULL) || (out_packet == NULL)) {
        return false;
    }

    switch (parser->state) {
        case STATE_WAIT_STX:
            // TODO: Implement STX checking
            // 1. If byte is PARSER_STX:
            //    - Reset payload_index to 0
            //    - Start calculated_crc calculation with this byte
            //    - Move state to STATE_READ_LEN
            if (byte == PARSER_STX) {
                parser->payload_index = 0U;
                parser->calculated_crc = byte;
                parser->state = STATE_READ_LEN;
            }
            break;

        case STATE_READ_LEN:
            // TODO: Implement Length verification
            // 1. Validate length: if byte is 0 or greater than PARSER_MAX_PAYLOAD,
            //    this is an invalid frame. Reset state back to STATE_WAIT_STX.
            // 2. Otherwise:
            //    - Save length to parser->rx_packet.length
            //    - Update calculated_crc (XOR with this byte)
            //    - Move state to STATE_READ_PAYLOAD
            if (byte == 0U || byte > PARSER_MAX_PAYLOAD) {
                parser->state = STATE_WAIT_STX;
                break;
            }

            parser->rx_packet.length = byte;
            parser->calculated_crc ^= byte;
            parser->state = STATE_READ_PAYLOAD;
            break;

        case STATE_READ_PAYLOAD:
            // TODO: Implement Payload accumulation
            // 1. Save byte into parser->rx_packet.payload at current payload_index
            // 2. Update calculated_crc (XOR with this byte)
            // 3. Increment payload_index
            // 4. If payload_index reached rx_packet.length:
            //    - Move state to STATE_READ_CRC
            if (parser->payload_index < PARSER_MAX_PAYLOAD)
            {
                parser->rx_packet.payload[parser->payload_index] = byte;
            }
            parser->calculated_crc ^= byte;
            ++parser->payload_index;

            if (parser->payload_index == parser->rx_packet.length) {
                parser->state = STATE_READ_CRC;
            }
            break;

        case STATE_READ_CRC:
            // TODO: Implement CRC reception
            // 1. Save incoming byte as received_crc
            // 2. Move state to STATE_WAIT_ETX
            parser->received_crc = byte;
            parser->state = STATE_WAIT_ETX;
            break;

        case STATE_WAIT_ETX:
            // TODO: Implement ETX and CRC verification
            // 1. If byte is PARSER_ETX:
            //    - Verify if received_crc matches calculated_crc
            //    - If MATCH: copy parser->rx_packet to out_packet (use memcpy), set packet_ready to true
            // 2. Regardless of match/mismatch or bad ETX, we must reset the parser state
            //    back to STATE_WAIT_STX to be ready for the next frame.
            if (byte == PARSER_ETX) {
                if (parser->received_crc == parser->calculated_crc) {
                    (void)memcpy(out_packet, &parser->rx_packet, sizeof(Packet_t));
                    packet_ready = true;
                }
            }

            parser->state = STATE_WAIT_STX;
            parser->payload_index = 0U;
            parser->calculated_crc = 0U;
            parser->received_crc = 0U;
            parser->rx_packet.length = 0U;
            break;

        default:
            // Defensive programming: handle corrupted state enum
            parser->state = STATE_WAIT_STX;
            break;
    }
    return packet_ready;
}