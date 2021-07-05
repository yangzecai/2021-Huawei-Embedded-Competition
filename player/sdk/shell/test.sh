#!/bin/bash

CUR_DIR="$( cd "$( dirname "$0"  )" && pwd  )"
cd $CUR_DIR
bash build.sh
../bin/Embedded-2021 < ../../judge/cases/TestData_24.case