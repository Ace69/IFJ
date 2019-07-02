#!/bin/sh
POSIXLY_CORRECT=yes
green=`tput setaf 2`
red=`tput setaf 1`

cesta=$(realpath .)
for file in $cesta/tests/*.txt
do
  echo ""
  ./main < "$file"
  if [  $? -eq 0 ]
then
echo "${green}-------------------TEST $file PROSEL--------------------------"
else
  echo "${red}-------------------TEST $file NEPROSEL-----------------------"
fi
  echo ""
done
