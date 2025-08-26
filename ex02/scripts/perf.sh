#!/bin/bash

if [ -z "$1" ]; then
  exit 1
fi

# Reset
Color_Off='\033[0m'       # Text Reset
# Regular Colors
Black='\033[0;30m'        # Black
Red='\033[0;31m'          # Red
Green='\033[0;32m'        # Green
Yellow='\033[0;33m'       # Yellow
Blue='\033[0;34m'         # Blue
Purple='\033[0;35m'       # Purple
Cyan='\033[0;36m'         # Cyan
White='\033[0;37m'        # White
# Bold
BBlack='\033[1;30m'       # Black
BRed='\033[1;31m'         # Red
BGreen='\033[1;32m'       # Green
BYellow='\033[1;33m'      # Yellow
BBlue='\033[1;34m'        # Blue
BPurple='\033[1;35m'      # Purple
BCyan='\033[1;36m'        # Cyan
BWhite='\033[1;37m'       # White

EXEC="./PmergeMe"
DIST=".dist/"

echo -e -n "Do you want flag optimizations for your program? [${BGreen}y (default)${Color_Off} / ${BRed}n${Color_Off}]: "
read -r res_optimize

if [[ "$(uname)" != "Darwin" ]]; then
  echo -e -n "Do you want a deep test with Valgrind? It is recommanded for size<5000. [${BGreen}y  (default)${Color_Off} / ${BRed}n${Color_Off}]: "
  read -r res_valgrind
fi

echo -e "${Black}Cleaning folder...${Color_Off}"
if [[ "n" != "$res_optimize" ]]; then
  EXEC=$EXEC"_with_turbo"
  DIST=".dist_turbo/"
  rm -rf ./PmergeMe .dist/
fi

echo -e "${Black}Compiling program...${Color_Off}"
if [[ "n" == "$res_optimize" ]]; then
  RES=$(make)
else
  RES=$(make bonus)
fi

if [[ $? -ne 0 ]]; then
  echo "${BRed}Compilation error.\n$RES"
  exit 1
fi

if [[ "$(uname)" != "Darwin" && "n" != "$res_valgrind" ]]; then
  EXEC="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --show-mismatched-frees=yes --track-fds=yes --trace-children=yes "$EXEC
fi

echo -e "${Black}Loading input file...${Color_Off}"
NUMBER=$1
shuf -i 1-${NUMBER} -n ${NUMBER} | tr "\n" " " > .large

if [[ "$(uname)" != "Darwin" && "n" != "$res_valgrind" ]]; then
  echo -e "Running script ${Green}with${Color_Off} Valgrind (memory check) with ${BPurple}${NUMBER}${Color_Off} elements."
else
  echo -e "Running script ${Red}without${Color_Off} Valgrind (memory check) with ${BPurple}${NUMBER}${Color_Off} elements."
fi

${EXEC} file:.large 1> .perf 2> .perf_err

echo -e "${BBlue}- File Descriptors:${Color_Off}"
cat .perf_err | grep "Open file" | sed 's/^==[0-9]*==\s*//'
cat .perf_err | grep "Open AF_UNIX" | sed 's/^==[0-9]*==\s*//'

echo -e "\n${BGreen}- Heap Memory Usage:${Color_Off}"
cat .perf_err | grep "in use at exit" | sed 's/^==[0-9]*==\s*//'
cat .perf_err | grep "total heap usage" | sed 's/^==[0-9]*==\s*//'
cat .perf_err | grep "All heap blocks" | sed 's/^==[0-9]*==\s*//'

echo -e "\n${BRed}- Errors Summary:${Color_Off}"
cat .perf_err | grep "ERROR SUMMARY:" | sed 's/^==[0-9]*==\s*//' | sed 's/ERROR SUMMARY: //'

echo -e "\n${BYellow}- Output:${Color_Off}"
awk 'length < 200' .perf

rm -rf .perf .perf_err .large
