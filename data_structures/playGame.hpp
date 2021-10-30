#ifndef BOOST_SIMULATION_PLAYGAME_HPP
#define BOOST_SIMULATION_PLAYGAME_HPP

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

/*******************************************/
/**************** PlayGame_t ****************/
/*******************************************/
struct PlayGame_t {
    PlayGame_t() {}
    PlayGame_t(bool i_isTriggerGame)
        :isTriggerGame(i_isTriggerGame) {}
    bool isTriggerGame;
};

istream& operator>> (istream& is, PlayGame_t& msg);

ostream& operator<<(ostream& os, const PlayGame_t& msg);


#endif // BOOST_SIMULATION_PLAYGAME_HPP