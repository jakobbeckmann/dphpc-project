#!/bin/bash

OUTFOLDER='job_output'
JOBNAME='dphpc'

bsub -n 24 -W 00:60 -o $OUTFOLDER/$JOBNAME_%J.out -e $JOBNAME_%J.err -J $JOBNAME python ./python/scripts/grid_run_exe.py

#-J Name für den Job
#-n anzahl cores
#-W wallclock time
#-o output file 
#-e error file
#exe kommt immer am Schluss

