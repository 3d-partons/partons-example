#!/bin/bash

cd "/root/workspace/partons-example/"
echo "${PWD}"
echo $(ls)
bin/PARTONS_example ${@}
