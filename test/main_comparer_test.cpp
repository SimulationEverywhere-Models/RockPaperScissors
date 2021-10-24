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

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

/***** Define input port for coupled models *****/
struct gameActionIn1 : public in_port<GameAction_t> {};
struct gameActionIn2 : public in_port<GameAction_t> {};
struct playGameStartIn : public in_port<PlayGame_t> {};

/***** Define output ports for coupled model *****/
struct playGameOut1 : public out_port<PlayGame_t> {};
struct playGameOut2 : public out_port<PlayGame_t> {};
struct winReportOut : public out_port<WinReport_t> {};


/****** Input PlayGameTrigger atomic model declaration *******************/
template<typename T>
class InputReader_PlayGame_t : public iestream_input<PlayGame_t,T> {
    public:
        InputReader_PlayGame_t () = default;
        InputReader_PlayGame_t (const char* file_path) : iestream_input<PlayGame_t,T>(file_path) {}
};

/****** Input PlayerChoice atomic model declaration *******************/
template<typename T>
class InputReader_GameAction_t : public iestream_input<GameAction_t, T> {
public:
    InputReader_GameAction_t() = default;
    InputReader_GameAction_t(const char* file_path) : iestream_input<GameAction_t, T>(file_path) {}
};
int main(){

    /****** Input PlayerChoice atomic model instantiation *******************/
    const char * i_input_start_data = "../input_data/start_game_input_test.txt";
    shared_ptr<dynamic::modeling::model> input_reader_play_game;
    input_reader_play_game = dynamic::translate::make_dynamic_atomic_model<InputReader_PlayGame_t, TIME, const char*>("input_reader_play_game", move(i_input_start_data));

    /****** Input PlayGameTrigger atomic model instantiation (Player1) *******************/
    const char* i_input_player1_data = "../input_data/player1_choice_input_test.txt";
    shared_ptr<dynamic::modeling::model> input_reader_player1_choice;
    input_reader_player1_choice = dynamic::translate::make_dynamic_atomic_model<InputReader_GameAction_t, TIME, const char*>("input_reader_player1_choice", move(i_input_player1_data));

    /****** Input PlayGameTrigger atomic model instantiation (Player2) *******************/
    const char* i_input_player2_data = "../input_data/player2_choice_input_test.txt";
    shared_ptr<dynamic::modeling::model> input_reader_player2_choice;
    input_reader_player2_choice = dynamic::translate::make_dynamic_atomic_model<InputReader_GameAction_t, TIME, const char*>("input_reader_player2_choice", move(i_input_player2_data));

    /****** Comparer atomic model instantiation *******************/
    shared_ptr<dynamic::modeling::model> comparer1;
    comparer1 = dynamic::translate::make_dynamic_atomic_model<Comparer, TIME>("comparer1");

    /*******TOP MODEL********/
    dynamic::modeling::Ports iports_TOP;
    iports_TOP = {};
    dynamic::modeling::Ports oports_TOP;
    oports_TOP = {typeid(playGameOut1), typeid(playGameOut2), typeid(winReportOut)};
    dynamic::modeling::Models submodels_TOP;
    submodels_TOP = {input_reader_play_game, input_reader_player1_choice, input_reader_player2_choice, comparer1};
    dynamic::modeling::EICs eics_TOP;
    eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP;
    eocs_TOP = {
        dynamic::translate::make_EOC<Comparer_defs::playGameOut1,playGameOut1>("PlayGameOutTrigger1"),
        dynamic::translate::make_EOC<Comparer_defs::playGameOut2,playGameOut2>("PlayGameOutTrigger2"),
        dynamic::translate::make_EOC<Comparer_defs::winReportOut,winReportOut>("WinReport"),

    };
    dynamic::modeling::ICs ics_TOP;
    ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<PlayGame_t>::out,Comparer_defs::playGameStartIn>("input_reader_play_game","comparer1"),
        dynamic::translate::make_IC<iestream_input_defs<GameAction_t>::out,Comparer_defs::gameActionIn1>("input_reader_player1_choice","comparer1"),
        dynamic::translate::make_IC<iestream_input_defs<GameAction_t>::out,Comparer_defs::gameActionIn2>("input_reader_player2_choice","comparer1")
    };
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP 
    );

    /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/comparer_test_output_messages.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/comparer_test_output_state.txt");
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
    dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    r.run_until(NDTime("00:10:00:000"));
    return 0;
}
