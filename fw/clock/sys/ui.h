#ifndef _UI_H_
#define _UI_H_

struct user_page {
    void (*init)(void *data);
    void (*draw)(void *data);
    void (*ctl)(void *data);
    struct user_page *next;
    struct user_page *prev;
};

void ui_main_loop(void *data);

#endif