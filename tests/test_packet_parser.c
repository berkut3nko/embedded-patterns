#include "tests.h"

/**
 * @brief Helper to feed an entire packet byte-by-byte into the parser.
 */
static bool feed_packet_to_parser(PacketParser_t * const parser, const uint8_t * const raw_data, const uint32_t len, Packet_t * const out) {
    bool result = false;
    for (uint32_t i = 0U; i < len; ++i) {
        if (PacketParser_ParseByte(parser, raw_data[i], out)) {
            result = true;
        }
    }
    return result;
}

void test_packet_parser() {
    RingBuffer_t rb;
    PacketParser_t parser;
    PacketParser_Init(&parser);
    Packet_t parsed_packet;

    // Test Case A: Valid Packet Parsing
    // STX (0x02) | LEN (0x04) | Data (0xDE, 0xAD, 0xBE, 0xEF) | CRC (STX^LEN^D0^D1^D2^D3) | ETX (0x03)
    // CRC calculation: 0x02 ^ 0x04 ^ 0xDE ^ 0xAD ^ 0xBE ^ 0xEF = 0x24
    uint8_t valid_raw_packet[] = { 0x02U, 0x04U, 0xDEU, 0xADU, 0xBEU, 0xEFU, 0x24U, 0x03U };
    
    bool parse_success = feed_packet_to_parser(&parser, valid_raw_packet, sizeof(valid_raw_packet), &parsed_packet);
    assert(parse_success == true);
    assert(parsed_packet.length == 4U);
    assert(parsed_packet.payload[0] == 0xDEU);
    assert(parsed_packet.payload[1] == 0xADU);
    assert(parsed_packet.payload[2] == 0xBEU);
    assert(parsed_packet.payload[3] == 0xEFU);

    // Test Case B: Corrupt CRC Packet
    // Changing CRC from 0x24 to 0x00
    uint8_t corrupt_crc_packet[] = { 0x02U, 0x04U, 0xDEU, 0xADU, 0xBEU, 0xEFU, 0x00U, 0x03U };
    parse_success = feed_packet_to_parser(&parser, corrupt_crc_packet, sizeof(corrupt_crc_packet), &parsed_packet);
    assert(parse_success == false); // Should reject due to bad checksum

    // Test Case C: Buffer Integration (Push bytes to RingBuffer, then pop and parse)
    RingBuffer_Init(&rb);
    PacketParser_Init(&parser);

    // Push raw packet bytes into RingBuffer
    for (uint32_t i = 0U; i < sizeof(valid_raw_packet); ++i) {
        assert(RingBuffer_Push(&rb, valid_raw_packet[i]) == true);
    }

    // Process from RingBuffer byte-by-byte
    bool integration_parse_success = false;
    uint8_t stream_byte = 0U;
    while (RingBuffer_Pop(&rb, &stream_byte)) {
        if (PacketParser_ParseByte(&parser, stream_byte, &parsed_packet)) {
            integration_parse_success = true;
        }
    }
    assert(integration_parse_success == true);
    assert(parsed_packet.length == 4U);
    assert(parsed_packet.payload[0] == 0xDEU);
    return;
}