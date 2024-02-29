#!/bin/bash
#SBATCH --job-name=cs552_2
#SBATCH --output=/home/ijr44/act23/run3.txt
#SBATCH --error=/home/ijr44/act23run3.txt
#SBATCH --time=100:00				# 2 min
#SBATCH --mem=100G
#SBATCH --nodes=1
#SBATCH --ntasks=8
#SBATCH --cpus-per-task=1

module load openmpi

mpicc -O3 ~/cs552/cs552_2/distance_act1_raspet.c -lm -o ~/out_act2331
mpicc -O3 ~/cs552/cs552_2/distance_act2_raspet.c -lm -o ~/out_act2332

srun -n1 /usr/bin/perf stat -B -e cache-references,cache-misses ~/out_act2331 100000 90 10 ~/cs552/MSD_year_prediction_normalize_0_1_100k.txt
srun -n1 /usr/bin/perf stat -B -e cache-references,cache-misses ~/out_act2332 100000 90 10 ~/cs552/MSD_year_prediction_normalize_0_1_100k.txt