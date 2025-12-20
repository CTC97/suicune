#!/usr/bin/env bash
set -euo pipefail

# Define colors
RESET="\033[0m"
MEDIUM_BLUE="\033[38;5;24m"  # #4A6FA5
LIGHT_BLUE="\033[38;5;153m"  # #E6EEF7
PURPLE="\033[38;5;139m"  # #B7AEC8
LIGHT_PURPLE="\033[38;5;140m"  # #9E97B8


echo -e "${MEDIUM_BLUE}Cleaning...${RESET}"
make clean > /dev/null 2>&1;
echo -e "\t${LIGHT_BLUE}Done.${RESET}"

echo -e "${MEDIUM_BLUE}Building...${RESET}"
make > /dev/null;
echo -e "\t${LIGHT_BLUE}Done.${RESET}"

echo -e "${MEDIUM_BLUE}Running...${RESET}"
build/build > /dev/null;