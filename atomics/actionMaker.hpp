/**
* Muhammad Salman - 100995527
* SYSC5104F - Carleton University
*
* Request Receiver:
* Cadmium implementation of CD++ Request Receiver atomic model
*/

#ifndef _ACTIONMAKER_HPP__
#define _ACTIONMAKER_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <time.h>
#include <string>
#include <random>
#include <ctime>

#include "../data_structures/playGame.hpp"
#include "../data_structures/gameOption.hpp"

using namespace cadmium;
using namespace std;

//Port definition
struct ActionMaker_defs{
    struct gameActionOut : public out_port<GameAction_t> {};
    struct playGameIn : public in_port<PlayGame_t> {};
};

template<typename TIME> 
class ActionMaker{
    public:
    // ports definition
    using input_ports=tuple<typename ActionMaker_defs::playGameIn>;
    using output_ports=tuple<typename ActionMaker_defs::gameActionOut>;
    // state definition
    struct state_type{
        bool active; //triggered to true when playGameIn received
        int choice; //player choice
    }; 
    state_type state;    
    // default constructor
    ActionMaker() {
        state.active = false; // set to true once game request received
        state.choice = -1; //default is -1, means no choice, otherwise 1-3
    }     
    // internal transition
    void internal_transition() {
        state.active = false;
        state.choice = -1;
 
    }
    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        if (get_messages<typename ActionMaker_defs::playGameIn>(mbs).size() > 1) {
            assert(false && "One request at a time!"); //Make sure more than one request is not made at the same time
        }
        else {
            if (state.active == false) {
                state.active = true;
                std::random_device rand;
                std::mt19937 generate(rand());
                std::uniform_int_distribution<> distribute(1, 3);
                state.choice = distribute(generate); //generate value between 1-3

                
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
        if (state.active == true) {
            vector<GameAction_t> gameChoice;

            //output choice
            gameChoice.push_back(state.choice);
            get_messages<typename ActionMaker_defs::gameActionOut>(bags) = gameChoice;
        }
        return bags; 
    }
    // time_advance function
    TIME time_advance() const {
        TIME next_internal;
        if (state.active) {
            next_internal = TIME("00:00:03:000"); //time to provide signal
        }else {
            next_internal = numeric_limits<TIME>::infinity();
        }    
        return next_internal;
    }

    friend ostringstream& operator<<(ostringstream& os, const typename ActionMaker<TIME>::state_type& i) {
        os << "Active? : " << i.active;
        return os;
    }
};    
#endif // _REQUESTRECEIVER_HPP__