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
struct Player2_defs{
	struct gameActionOut : public out_port<GameAction_t> {};
	struct playGameIn : public in_port<PlayGame_t> {};
};


/****** RequestReceiver atomic model instantiation ******/
shared_ptr<dynamic::modeling::model> requestReceiver2 = dynamic::translate::make_dynamic_atomic_model<RequestReceiver,TIME>("requestReceiver2");

/****** ActionMaker atomic model instantiations ****************/
shared_ptr<dynamic::modeling::model> actionMaker2 = dynamic::translate::make_dynamic_atomic_model<ActionMaker,TIME>("actionMaker2");

/*******Player MODEL********/
dynamic::modeling::Ports iports_Player2 = {typeid(Player2_defs::playGameIn)};
dynamic::modeling::Ports oports_Player2 = {typeid(Player2_defs::gameActionOut)};
dynamic::modeling::Models submodels_Player2 = {requestReceiver2, actionMaker2};
dynamic::modeling::EICs eics_Player2 = {
	dynamic::translate::make_EIC<Player2_defs::playGameIn,RequestReceiver_defs::playGameIn>("requestReceiver2")
};
dynamic::modeling::EOCs eocs_Player2 = {
	dynamic::translate::make_EOC<ActionMaker_defs::gameActionOut,Player2_defs::gameActionOut>("actionMaker2")
};
dynamic::modeling::ICs ics_Player2 = {
    dynamic::translate::make_IC<RequestReceiver_defs::playGameOut, ActionMaker_defs::playGameIn>("requestReceiver2","actionMaker2")
};