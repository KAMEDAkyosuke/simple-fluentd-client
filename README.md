simple-fluentd-client
=====================

これはなに？
------------

fluented forward Input Plugin に書き込む c のクライアントです。

使い方
------

    $ git submodule init
    $ git submodule update
    $ cd external/msgpack-c
    $ ./bootstrap
    $ ./configure
    $ make
    $ cd ../../
    $ make
    $ ./main
