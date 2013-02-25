#include <stdio.h>

#include <msgpack.h>
#include "simple-fluentd-client.h"

int main(int argc, char** argv)
{
    flented_client_ctx_t* ctx = fluentd_client_new();
    ctx->address = "127.0.0.1";
    ctx->port = 24224;
    
    fluentd_client_connect(ctx);

	/* msgpack::sbuffer is a simple buffer implementation. */
	msgpack_sbuffer sbuf;
	msgpack_sbuffer_init(&sbuf);

	/* serialize values into the buffer using msgpack_sbuffer_write callback function. */
	msgpack_packer pk;
	msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    time_t now = time(NULL);

	msgpack_pack_array(&pk, 2);
	msgpack_pack_raw(&pk, 10);
	msgpack_pack_raw_body(&pk, "debug.test", 10);
	msgpack_pack_array(&pk, 2);
	msgpack_pack_long(&pk, now);
    msgpack_pack_map(&pk, 2);
	msgpack_pack_raw(&pk, 3);
	msgpack_pack_raw_body(&pk, "key", 3);
	msgpack_pack_raw(&pk, 5);
	msgpack_pack_raw_body(&pk, "value", 5);

    fluentd_client_write(ctx, &sbuf);

	return 0;
}
