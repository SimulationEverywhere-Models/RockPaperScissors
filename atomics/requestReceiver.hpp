/**
* Muhammad Salman - 100995527
* SYSC5104F - Carleton University
*
* Request Receiver:
* Cadmium implementation of CD++ Request Receiver atomic model
*/

#ifndef _REQUESTRECEIVER_HPP__
#define _REQUESTRECEIVER_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <random>

#include "../data_structures/playGame.hpp"

using namespace cadmium;
using namespace std;

//Port definition
struct RequestReceiver_defs{
    struct playGameOut : public out_port<PlayGame_t> {};
    struct playGameIn : public in_port<PlayGame_t> {};
};

template<typename TIME> 
class RequestReceiver{
    public:
    // ports definition
    using input_ports=tuple<typename RequestReceiver_defs::playGameIn>;
    using output_ports=tuple<typename RequestReceiver_defs::playGameOut>;
    // state definition
    struct state_type{
        bool active; //triggered to true when playGameStartIn received
        bool sent; //triggered to true when command sent to actionMaker to prepare command 
    }; 
    state_type state;    
    // default constructor
    RequestReceiver() {
        state.active = false; //true if game request from Comparer has been received
        state.sent = false; // true if game request has been sent from RequestReceiver to ActionMaker
    }     
    // internal transition
    void internal_transition() {
        std::cout << "helloChangeState";
        if (state.active == true && state.sent == false) {
            state.sent = true; //variable to signify request has been sent
        }
 
    }
    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        if (get_messages<typename RequestReceiver_defs::playGameIn>(mbs).size() > 1) {
            assert(false && "One request at a time!"); //Make sure more than one request is not made at the same time
        }
        else {
            if (state.active == false) {
                state.active = true;
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
        std::cout << "helloOUTPUT ";
        if (state.active == true) {
            vector<PlayGame_t> playerTrigger;

            //tell next model to stay ready to make decision
            playerTrigger.push_back(true);

            get_messages<typename RequestReceiver_defs::playGameOut>(bags) = playerTrigger;
            return bags;
            
        }
        
    }
    // time_advance function
    TIME time_advance() const {
        TIME next_internal;
        if (state.active == true) {
            next_internal = TIME("00:00:20:000"); //decision making time
        }else {
            next_internal = numeric_limits<TIME>::infinity();
        }    
        return next_internal;
    }

    friend ostringstream& operator<<(ostringstream& os, const typename RequestReceiver<TIME>::state_type& i) {
        os << "Active? : " << i.active << " Sent? :"<<i.sent;
        return os;
    }
};    
#endif // _REQUESTRECEIVER_HPP__