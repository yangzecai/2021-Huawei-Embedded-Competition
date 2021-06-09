#!/bin/bash

CUR_DIR="$( cd "$( dirname "$0"  )" && pwd  )"
cd $CUR_DIR
cd ../

inputPath=$1
outputPath=$2

./bin/Embedded-2021 < ${inputPath} > ${outputPath}