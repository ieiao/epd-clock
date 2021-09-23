#ifndef _UI_H_
#define _UI_H_

enum {
    PAGE_KEEP = 0,
    PAGE_NEXT,
    PAGE_PREV,
    PAGE_BREAK
};

struct user_page {
    void (*init)(void *data);
    void (*draw)(void *data);
    int (*ctl)(void *data);
    struct user_page *next;
    struct user_page *prev;
};

void ui_main_loop(void *data);

#endif
