#!/bin/bash

CUR_DIR="$( cd "$( dirname "$0"  )" && pwd  )"
cd $CUR_DIR

rm Embedded-2021.zip

zip -r Embedded-2021.zip sdk/code