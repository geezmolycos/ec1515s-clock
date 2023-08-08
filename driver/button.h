
#ifndef _BUTTON_H
#define _BUTTON_H

#include "project-defs.h"

#define BUTTON_BOUNCE_TIME 1
#define BUTTON_REPEAT_INITIAL 40
#define BUTTON_REPEAT_INTERVAL 6
#define BUTTON_SERIES_TIME 6

typedef enum {
    BUTTONEVENT_IDLE = 0,
    BUTTONEVENT_DOWN,
    BUTTONEVENT_REPEAT,
    BUTTONEVENT_UP,
} ButtonEventType;

typedef struct {
    ButtonEventType type;
    uint8_t series;
} ButtonEvent;

#define M_BUTTON_STATE_ISDOWN 0x01
#define P_BUTTON_STATE_ISDOWN 0
#define M_BUTTON_STATE_BOUNCE 0x02
#define P_BUTTON_STATE_BOUNCE 1
#define M_BUTTON_STATE_REPEAT 0x04
#define P_BUTTON_STATE_REPEAT 2

typedef struct {
    uint8_t state;
    uint8_t bounce_time;
    uint8_t series;
    uint8_t time;
    bool (*get)();
} ButtonConfig;

void button_test_event(ButtonConfig *button, ButtonEvent *event);

#endif // _BUTTON_H
