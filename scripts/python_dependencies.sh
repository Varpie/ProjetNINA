#!/bin/bash
printf "checking for python dependencies..."

sel=$(pip list --format columns | grep selenium)
lxml=$(pip list --format columns | grep lxml)
flag=0
message="\nThis program needs the following dependencie(s) :\n"

if [ -z "$lxml" ]; then
  message+="\t-lxml\n"
  flag=1
fi

if [ -z "$sel" ]; then
  message+="\t-selenium\n"
fi

if [ "$flag" -ne "0" ]; then
  printf "flag : %d" "$flag"
  message+="Do you want to install those ? [Y/n]"
  echo -en "$message"
  read -n 1 -e choice < /dev/tty
fi

case $choice in
  "y") flag=1 ;;
  "Y") flag=1 ;;
  "") flag=1 ;;
  *) flag=0; echo -e "Careful, program won't work without those dependencies" ;;
esac

if [ "$flag" -eq 1 ]; then
  if [ -z "$lxml" ]; then
    sudo pip install lxml
  fi
  if [ -z "$sel" ]; then
    sudo pip install selenium
  fi
fi

printf " done.\n"