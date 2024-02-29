#!/bin/bash
#SBATCH --job-name=cs552_2
#SBATCH --output=/home/ijr44/act21/run6.txt
#SBATCH --error=/home/ijr44/act21/run6.txt
#SBATCH --time=100:00				# 2 min
#SBATCH --mem=100G
#SBATCH --nodes=3
#SBATCH --ntasks=20
#SBATCH --cpus-per-task=1

module load openmpi

mpicc -O3 ~/cs552/cs552_2/distance_act2_raspet.c -lm -o ~/out_act216

srun ~/out_act216 100000 90 10 ~/cs552/MSD_year_prediction_normalize_0_1_100k.txt