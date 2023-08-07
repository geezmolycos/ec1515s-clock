
#include "button.h"

void button_test_event(ButtonConfig *button, ButtonEvent *event){
    bool new_isdown = !button->get(); // read
    event->type = BUTTONEVENT_IDLE;
    event->series = button->series;
    bool isdown = button->state & M_BUTTON_STATE_ISDOWN;
    bool bounce = button->state & M_BUTTON_STATE_BOUNCE;
    bool repeat = button->state & M_BUTTON_STATE_REPEAT;

    button->time++;
    if (button->time >= BUTTON_SERIES_TIME){
        button->series = 0;
    }
    if (bounce){
        button->bounce_time++;
        if (new_isdown == isdown){
            bounce = false;
        } else if (button->bounce_time > BUTTON_BOUNCE_TIME){
            bounce = false;
            if (isdown){
                button->series++;
                event->type = BUTTONEVENT_UP;
            } else {
                event->type = BUTTONEVENT_DOWN;
            }
            isdown = !isdown;
            button->time = 0;
            repeat = false;
        }
    } else {
        if (new_isdown != isdown){
            bounce = true;
            button->bounce_time = 0;
        }
    }
    if (repeat){
        if (button->time >= BUTTON_REPEAT_INTERVAL){
            button->time = 0;
            if (isdown){
                event->type = BUTTONEVENT_REPEAT;
            }
        }
    } else {
        if (isdown && button->time >= BUTTON_REPEAT_INITIAL){
            button->time = 0;
            repeat = true;
        }
    }
    
    button->state = (
        isdown << P_BUTTON_STATE_ISDOWN
        | bounce << P_BUTTON_STATE_BOUNCE
        | repeat << P_BUTTON_STATE_REPEAT
    );
}
