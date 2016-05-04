#!/bin/bash -e

echo "Clean working directory ..."
make clean

echo -e "\n\nCompiling ..."
make object

echo -e "\n\nCreating ELF and HEX files ..."
make elf hex

echo -e "\n\nDone"
