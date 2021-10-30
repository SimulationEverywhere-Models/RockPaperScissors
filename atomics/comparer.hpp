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
        vector <int> winnerTracker; //tracks the scores of the past rounds
        int leading;
        int leadingDisplay;
        int playerIDDisplay;
    }; 
    state_type state;    
    // default constructor
    Comparer() {
        state.active = false; //set to true when input received
        state.playerResult1 = -1; //default setting, meaning no response yet from player
        state.playerResult2 = -1; //default setting, meaning no response yet from player
        state.received1 = false; //set to true when plyer response received
        state.received2 = false; //set to true when plyer response received
        state.playerIDWin = -1; //no winner selected yet
        state.winnerTracker;
        state.leading=0;
        state.leadingDisplay=0;
        state.playerIDDisplay=0;
    }     
    // internal transition
    //1-rock, 2- paper, 3 -scissors
    void internal_transition() {
        //return to default settings
        
        state.active = false;
        state.received1 = false;
        state.received2 = false;
        state.playerIDWin = -1; 
        state.playerResult1 = -1;
        state.playerResult2 = -1;
        state.playerIDDisplay = -1;
        state.leadingDisplay= -1;
        

            
        
    }
    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        if (get_messages<typename Comparer_defs::playGameStartIn>(mbs).size() > 1) {
            assert(false && "One game at a time!"); //Make sure more than one game is not started at once
        }

        //if current game is ongoing, refuse
        if (get_messages<typename Comparer_defs::playGameStartIn>(mbs).size() == 1) {
            //trigger game to start
            if (state.active == false) {
                state.active = true;
            }
        }
        //receiving player input
        if (get_messages<typename Comparer_defs::gameActionIn1>(mbs).size() == 1) {
            state.active = true;
            if (state.active == true && state.received1 == false){
                vector<GameAction_t> player1;
                player1 = get_messages<typename Comparer_defs::gameActionIn1>(mbs); //receive input from player 1

                state.playerResult1 = player1[0].choice;
                state.received1 = true;
            }
        }

        //receiving player input
        if (get_messages<typename Comparer_defs::gameActionIn2>(mbs).size() == 1) {
            state.active = true;
            if (state.active == true && state.received2 == false) {
                vector<GameAction_t> player2;
                player2 = get_messages<typename Comparer_defs::gameActionIn2>(mbs); //receive input from player 2

                state.playerResult2 = player2[0].choice;
                state.received2 = true;
            }
        }
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

            //append the winner to winnerTracker vector
            state.winnerTracker.push_back(state.playerIDWin);

            //count who is leading the games
            int player1Counter = 0;
            int player2Counter = 0;
            for (int i : state.winnerTracker) {
                if (i == 1) {
                    player1Counter++;

                }
                if (i == 2) {
                    player2Counter++;
                }
                std::cout << i;
            }
            if (player1Counter > player2Counter) {
                state.leading = 1;
            }
            else if (player1Counter < player2Counter) {
                state.leading = 2;
            }
            else {
                state.leading = 0;
            }


        }


    }
    // confluence transition
    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        internal_transition();
        external_transition(TIME(), move(mbs));
    }
    // output function
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
        if (state.active == true && state.received1 == true && state.received2 == true && state.playerIDWin != -1) {
            vector<WinReport_t> report;
            report.push_back(state.playerIDWin);
            get_messages<typename Comparer_defs::winReportOut>(bags) = report;

            state.playerIDDisplay = state.playerIDWin;
            state.leadingDisplay = state.leading;
            
        }
        else if (state.active == true && state.received1 == false && state.received2 == false) {
            vector<PlayGame_t> playerTrigger1;
            vector<PlayGame_t> playerTrigger2;

            //trigger players to provide response, tell them game has begun
            //playerTrigger1.push_back(true);
            //playerTrigger2.push_back(true);

            get_messages<typename Comparer_defs::playGameOut1>(bags) = playerTrigger1;
            get_messages<typename Comparer_defs::playGameOut2>(bags) = playerTrigger2;
            return bags;
            
        }

        return bags;
    }
    // time_advance function
    TIME time_advance() const {
        TIME next_internal;
        if (state.active == true && state.received1 == false && state.received2 == false) {
            next_internal = TIME("00:00:05:000"); //referee time to alert players
        }
        else if (state.active == true && state.received1 == true && state.received2 == true) {
            next_internal = TIME("00:00:15:000"); //referee time to make winning decision
        }
        else {
            next_internal = numeric_limits<TIME>::infinity();
        }    
        return next_internal;
    }

    friend ostringstream& operator<<(ostringstream& os, const typename Comparer<TIME>::state_type& i) {
        os << "Active? : " << i.active << " & Response from P1 : " << i.playerResult1 << " & Response from P2 : " << i.playerResult2 << " & winner is player : " << i.playerIDDisplay << " Current round leader is: " << i.leadingDisplay;
        return os;
    }
  
};    
#endif // _COMPARER_HPP__