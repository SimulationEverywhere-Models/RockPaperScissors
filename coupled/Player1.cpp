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
struct Player1_defs{
	struct gameActionOut : public out_port<GameAction_t> {};
	struct playGameIn : public in_port<PlayGame_t> {};
};


/****** RequestReceiver atomic model instantiation ******/
shared_ptr<dynamic::modeling::model> requestReceiver1 = dynamic::translate::make_dynamic_atomic_model<RequestReceiver,TIME>("requestReceiver1");

/****** ActionMaker atomic model instantiations ****************/
shared_ptr<dynamic::modeling::model> actionMaker1 = dynamic::translate::make_dynamic_atomic_model<ActionMaker,TIME>("actionMaker1");

/*******Player MODEL********/
dynamic::modeling::Ports iports_Player1 = {typeid(Player1_defs::playGameIn)};
dynamic::modeling::Ports oports_Player1 = {typeid(Player1_defs::gameActionOut)};
dynamic::modeling::Models submodels_Player1 = {requestReceiver1, actionMaker1};
dynamic::modeling::EICs eics_Player1 = {
	dynamic::translate::make_EIC<Player1_defs::playGameIn,RequestReceiver_defs::playGameIn>("requestReceiver1")
};
dynamic::modeling::EOCs eocs_Player1 = {
	dynamic::translate::make_EOC<ActionMaker_defs::gameActionOut,Player1_defs::gameActionOut>("actionMaker1")
};
dynamic::modeling::ICs ics_Player1 = {
    dynamic::translate::make_IC<RequestReceiver_defs::playGameOut, ActionMaker_defs::playGameIn>("requestReceiver1","actionMaker1")
};