#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include "subapi.h"

int main(int argc, char const *argv[])
{
    std::vector<std::string> sub_list;
    std::string conn = "tcp://localhost:8231";
    char code[5] = "8000";
    char c;
    int i;
    for (i = 0; i < 10; i++) {
    	printf("[%d] name: stock_%d\n", i, i);
	}
	std::cout << "choose your stock, split by space (end with Enter): " << std::endl;
    for(;;){
    	c = getchar();
    	if (c == 10) {
    		std::cout << "quit" << std::endl;
    		break;
    	}
    	code[3] = c;
    	sub_list.push_back(code);
    }
    std::cout << "thanks for your subscription!" << std::endl;
    //sub_list.push_back("8001");
    //sub_list.push_back("8008");
    sub_stock(conn, sub_list);
    return 0;
}
