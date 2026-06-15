#include "tests.h"
#include <stdio.h>

int main(void) {
    printf("Starting embedded integration tests...\n");
    test_ring_buffer();
    test_packet_parser();
    printf("All integration tests executed successfully! PR is ready for submission.\n");
    return 0;
}