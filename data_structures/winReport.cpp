#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "winReport.hpp"

/***************************************************/
/************* Output stream ************************/
/***************************************************/

ostream& operator<<(ostream& os, const WinReport_t& msg) {
  os << msg.winner;
  return os;
}

/***************************************************/
/************* Input stream ************************/
/***************************************************/

istream& operator>> (istream& is, WinReport_t& msg) {
  is >> msg.winner;
  return is;
}
