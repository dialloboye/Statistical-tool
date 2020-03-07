#!/bin/bash

#Script to clean of generated lists before running a new time. This helps preventing from false successful execution.

listFiles=( listGaussianSignal listInterpolatedSignalacceptance listInterpolatedSignalefficiency listSplitSignalacceptance listSplitSignalefficiency listToHadd listRescaledGaussianSignal listGaussianSignalUnscaled )

for f in "${listFiles[@]}"
do
	rm -f $f
done

