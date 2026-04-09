#pragma once
#include "Client.h"
#include "opencv2/core.hpp"

#define IP_ADDRESS "192.168.137.148"
#define PORT 4618

class CLab9Client
    {
    protected:
       CClient _client;

    public:
       CLab9Client(std::string ip_address, int port);
       //~CLab9Client();

       void print_menu();

       void auto_mode();
       void manual_mode();

       void sort_red();
       void sort_blue();
       void sort_green();
       void sort_purple();

       void get_bin_counts();

       void get_image();

       void run();
    };

