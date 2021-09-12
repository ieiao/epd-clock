#include "button.h"
#include "gpio.h"

struct button {
    unsigned char offset;
    unsigned char state;
    unsigned char count;
    unsigned char event;
};

static struct button buttons[] = {
    {
        .offset = 0,
    },
    {
        .offset = 1,
    },
    {
        .offset = 2,
    }
};

unsigned char get_button_event(unsigned char button)
{
    unsigned char event = buttons[button].event;
    if (event != BT_EVENT_NONE)
        buttons[button].event = BT_EVENT_NONE;
    return event;
}

void clear_all_button_event(void)
{
    unsigned char i;
    for (i = 0; i < 3; i++) {
        buttons[i].state = BT_BEGIN;
        buttons[i].count = 0;
        buttons[i].event = BT_EVENT_NONE;
    }
}

/* 按键检测函数，需要每20ms执行一次 */
void __button_scan(struct button *button)
{
    switch (button->state)
    {
    case BT_BEGIN:
        if ((P2IN & (1 << button->offset)) == 0)
            button->state = BT_PUSHED;
    break;
    
    case BT_PUSHED:
        if ((P2IN & (1 << button->offset)) == 0)
            button->count ++;
        else
            button->state = BT_RELEASE;
    break;

    case BT_RELEASE:
        if (button->count > 3 && button->count <= 40)
            button->event = BT_EVENT_SHORT;
        else if (button->count > 40)
            button->event = BT_EVENT_LONG;
        else
            button->event = BT_EVENT_NONE;
        button->count = 0;
        button->state = BT_BEGIN;
    break;
    
    default:
        break;
    }
}

void buttons_scan(void)
{
    unsigned char i;

    for (i = 0; i < 3; i++) {
        __button_scan(&buttons[i]);
    }
}