#!/bin/bash
a=`find ./ -name "*.cpp"  | xargs cat | grep -v ^$ | wc -l`
b=`find ./ -name "*.h"  | xargs cat | grep -v ^$ | wc -l`

echo $[$a + $b]

