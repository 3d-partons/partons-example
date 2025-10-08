#!/bin/bash

#set -e

ROOTDIR=$PWD

cd $ROOTDIR/make_file/build
cmake .. -DElementaryUtils_HINT=$PWD/../../../../elementary-utils -DNumA++_HINT=$PWD/../../../../numa -DPARTONS_HINT=$PWD/../../../../partons
make

cd $ROOTDIR/make_file/bin
ln -s ../../../bin/logger.properties
ln -s ../../../bin/partons.properties
mkdir tmp
cd $ROOTDIR/make_file
ln -s ../../data

cd $ROOTDIR/make_file 
./bin/make_file ../file.hdf5 quick

cd $ROOTDIR/compare_files/build
cmake .. 
make

cd $ROOTDIR
./compare_files/bin/compare_files file.hdf5 reference/file_ref.hdf5

exit $?
