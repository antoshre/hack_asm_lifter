//
// Created by rob on 7/24/20.
//

#ifndef HACK_LIFTER_EMBED_H
#define HACK_LIFTER_EMBED_H

#include <cstdint>

extern "C" {
void handle_keyboard(int16_t *);
void handle_screen(int16_t *);
};

#endif //HACK_LIFTER_EMBED_H
