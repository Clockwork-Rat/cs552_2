#!/bin/bash
#SBATCH --job-name=cs552_2
#SBATCH --output=/home/ijr44/act21/run1.txt
#SBATCH --error=/home/ijr44/act21/run1.txt
#SBATCH --time=100:00				# 2 min
#SBATCH --mem=100G
#SBATCH --nodes=1
#SBATCH --ntasks=20
#SBATCH --cpus-per-task=1

module load openmpi

mpicc -O3 ~/cs552/cs552_2/distance_act2_raspet.c -lm -o ~/out

srun ~/out 100000 90 4000 ~/cs552/MSD_year_prediction_normalize_0_1_100k.txt