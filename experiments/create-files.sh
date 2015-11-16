#!/bin/bash

show_files()
{
  ls -lh /tmp | grep "fs-[0-9]*MB$"
}

create_files()
{
  dd if=/dev/urandom of=/tmp/fs-1MB bs=1M count=1
  dd if=/dev/urandom of=/tmp/fs-10MB bs=1M count=10
  dd if=/dev/urandom of=/tmp/fs-30MB bs=1M count=30
  dd if=/dev/urandom of=/tmp/fs-50MB bs=1M count=50
}

if [[ $(find /tmp -name 'fs-*MB' | wc -l ) = 4 ]]
then
  echo "It seems like you already have the required files!"
  
  echo
  show_files
  echo

  echo "Initializating the experiments"
else
  echo "Creating required files."
  echo "It requires 91MB of harddisk space (in /tmp)"
  read -p "Are you sure to proceed? [y/N] " -n 1 -r
  echo    

  if [[ ! $REPLY =~ ^[Yy]$ ]]
  then
    echo "Nothing will be done!"
    exit 1
  fi

  create_files

  echo
  show_files 
  echo

  echo "Done!"
fi

