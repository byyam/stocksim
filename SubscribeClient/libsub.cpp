#include "zmq.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include "message.pb.h"
#include "subapi.h"

#define BUFF_LEN 256
#define HEAD_LEN 8

static char *
s_recv (void *socket) {
    char buffer [BUFF_LEN];
    int size = zmq_recv (socket, buffer, BUFF_LEN - 1, 0);
    if (size == -1)
        return NULL;
    if (size > BUFF_LEN - 1)
        size = BUFF_LEN - 1;
    buffer [size] = 0;
    return strdup (buffer);
}

static void *
print_byte(char *buf, int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (i % 4 == 0 ) {
            printf("\n");
        }
        printf("%08x ", *(buf + i));
    }
    printf("\n\n");
}

int sub_stock (std::string &conn, std::vector<std::string> &sub_list)
{
    void *context = zmq_ctx_new ();
    void *subscriber = zmq_socket (context, ZMQ_SUB);
    int rc = zmq_connect (subscriber, conn.c_str());
    assert (rc == 0);

    std::vector<std::string>::iterator it;
    for (it = sub_list.begin(); it != sub_list.end(); it++) {
        const char *filter = (*it).c_str();
        rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE,
                         filter, strlen (filter));
        assert (rc == 0);
    }

    //const char *filter = "8008";
    //rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE,
    //                     filter, strlen (filter));
    //assert (rc == 0);

    while(1) {
    	stocksim::Stock stock_msg;
        char *buff = s_recv (subscriber);
        int buff_len = strlen(buff) + 1;
        int offset = HEAD_LEN;
        int str_len = buff_len - offset;
        printf("buff_len: %d\n", buff_len);
        printf("offset: %d\n", offset);
        printf("str_len: %d\n", str_len);
        char *body = (char *)malloc(str_len);
        char *header = (char *)malloc(offset);
        memset(body, 0, str_len);
        memset(header, 0, offset);
        memcpy(body, buff + offset, str_len);
        memcpy(header, buff, offset);

        //print_byte(buff, buff_len);
        //print_byte(body, str_len);
        //print_byte(header, offset);

        bool ret = stock_msg.ParseFromArray(body, strlen(body));
        std::cout << "code: " << stock_msg.code() << std::endl;
        std::cout << "name: " << stock_msg.name() << std::endl;
        std::cout << "in_price: " << stock_msg.in_price() << std::endl;
        std::cout << "out_price: " << stock_msg.out_price() << std::endl;
        std::cout << "trade: " << stock_msg.trade() << std::endl;
        free (body);
        free (buff);
    }
    zmq_close (subscriber);
    zmq_ctx_destroy (context);
    return 0;
}

/*
int main(int argc, char const *argv[])
{
    std::vector<std::string> sub_list;
    sub_list.push_back("8001");
    sub_list.push_back("8008");
    sub_stock(sub_list);
    return 0;
}
*/
