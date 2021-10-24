#ifndef BOOST_SIMULATION_WINREPORT_HPP
#define BOOST_SIMULATION_WINREPORT_HPP

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

/*******************************************/
/**************** WinReport_t ****************/
/*******************************************/
struct WinReport_t{
  WinReport_t(){}
  WinReport_t(int i_winner)
   :winner(i_winner){}
  	int winner;
};

istream& operator>> (istream& is, WinReport_t& msg);

ostream& operator<<(ostream& os, const WinReport_t& msg);


#endif // BOOST_SIMULATION_WINREPORT_HPP