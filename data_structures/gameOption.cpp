#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "gameOption.hpp"

/***************************************************/
/************* Output stream ************************/
/***************************************************/

ostream& operator<<(ostream& os, const GameAction_t& msg) {
  os << msg.choice;
  return os;
}

/***************************************************/
/************* Input stream ************************/
/***************************************************/

istream& operator>> (istream& is, GameAction_t& msg) {
  is >> msg.choice;
  return is;
}
