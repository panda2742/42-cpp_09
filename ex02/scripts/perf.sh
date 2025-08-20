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


NUMBER=$1
SEQ=$(shuf -i 1-${NUMBER} -n ${NUMBER} | tr "\n" " ")

if [ -f ./PmergeMe_with_turbo ]; then
  EXEC="./PmergeMe_with_turbo"
else
  EXEC="./PmergeMe"
fi

echo -e -n "${Black}Do you want a deep test with Valgrind? It is recommanded for small sequences (<5000). (takes much more time) [${BGreen}y${Black} / ${BRed}n (default)${Black}]${Color_Off} "
read -r result

y="y"
if [[ "$y" == "$result" ]]; then
  echo -e "${Purple}Running script with Valgrind (memory check) with ${BRed}${NUMBER}${Purple} elements.${Color_Off}"
  valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --show-mismatched-frees=yes --track-fds=yes --trace-children=yes ${EXEC} ${SEQ} 1> .perf 2> .perf_vg

  echo -e "${BBlue}- File Descriptors:${Color_Off}"
  cat .perf_vg | grep "Open file" | sed 's/^==[0-9]*==\s*/\t/'
  cat .perf_vg | grep "Open AF_UNIX" | sed 's/^==[0-9]*==\s*/\t/'

  echo -e "\n${BGreen}- Heap Memory Usage:${Color_Off}"
  cat .perf_vg | grep "in use at exit" | sed 's/^==[0-9]*==\s*/\t/'
  cat .perf_vg | grep "total heap usage" | sed 's/^==[0-9]*==\s*/\t/'
  cat .perf_vg | grep "All heap blocks" | sed 's/^==[0-9]*==\s*/\t/'

  echo -e "\n${BRed}- Errors Summary:${Color_Off}"
  cat .perf_vg | grep "ERROR SUMMARY:" | sed 's/^==[0-9]*==\s*/\t/' | sed 's/ERROR SUMMARY: //'

  echo -e "\n${BYellow}- Time (with Valgrind!):${Color_Off}"
  awk -F'\n' '/took|sorted/ {print "\t" $0}' .perf

  ${EXEC} ${SEQ} 1> .perf 2> /dev/null

  echo -e "\n${BCyan}- Time (without Valgrind!):${Color_Off}"
  awk -F'\n' '/took|sorted/ {print "\t" $0}' .perf
else
  echo -e "${Cyan}Running script without Valgrind (fast check) with ${BRed}${NUMBER}${Purple} elements.${Color_Off}"
  ${EXEC} ${SEQ} 1> .perf 2> /dev/null

  echo -e "${BCyan}- Time (without Valgrind!):${Color_Off}"
  awk -F'\n' '/took|sorted/ {print "\t" $0}' .perf
fi

rm -f .perf > /dev/null
rm -f .perf_vg > /dev/null