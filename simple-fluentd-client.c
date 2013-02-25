#include "simple-fluentd-client.h"

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>

fluentd_client_ctx_t* fluentd_client_new()
{
    fluentd_client_ctx_t* ctx;
    ctx = (fluentd_client_ctx_t*) calloc(1, sizeof(fluentd_client_ctx_t));
    ctx->address = NULL;
    ctx->port = 0;
    ctx->socket = 0;
    return ctx;
}
void fluentd_client_ctx_free(const fluentd_client_ctx_t *ctx)
{
    if(ctx->socket != 0){
        close(ctx->socket);
    }
    free((void*)ctx);
}

void fluentd_client_connect(fluentd_client_ctx_t* ctx)
{
    if((ctx->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    int flags;
    flags = fcntl(ctx->socket, F_GETFL, 0);
    if (flags < 0) {
        perror("fcntl error");
        exit(EXIT_FAILURE);
    }
    if (fcntl(ctx->socket, F_SETFL, flags | O_NONBLOCK) < 0) {
        perror("fcntl error");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(ctx->port);
    addr.sin_addr.s_addr = inet_addr(ctx->address);

 reconnect:
    if((connect(ctx->socket, (struct sockaddr *) &addr, sizeof(addr))) < 0){
        int err = errno;
        switch(err){
        case EISCONN:
            puts("connected");
            break;
        case EALREADY:
        case EINTR:
        case EINPROGRESS:
            puts("wait");
            {
                /* sleep */
                struct timeval tv;
                tv.tv_sec = 1;
                tv.tv_usec = 0;
                if(select(0, NULL, NULL, NULL, &tv) == -1){
                    perror("select error");
                    exit(EXIT_FAILURE);
                }
                puts("re connect");
                goto reconnect;
                break;
            }
        default:
            perror("connect error");
            exit(EXIT_FAILURE);
        }
    }
}

int fluentd_client_write(const fluentd_client_ctx_t* ctx, const msgpack_sbuffer *sbuf)
{
    int r;
 rewrite:
    r = write(ctx->socket, sbuf->data, sbuf->size);
    if(r < 0){
        int err = errno;
        switch(err){
        case EAGAIN:
        case EINTR:
            goto rewrite;
            break;
        default:
            perror("write error");
            exit(EXIT_FAILURE);
        }
    }
    return r;
}
