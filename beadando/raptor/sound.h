#ifndef SOUND_H
#define SOUND_H

#include <stdbool.h>
#include <windows.h>

typedef struct Sounds{
    bool* mute_ptr;
    char* coin_sound;
    char* ouch_sound;
    char* jump_sound;
} Sounds;

void init_sounds(Sounds* sounds);

//Call this function with pthread only
void* play_sound(void* _);

#endif /* SOUND_H */

