#!/bin/bash

rm main.o
rm TrainLocation.o
rm Station.o
rm Track.o
rm massTransit

g++ -c main.cpp
g++ -c TrainLocation.cpp 
g++ -c Station.cpp 
g++ -c Track.cpp
g++ -o massTransit main.o TrainLocation.o Station.o Track.o -lpthread -lncurses
