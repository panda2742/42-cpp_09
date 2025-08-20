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
SEQ=$(shuf -i 1-100000 -n ${NUMBER} | tr "\n" " ")

echo -e -n "${Yellow} Do you want a deep test with Valgrind? (takes much more time) [${BGreen}y${Yellow} / ${BRed}n (default)${Yellow}]${Color_Off} "
read -r result

make bonus > /dev/null

y="y"
if [ "$y" == $result ]; then
  echo -e "${BPurple}Running script with Valgrind (memory check).${Color_Off}"
  valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --show-mismatched-frees=yes --track-fds=yes --trace-children=yes ./PmergeMe ${SEQ} 1> /dev/null 2> .perf_vg

  echo -e "${BBlue}File Descriptors:${Color_Off}"
  cat .perf_vg | grep -E "Open file" | sed 's/^==[0-9]*==\s*/\t/'
  cat .perf_vg | grep -E "Open AF_UNIX" | sed 's/^==[0-9]*==\s*/\t/'

  echo -e "\n${BGreen}Heap Memory Usage:${Color_Off}"
  cat .perf_vg | grep -E "in use at exit" | sed 's/^==[0-9]*==\s*/\t/'
  cat .perf_vg | grep -E "total heap usage" | sed 's/^==[0-9]*==\s*/\t/'
  cat .perf_vg | grep -E "All heap blocks" | sed 's/^==[0-9]*==\s*/\t/'

  echo -e "\n${BRed}Errors Summary:${Color_Off}"
  cat .perf_vg | grep -E "ERROR SUMMARY:" | sed 's/^==[0-9]*==\s*/\t/' | sed 's/ERROR SUMMARY: //'
else
  echo -e "${BCyan}Running script without Valgrind.${Color_Off} "
  ./PmergeMe ${SEQ} > .perf
  cat .perf | awk '{if(length($$0) > 141) print substr($$0,1,50) " ..."; else print $$0}'
fi

rm -f .perf > /dev/null
rm -f .perf_vg > /dev/null