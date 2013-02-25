e#ifndef SIMPLE_FLUENTD_CLIENT_H
#define SIMPLE_FLUENTD_CLIENT_H

#include <msgpack.h>

typedef struct {
    char* address;    /* xxx.xxx.xxx.xxx */
    int port;
    int socket;
} fluentd_client_ctx_t;

fluentd_client_ctx_t* fluentd_client_new();
void fluentd_client_ctx_free(const fluentd_client_ctx_t *ctx);

void fluentd_client_connect(fluentd_client_ctx_t* ctx);

int fluentd_client_write(const fluentd_client_ctx_t* ctx, const msgpack_sbuffer *sbuf);

#endif
