#!/usr/bin/env bash
#  @author : GONNIN THIBAUT, FILIOL DE RAIMOND-MICHEL Guillaume
ABSOLUTE_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd $ABSOLUTE_PATH/bin
./Field -e2 -t1 -p4 -m
