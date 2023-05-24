#include "sound.h"

#define COIN_SOUND "assets/sounds/coin.wav"
#define OUCH_SOUND "assets/sounds/ouch.wav"
#define JUMP_SOUND "assets/sounds/jump.wav"


bool mute;

void init_sounds(Sounds* sounds){
    (sounds->mute_ptr) = &mute; //Yes, I know...
    *(sounds->mute_ptr) = false;
    sounds->coin_sound = COIN_SOUND;
    sounds->ouch_sound = OUCH_SOUND;
    sounds->jump_sound = JUMP_SOUND;
}

void* play_sound(void* _){
    char* file_name = (char*)_;
    if(!mute){
        PlaySound(file_name, NULL, SND_ASYNC);
    }
    return NULL;
}

