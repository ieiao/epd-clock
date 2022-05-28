#ifndef __HTTPS_REQUEST_H__
#define __HTTPS_REQUEST_H__

#include <stddef.h>

int https_get_request(const char *host, const char *url,
                      char *out_buf, size_t out_buf_size);

#endif
