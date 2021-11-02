//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
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

//Coupled Models
#include "../coupled/Player1.cpp"


//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

/***** Define input port for coupled models *****/
//not needed here, since we are loading input data

/***** Define output ports for coupled model *****/
struct top_out : public out_port<GameAction_t> {};

/****** Input Reader for atomic model declaration *******************/
template<typename T>
class InputReader_PlayGame_t : public iestream_input<PlayGame_t, T> {
public:
    InputReader_PlayGame_t() = default;
    InputReader_PlayGame_t(const char* file_path) : iestream_input<PlayGame_t, T>(file_path) {}
};

int main(){

    /****** Input Game Trigger atomic model instantiation *******************/
    const char* i_input_data = "../input_data/requestReceiver_input_test.txt";
    shared_ptr<dynamic::modeling::model> input_reader;
    input_reader = dynamic::translate::make_dynamic_atomic_model<InputReader_PlayGame_t, TIME, const char*>("input_reader", move(i_input_data));

	/****** Player coupled model instantiation *******************/
	shared_ptr<dynamic::modeling::coupled<TIME>> Player1 = make_shared<dynamic::modeling::coupled<TIME>>("Player1", submodels_Player1, iports_Player1, oports_Player1, eics_Player1, eocs_Player1, ics_Player1);

	/*******TOP MODEL********/
    dynamic::modeling::Ports iports_TOP = {};
    dynamic::modeling::Ports oports_TOP = {typeid(top_out)};
    dynamic::modeling::Models submodels_TOP = {input_reader,Player1};
    dynamic::modeling::EICs eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP = {
        dynamic::translate::make_EOC<Player1_defs::gameActionOut,top_out>("Player1")
		};
    dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<PlayGame_t>::out,Player1_defs::playGameIn>("input_reader","Player1")
	};
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;

    TOP = make_shared<dynamic::modeling::coupled<TIME>>("TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP);

    /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/COUPLED_TEST_PLAYER1_outputs.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/COUPLED_TEST_PLAYER1_state.txt");
    struct oss_sink_state{
        static ostream& sink(){          
            return out_state;
        }
    };
    
    using state=logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>, oss_sink_state>;
    using log_messages=logger::logger<logger::logger_messages, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_mes=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_sta=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_state>;
    using logger_top=logger::multilogger<state, log_messages, global_time_mes, global_time_sta>;

    /************** Runner call ************************/ 
    dynamic::engine::runner<NDTime, logger_top> r(TOP,{0});
    r.run_until(NDTime("00:10:00:000"));
    return 0;
}