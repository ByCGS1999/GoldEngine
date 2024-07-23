#!/bin/bash

g++ -std=c++17 ./WineBootstrapper/WineBootstrapper.cpp ./WineBootstrapper/system2/sys2.c -o ./bin/bootstrapper -fpermissive

chmod +x ./bin/bootstrapper
