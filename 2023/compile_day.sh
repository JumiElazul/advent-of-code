#!/bin/bash
clear
g++ -std=c++2a -g -ggdb -Wall -Wextra -Wconversion -Wsign-conversion -pedantic-errors -fsanitize=address,undefined Day$1_main.cpp -o Day$1
