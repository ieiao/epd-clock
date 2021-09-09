#include "utf8-cov.h"

static const char head_mask[] = {0xff, 0x7f, 0x1f, 0x0f, 0x07, 0x03, 0x01};
static const char shift_tab[] = {0, 6, 12, 18, 24, 30};

int32_t utf8_cov_unicode(const char *stream, uint32_t *offset)
{
    char head = *stream;
    int32_t unicode = 0;
    int length = 0;

    if (!(head & 0x80))
        length = 1;
    else
    {
        while (head & 0x80)
        {
            length++;
            head <<= 1;
        }
    }

    if (length == 1)
        unicode = head;
    else
    {
        stream += (length - 1);
        switch (length)
        {
        case 6:
            unicode |= ((*stream--) & 0x3f);
        case 5:
            unicode |= ((*stream--) & 0x3f) << shift_tab[length - 5];
        case 4:
            unicode |= ((*stream--) & 0x3f) << shift_tab[length - 4];
        case 3:
            unicode |= ((*stream--) & 0x3f) << shift_tab[length - 3];
        case 2:
            unicode |= ((*stream--) & 0x3f) << shift_tab[length - 2];
            unicode |= (*stream & head_mask[length - 1]) << shift_tab[length - 1];
            break;
        default:
            unicode = -1;
        }
    }

    *offset = length;
    return unicode;
}