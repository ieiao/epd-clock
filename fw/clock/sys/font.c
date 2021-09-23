#include <stdint.h>
#include "font.h"
#include "search.h"
#include "utf8-cov.h"

const uint32_t chinese_unicode_tab[] = {
    0x4e00, /* 一 */
    0x4e09, /* 三 */
    0x4e8c, /* 二 */
    0x4e94, /* 五 */
    0x516d, /* 六 */
    0x5206, /* 分 */
    0x5236, /* 制 */
    0x53d6, /* 取 */
    0x56db, /* 四 */
    0x5c0f, /* 小 */
    0x5c4f, /* 屏 */
    0x5e74, /* 年 */
    0x606f, /* 息 */
    0x65e5, /* 日 */
    0x65f6, /* 时 */
    0x661f, /* 星 */
    0x6708, /* 月 */
    0x671f, /* 期 */
    0x6bb5, /* 段 */
    0x6d88, /* 消 */
    0x786e, /* 确 */
    0x7f6e, /* 置 */
    0x8ba4, /* 认 */
    0x8bbe, /* 设 */
    0x949f, /* 钟 */
    0x95f4, /* 间 */
};

uint32_t get_chinese_offset(uint32_t unicode)
{
    return (int32_binary_search((int32_t *)chinese_unicode_tab,
                               sizeof(chinese_unicode_tab)/sizeof(int32_t),
                               unicode) * 32) + CHINESE_FONT_OFFSET;
}

uint32_t get_ascii_offset(uint32_t unicode)
{
    return ((unicode - 0x20) * 16) + ASCII_FONT_OFFSET;
}
