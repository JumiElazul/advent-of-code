#!/bin/bash

read -p "Print opcodes? y/n: " print_opcodes
if [ $print_opcodes == "y" ]; then
    g++ -g -Wall -Wextra -Werror -fsanitize=address,undefined -std=c++20 main.cpp intcode.cpp -o main -DPRINT_OPCODES
else
    g++ -g -Wall -Wextra -Werror -fsanitize=address,undefined -std=c++20 main.cpp intcode.cpp -o main
fi
