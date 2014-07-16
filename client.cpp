#include "zmq.hpp"
#include <iostream>
#include <sstream>
#include "message.pb.h"

static char *
s_recv (void *socket) {
    char buffer [256];
    int size = zmq_recv (socket, buffer, 255, 0);
    if (size == -1)
        return NULL;
    if (size > 255)
        size = 255;
    buffer [size] = 0;
    return strdup (buffer);
}

int main (int argc, char *argv[])
{
    //  Socket to talk to server
    printf ("Collecting updates from weather server...\n");
    void *context = zmq_ctx_new ();
    void *subscriber = zmq_socket (context, ZMQ_SUB);
    int rc = zmq_connect (subscriber, "tcp://localhost:8231");
    assert (rc == 0);

    //  Subscribe to zipcode, default is NYC, 10001
    const char *filter = "";
    rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE,
                         filter, strlen (filter));
    assert (rc == 0);

    while(1) {
    	stocksim::Stock stock_msg;
        char *string = s_recv (subscriber);

        printf("string buf size: %d %d\n", sizeof(string), strlen(string));
        bool ret = stock_msg.ParseFromArray(string, strlen(string));
        std::cout << "code: " << stock_msg.code() << std::endl;
        std::cout << "name: " << stock_msg.name() << std::endl;
        std::cout << "in_price: " << stock_msg.in_price() << std::endl;
        std::cout << "out_price: " << stock_msg.out_price() << std::endl;
        std::cout << "trade: " << stock_msg.trade() << std::endl;
        free (string);
    }
    zmq_close (subscriber);
    zmq_ctx_destroy (context);
    return 0;
}
