#ifndef BOOST_SIMULATION_GAMEOPTION_HPP
#define BOOST_SIMULATION_GAMEOPTION_HPP

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

/*******************************************/
/**************** GameAction_t ****************/
/*******************************************/
struct GameAction_t{
  GameAction_t(){}
  GameAction_t(int i_choice)
   :choice(i_choice){}
  	int choice;
};

istream& operator>> (istream& is, GameAction_t& msg);

ostream& operator<<(ostream& os, const GameAction_t& msg);

#endif // BOOST_SIMULATION_GAMEOPTION_HPP