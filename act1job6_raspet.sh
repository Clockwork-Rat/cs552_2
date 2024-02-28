#!/bin/bash
#SBATCH --job-name=cs552_2
#SBATCH --output=~/cs552/run6.txt
#SBATCH --error=~/cs552/run6.err
#SBATCH --time=100:00				# 2 min
#SBATCH --mem=90G
#SBATCH --nodes=3
#SBATCH --ntasks=20
#SBATCH --cpus-per-task=1

module load openmpi

mpicc -O3 ~/cs552/cs552_2/distance_act1_raspet.c -lm -o ~/out6

srun ~/out6 100000 90 5000 ~/cs552/MSD_year_prediction_normalize_0_1_100k.txt