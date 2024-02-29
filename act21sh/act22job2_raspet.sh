#!/bin/bash
#SBATCH --job-name=cs552_2
#SBATCH --output=/home/ijr44/act21/run2.txt
#SBATCH --error=/home/ijr44/act21/run2.txt
#SBATCH --time=600:00				# 2 min
#SBATCH --mem=100G
#SBATCH --nodes=1
#SBATCH --ntasks=20
#SBATCH --cpus-per-task=1

module load openmpi

mpicc -O3 ~/cs552/cs552_2/distance_act2_raspet.c -lm -o ~/out_act212

srun ~/out_act212 100000 90 100 ~/cs552/MSD_year_prediction_normalize_0_1_100k.txt