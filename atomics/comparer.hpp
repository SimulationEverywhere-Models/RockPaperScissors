/**
* Muhammad Salman - 100995527
* SYSC5104F - Carleton University
*
* Comparer:
* Cadmium implementation of CD++ Comparer atomic model
*/

#ifndef _COMPARER_HPP__
#define _COMPARER_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <random>

#include "../data_structures/playGame.hpp"
#include "../data_structures/gameOption.hpp"
#include "../data_structures/winReport.hpp"

using namespace cadmium;
using namespace std;

//Port definition
struct Comparer_defs{
    struct playGameOut1 : public out_port<PlayGame_t> {};
    struct playGameOut2 : public out_port<PlayGame_t> {};
    struct winReportOut : public out_port<WinReport_t> {};
    struct gameActionIn1 : public in_port<GameAction_t> {};
    struct gameActionIn2 : public in_port<GameAction_t> {};
    struct playGameStartIn : public in_port<PlayGame_t> {};
};

template<typename TIME> 
class Comparer{
    public:
    // ports definition
    using input_ports=tuple<typename Comparer_defs::gameActionIn1, typename Comparer_defs::gameActionIn2, typename Comparer_defs::playGameStartIn>;
    using output_ports=tuple<typename Comparer_defs::playGameOut1, typename Comparer_defs::playGameOut2, typename Comparer_defs::winReportOut>;
    // state definition
    struct state_type{
        bool active; //triggered to true when playGameStartIn received
        int playerResult1; //Option chosen by player1
        int playerResult2; //Option chosen by player2
        bool received1; //triggered to true when player1 sends back response
        bool received2; //triggered to true when player2 sends back response
        int playerIDWin; //the ID of the player that won the match
    }; 
    state_type state;    
    // default constructor
    Comparer() {
        state.active = false;
        state.playerResult1 = -1;
        state.playerResult2 = -1;
        state.received1 = false;
        state.received2 = false;
        state.playerIDWin = -1;
    }     
    // internal transition
    //1-rock, 2- paper, 3 -scissors
    void internal_transition() {
        if (state.active == true && state.received1 == true && state.received2 == true) {
            if (state.playerResult1 == state.playerResult2) {
                state.playerIDWin = 0;
            }
            else if (state.playerResult1 == 1 && state.playerResult2 == 2) {
                state.playerIDWin = 2;
            }
            else if (state.playerResult1 == 1 && state.playerResult2 == 3) {
                state.playerIDWin = 1;
            }
            else if (state.playerResult1 == 2 && state.playerResult2 == 1) {
                state.playerIDWin = 1;
            }
            else if (state.playerResult1 == 2 && state.playerResult2 == 3) {
                state.playerIDWin = 2;
            }
            else if (state.playerResult1 == 3 && state.playerResult2 == 1) {
                state.playerIDWin = 2;
            }
            else if (state.playerResult1 == 3 && state.playerResult2 == 2) {
                state.playerIDWin = 1;
            }
        }
        //for an internal transition to happen, the three variabless in the if statement must be true
        /*
        if (state.active == true && state.playerIDWin != -1) {
            state.active = false;
            state.received1 = false;
            state.received2 = false;
        }*/
    }
    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
        if (get_messages<typename Comparer_defs::playGameStartIn>(mbs).size() > 1) {
            assert(false && "One game at a time!"); //Make sure more than one game is not started at once
        }
        else {
            if (state.active == false && state.playerIDWin == -1) {
                state.active = true;
            }
            else {
                assert(false && "One gaame at a time, a match is pending!"); //Make sure more current game is not interrupted
            }
        }

        vector<GameAction_t> player1;
        vector<GameAction_t> player2;
        player1 = get_messages<typename Comparer_defs::gameActionIn1>(mbs); //receive input from player 1
        player2 = get_messages<typename Comparer_defs::gameActionIn2>(mbs); //receive input from player 2
        std::cout << "Thanks for viewing my code!";

        state.playerResult1 = player1[0];
        state.received1 = true;

        state.playerResult2 = player2[0];
        state.received2 = true;

    }
    // confluence transition
    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        internal_transition();
        external_transition(TIME(), move(mbs));
    }
    // output function
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
        if (state.active == true && state.received1 == true && state.received2 == true) {
            vector<WinReport_t> report;
            report.push_back(state.playerIDWin);
            get_messages<typename Comparer_defs::winReportOut>(bags) = report;
        }
        else if (state.active == true && state.received1 == false && state.received2 == false) {
            vector<PlayGame_t> playerTrigger1;
            vector<PlayGame_t> playerTrigger2;

            //trigger players to provide response, tell them game has begun
            playerTrigger1.push_back(true);
            playerTrigger2.push_back(true);

            get_messages<typename Comparer_defs::playGameOut1>(bags) = playerTrigger1;
            get_messages<typename Comparer_defs::playGameOut2>(bags) = playerTrigger2;
        }
        return bags;
    }
    // time_advance function
    TIME time_advance() const {
        TIME next_internal;
        if (state.active == true && state.received1 == true && state.received2 == true) {
            next_internal = TIME("00:00:05:000"); //referee decision time
        }else {
            next_internal = numeric_limits<TIME>::infinity();
        }    
        return next_internal;
    }

    friend ostringstream& operator<<(ostringstream& os, const typename Comparer<TIME>::state_type& i) {
        os << "Active? : " << i.active << " & Response from P1 : " << i.playerResult1 <<" & Response from P2 : " << i.playerResult2 << " & Response from P2 : " << i.playerIDWin;
        return os;
    }
  
};    
#endif // _COMPARER_HPP__