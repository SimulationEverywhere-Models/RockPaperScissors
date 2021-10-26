//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>

//Messages structures
#include "../data_structures/playGame.hpp"
#include "../data_structures/gameOption.hpp"
#include "../data_structures/winReport.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../atomics/comparer.hpp"
#include "../atomics/requestReceiver.hpp"
#include "../atomics/actionMaker.hpp"


//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

//Player definition
struct Player_defs{
	struct gameActionOut : public out_port<GameAction_t> {};
	struct playGameIn : public in_port<PlayGame_t> {};
};

/****** RequestReceiver atomic model instantiation ******/
shared_ptr<dynamic::modeling::model> requestReceiver = dynamic::translate::make_dynamic_atomic_model<RequestReceiver,TIME>("requestReceiver");

/****** ActionMaker atomic model instantiations ****************/
shared_ptr<dynamic::modeling::model> actionMaker = dynamic::translate::make_dynamic_atomic_model<ActionMaker,TIME>("actionMaker");

/*******Player MODEL********/
dynamic::modeling::Ports iports_Player = {typeid(Player_defs::playGameIn)};
dynamic::modeling::Ports oports_Player = {typeid(Player_defs::gameActionOut)};
dynamic::modeling::Models submodels_Player = {requestReceiver, actionMaker};
dynamic::modeling::EICs eics_Player = {
	dynamic::translate::make_EIC<Player_defs::playGameIn,RequestReceiver_defs::playGameIn>("requestReceiver")
};
dynamic::modeling::EOCs eocs_Player = {
	dynamic::translate::make_EOC<ActionMaker_defs::gameActionOut,Player_defs::gameActionOut>("actionMaker")
};
dynamic::modeling::ICs ics_Player = {
    dynamic::translate::make_IC<RequestReceiver_defs::playGameOut, ActionMaker_defs::playGameIn>("requestReceiver","actionMaker")
};