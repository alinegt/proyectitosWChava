#! /bin/bash

inputFile="/home/salvador/github/proyectitosWChava/SPE/data/processed/noCoil.root"
make
./getTimeCFD $inputFile cfdResults.root     