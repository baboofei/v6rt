#!/bin/bash

# Runs automated test

echo "Log created"
mkdir -p logs

i="0"

while [ $i -lt 10 ]
do
make qemu-no-x > logs/$(date +%s).txt & i=$[$i + 1]
done
