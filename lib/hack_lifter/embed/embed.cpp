//
// Created by rob on 7/24/20.
//

#include "embed.h"

#include <cstdio>

extern "C" {

void handle_keyboard(int16_t *MEM) {
    char c;
    std::scanf("%c", &c);
    MEM[0x6000] = c;
    std::printf("handle_keyboard called: %c\n", c);
}

void handle_screen(int16_t *MEM) {
    std::printf("handle_screen called\n");
}
}