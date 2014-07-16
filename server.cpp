#include "zmq.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "message.pb.h"


static int
s_send (void *socket, char *string) {
    int size = zmq_send (socket, string, strlen (string), 0);
    return size;
}

int main () {
    void *context = zmq_ctx_new ();
    void *publisher = zmq_socket (context, ZMQ_PUB);
    int rc = zmq_bind (publisher, "tcp://*:8231");
    assert (rc == 0);

    while (1) {
        stocksim::Stock stock_msg;
        stock_msg.set_code("123");
        stock_msg.set_name("stockname");
        stock_msg.set_in_price(0.1);
        stock_msg.set_out_price(0.2);
        stock_msg.set_trade(100);
        sleep(1);
        int size = stock_msg.ByteSize();
        char *buffer = (char *)malloc(size);
        stock_msg.SerializeToArray(buffer, size);
        std::cout << "send msg" << std::endl;
        s_send (publisher, buffer);
    }

    zmq_close (publisher);
    zmq_ctx_destroy (context);
    return 0;
}
