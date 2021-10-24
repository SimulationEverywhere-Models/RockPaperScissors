#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "playGame.hpp"

/***************************************************/
/************* Output stream ************************/
/***************************************************/

ostream& operator<<(ostream& os, const PlayGame_t& msg) {
  os << msg.isTriggerGame;
  return os;
}

/***************************************************/
/************* Input stream ************************/
/***************************************************/

istream& operator>> (istream& is, PlayGame_t& msg) {
  is >> msg.isTriggerGame;
  return is;
}
