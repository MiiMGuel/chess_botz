#include "event.h"

static input_event_t iemap[INPUT_EVENT_COUNT] = {
    { .type = INPUT_TYPE_KEYBOARD, .id = EVENT_ID_NONE },
    { .type = INPUT_TYPE_KEYBOARD, .id = EVENT_ID_MOVE_FORWARD },
    { .type = INPUT_TYPE_KEYBOARD, .id = EVENT_ID_MOVE_BACKWARD },
    { .type = INPUT_TYPE_KEYBOARD, .id = EVENT_ID_STRAFE_LEFT },
    { .type = INPUT_TYPE_KEYBOARD, .id = EVENT_ID_STRAFE_RIGHT },
};

input_event_t iemap_get(event_id_t id) {
    for (u32 i = 0; i < INPUT_EVENT_COUNT; i++) {
        if (iemap[i].id == id) return iemap[i];
    } return (input_event_t){ 0 };
}

input_event_t* iemap_getp(event_id_t id) {
    for (u32 i = 0; i < INPUT_EVENT_COUNT; i++) {
        if (iemap[i].id == id) return &iemap[i];
    } return NULL;
}

input_event_t iemap_getn(event_id_t id, u32 num) {
    u32 count = 0;
    for (u32 i = 0; i < INPUT_EVENT_COUNT; i++) {
        if (iemap[i].id == id && count < num) 
            count++;
        else return iemap[i];
    } return (input_event_t){ 0 };
}

input_event_t* iemap_getpn(event_id_t id, u32 num) {
    u32 count = 0;
    for (u32 i = 0; i < INPUT_EVENT_COUNT; i++) {
        if (iemap[i].id == id && count < num) 
            count++;
        else return &iemap[i];
    } return NULL;
}

void iemap_refresh(void) {
    for (u32 i = 0; i < INPUT_EVENT_COUNT; i++) {
        iemap[i].is_pressed = false;
    }
}

void iemap_update(SDL_Event* event) {
    for (u32 i = 0; i < INPUT_EVENT_COUNT; i++) {
        switch (iemap[i].type) {
        case INPUT_TYPE_KEYBOARD:
            if (event->type == SDL_KEYDOWN && event->key.keysym.sym == iemap[i].key) {
                iemap[i].is_down = true;
                iemap[i].is_pressed = true;
            } if (event->type == SDL_KEYUP && event->key.keysym.sym == iemap[i].key) {
                iemap[i].is_down = false;
            } break;

        case INPUT_TYPE_MOUSE:
            if (event->type == SDL_MOUSEMOTION) 
                iemap[i].is_motion = true;
            else iemap[i].is_motion = false;

            if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == iemap[i].key) {
                iemap[i].is_down = true;
                iemap[i].is_pressed = true;
            } if (event->type == SDL_MOUSEBUTTONUP && event->button.button == iemap[i].key) {
                iemap[i].is_down = false;
            } break;
        
        case INPUT_TYPE_JOYSTICK:
            break;
        
        default:
            break;
        }
    }
}