#!/bin/sh

./cvm/cvmas out >> temp;
./cvm/cvm temp;
rm temp
