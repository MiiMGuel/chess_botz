#ifndef EVENT_H
#define EVENT_H

#include "types.h"
#include "SDL2/SDL.h"

typedef enum event_id {
    EVENT_ID_NONE = 0,
    EVENT_ID_MOVE_FORWARD = 1,
    EVENT_ID_MOVE_BACKWARD = 2,
    EVENT_ID_STRAFE_LEFT = 3,
    EVENT_ID_STRAFE_RIGHT = 4,
} event_id_t;

typedef enum input_type {
    INPUT_TYPE_KEYBOARD = 0,
    INPUT_TYPE_MOUSE    = 1,
    INPUT_TYPE_JOYSTICK = 2
} input_type_t;

typedef struct input_event {
    input_type_t type;
    event_id_t   id;
    bool         is_down;
    bool         is_pressed;
    bool         is_motion;

    union {
        i32 key;
        u8 mouse;
    };
} input_event_t;

#define INPUT_EVENT_COUNT 5

input_event_t iemap_get(event_id_t id);
input_event_t* iemap_getp(event_id_t id);
input_event_t iemap_getn(event_id_t id, u32 num);
input_event_t* iemap_getpn(event_id_t id, u32 num);
void iemap_refresh(void);
void iemap_update(SDL_Event* event);

#endif // EVENT_H