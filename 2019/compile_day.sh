#!/bin/bash
g++ -std=c++2a -g -ggdb -Wall -Wextra -Wno-sign-compare -fsanitize=address,undefined day$1_main.cpp -o day$1.out
