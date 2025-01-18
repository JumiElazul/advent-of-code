#!/bin/bash

g++ -g -Wall -Wextra -Werror -fsanitize=address,undefined -std=c++20 main.cpp intcode.cpp -o main
