#!/bin/bash
#SBATCH --job-name=ping_pong
#SBATCH --output=~/ijr44/scratch/ping_pong.txt
#SBATCH --error=~/ijr44/scratch/ping_pong.err
#SBATCH --time=02:00				# 2 min
#SBATCH --mem=2000 
#SBATCH --nodes=1
#SBATCH --ntasks=6
#SBATCH --cpus-per-task=1

module load openmpi

mpicc ~/ijr44/scratch/pingpong_act1_raspet.c -lm -o ~/ijr44/scratch/ping_pong

srun ~/ijr44/scratch/ping_pong