CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I C:\Cadmium-Simulation-Environment\cadmium\include
INCLUDEDESTIMES=-I C:\Cadmium-Simulation-Environment\DESTimes\include

#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

#TARGET TO COMPILE ALL THE TESTS TOGETHER 
gameOption.o: data_structures/gameOption.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/gameOption.cpp -o build/gameOption.o

playGame.o: data_structures/playGame.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/playGame.cpp -o build/playGame.o

winReport.o: data_structures/winReport.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/winReport.cpp -o build/winReport.o

main_comparer_test.o: test/main_comparer_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/main_comparer_test.cpp -o build/main_comparer_test.o

#main_requestreceiver_test.o: test/main_requestreceiver_test.cpp
	#$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/main_requestreceiver_test.cpp -o build/main_requestreceiver_test.o

#main_actionmaker_test.o: test/main_actionmaker_test.cpp
	#$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/main_actionmaker_test.cpp -o build/main_actionmaker_test.o

tests: main_comparer_test.o gameOption.o playGame.o winReport.o # main_actionmaker_test.o main_requestreceiver_test.o
	$(CC) -g -o bin/MAIN_COMPARER_TEST build/main_comparer_test.o build/winReport.o build/gameOption.o build/playGame.o
	#$(CC) -g -o bin/MAIN_REQUESTRECEIVER_TEST build/main_requestreceiver_test.o build/winReport.o build/gameOption.o build/playGame.o
	#$(CC) -g -o bin/MAIN_ACTIONMAKER_TEST build/main_actionmaker_test.o build/winReport.o build/gameOption.o build/playGame.o


#TARGET TO COMPILE ONLY SIMULATOR
#main_top.o: top_model/main.cpp
#	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) top_model/main.cpp -o build/main_top.o
#simulator: main_top.o stationnumber.o passengerID.o
#	$(CC) -g -o bin/OTRAIN_MAIN_SIMULATION build/main_top.o build/passengerID.o build/stationnumber.o

#TARGET TO COMPILE EVERYTHING
#all: simulator tests

#CLEAN COMMANDS
clean:
	rm -f bin/* build/* simulation_results/*

