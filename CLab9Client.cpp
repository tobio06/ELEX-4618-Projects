#include "stdafx.h"
#include "CLab9Client.h"
#include <iostream>

CLab9Client::CLab9Client(std::string ip_address, int port)
   {
   _client.connect_socket(ip_address, port);
   }

void CLab9Client::print_menu()
   {
   std::cout << "\n************************************************";
   std::cout << "\n* ELEX4618 Lab 9: Socket with Recycling Sorter";
   std::cout << "\n************************************************";
   std::cout << "\n(1) Auto Mode";
   std::cout << "\n(2) Manual Mode";
   std::cout << "\n(3) Sort Red";
   std::cout << "\n(4) Sort Blue";
   std::cout << "\n(5) Sort Green";
   std::cout << "\n(6) Sort Purple";
   std::cout << "\n(7) Get Bin Counts";
   std::cout << "\n(8) Get Image";
   std::cout << "\n(0) Exit";
   std::cout << "\nCMD> ";
   }


void CLab9Client::auto_mode()
   {
   _client.tx_str("S 0 1\n");
   std::cout << "\nAuto Mode ON";
   }

void CLab9Client::manual_mode()
   {
   _client.tx_str("S 0 0\n");
   std::cout << "\nManual Mode ON";
   }


void CLab9Client::sort_red()
   {
   _client.tx_str("S 1 0\n");
   std::cout << "\nSorting Red...";
   }

void CLab9Client::sort_blue()
   {
   _client.tx_str("S 1 1\n");
   std::cout << "\nSorting Blue...";
   }

void CLab9Client::sort_green()
   {
   _client.tx_str("S 1 2\n");
   std::cout << "\nSorting Green...";
   }

void CLab9Client::sort_purple()
   {
   _client.tx_str("S 1 3\n");
   std::cout << "\nSorting Purple...";
   }

void CLab9Client::get_bin_counts()
   {
   std::string response;

   _client.tx_str("G 1 0\n");
   if (_client.rx_str(response))
      {
      std::cout << "\nBin 1 Count: " << response;
      }
   _client.tx_str("G 1 1\n");
   if (_client.rx_str(response))
      {
      std::cout << "\nBin 2 Count: " << response;
      }
   _client.tx_str("G 1 2\n");
   if (_client.rx_str(response))
      {
      std::cout << "\nBin 3 Count: " << response;
      }
   _client.tx_str("G 1 3\n");
   if (_client.rx_str(response))
      {
      std::cout << "\nBin 4 Count: " << response;
      }
   else
      {
      std::cout << "\nFailed to get bin counts.";
      }
   }


void CLab9Client::get_image()
   {
   while (true)
      {
         _client.tx_str("im");

         cv::Mat image;
         if (_client.rx_im(image))
         {
            cv::imshow("Live", image);
            cv::waitKey(1);
         }
      }
   }


void CLab9Client::run()
   {
   int cmd = -1;
   do
      {
      print_menu();
      std::cin >> cmd;

      switch (cmd)
         {
         case 1: auto_mode(); break;
         case 2: manual_mode(); break;
         case 3: sort_red(); break;
         case 4: sort_blue(); break;
         case 5: sort_green(); break;
         case 6: sort_purple(); break;
         case 7: get_bin_counts(); break;
         case 8: get_image(); break;
         }
      } while (cmd != 0);
   }