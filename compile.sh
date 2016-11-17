#!/usr/bin/env bash
#  @author : GONNIN THIBAUT, FILIOL DE RAIMOND-MICHEL Guillaume
ABSOLUTE_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
mkdir $ABSOLUTE_PATH/bin
gcc -Wall --std=c99 $ABSOLUTE_PATH/src/Field.c -o $ABSOLUTE_PATH/bin/Field -lpthread