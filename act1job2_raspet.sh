#!/bin/bash
#SBATCH --job-name=cs552_2
#SBATCH --output=~/cs552/run2.txt
#SBATCH --error=~/cs552/run2.err
#SBATCH --time=100:00				# 2 min
#SBATCH --mem=90G
#SBATCH --nodes=1
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1

module load openmpi

mpicc -O3 ~/cs552/cs552_2/distance_act1_raspet.c -lm -o ~/out2

srun ~/out2 100000 90 25000 ~/cs552/MSD_year_prediction_normalize_0_1_100k.txt