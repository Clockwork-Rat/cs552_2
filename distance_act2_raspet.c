 
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>



//Example compilation
//mpicc distance_matrix_starter.c -lm -o distance_matrix_starter

//Example execution
//mpirun -np 1 -hostfile myhostfile.txt ./distance_matrix_starter 100000 90 100 MSD_year_prediction_normalize_0_1_100k.txt



//function prototypes
int importDataset(char * fname, int N, double ** dataset);

double calc_distance(double *a1, double *a2, int dim) {

   double d_sqr = 0.0;

   for( size_t i = 0; i < dim; ++i ) {
     double mem = a1[i] - a2[i];
     d_sqr += mem * mem;
   }

   return sqrt(d_sqr);

}


int main(int argc, char **argv) {

  int my_rank, nprocs;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);


  //Process command-line arguments
  int N;
  int DIM;
  int blocksize;
  char inputFname[500];


  if (argc != 5) {
    fprintf(stderr,"Please provide the following on the command line: N (number of lines in the file), dimensionality (number of coordinates per point), block size, dataset filename.\n");
    MPI_Finalize();
    exit(0);
  }

  sscanf(argv[1],"%d",&N);
  sscanf(argv[2],"%d",&DIM);
  sscanf(argv[3],"%d",&blocksize);
  strcpy(inputFname,argv[4]);
  
  //pointer to dataset
  double ** dataset;

  
  if (N<1 || DIM <1)
  {
    printf("\nN is invalid or DIM is invalid\n");
    MPI_Finalize();
    exit(0);
  }
  //All ranks import dataset
  else
  {
   
    if (my_rank==0)
    {
    printf("\nNumber of lines (N): %d, Dimensionality: %d, Block size: %d, Filename: %s\n", N, DIM, blocksize, inputFname);
    }

    //allocate memory for dataset
    dataset=(double**)malloc(sizeof(double*)*N);
    for (int i=0; i<N; i++)
    {
      dataset[i]=(double*)malloc(sizeof(double)*DIM);
    }

    int ret=importDataset(inputFname, N, dataset);

    if (ret==1)
    {
      MPI_Finalize();
      return 0;
    }
  }

  //Write code here
  double dataset_literal[N][DIM];

  for (int i = 0; i < N; ++i) {
    for(int k = 0; k < DIM; ++k) {
      dataset_literal[i][k] = dataset[i][k];
    }
  }

  double local_data[blocksize][DIM];

  double **matrix;

  if(my_rank == 0) {
    matrix = (double **)malloc(N * sizeof(double *));
    for (size_t i = 0; i < N; ++i) {
      matrix[i] = (double *)malloc(sizeof(double));
    }
  }

  //rank zero scatters and sends ranges to all ranks
  MPI_Scatter(dataset_literal,
              blocksize*DIM,
              MPI_DOUBLE,
              local_data,
              blocksize*DIM,
              MPI_DOUBLE,
              0, 
              MPI_COMM_WORLD
              );

  //other ranks wait to receive locations from 0
  double lines[blocksize][N];
  size_t b = 3;

  // time calculation
  double tstart = MPI_Wtime();
  
  // tiling
  size_t i = 0;
  size_t j = 0;

  // make sure that all tiles have been addressed
  // this is the point of the while loop
  while(i < N && j < blocksize) {
    size_t target_i = i + b;
    // loop over rows to either the end of the row or the 
    // end of the entire matrix
    for(;i < target_i && i < N; ++i) {
      size_t target_j = j + b;
      // loop over columns until the end of the row or the 
      // end of the block
      for(;j < blocksize && j < target_j; ++j) {
        // calculate specific distance
        lines[i][j] = calc_distance(local_data[i], dataset[j], DIM);
      }
    }
  }

  // make sure all ranks have gotten to this point
  MPI_Barrier(MPI_COMM_WORLD);

  // end timer
  double tend = MPI_Wtime();

  //MPI_Gather(lines, 
  //           N*blocksize,
  //           MPI_DOUBLE,
  //           matrix,
  //           N*blocksize,
  //           MPI_DOUBLE, 
  //           0, 
  //           MPI_COMM_WORLD);

  double count;

  MPI_Reduce(
    lines,
    &count,
    N*blocksize,
    MPI_DOUBLE,
    MPI_SUM,
    0,
    MPI_COMM_WORLD
  );

  if(my_rank == 0){
    printf("Total: %f\n", count);
    printf("Time: %f\n", tend - tstart);
  }

  // print matrix
  //if (my_rank == 0) {
  //  for (size_t i = 0; i < N; ++i) {
  //    for (size_t j = 0; j < N; ++j) {
  //      printf("%f, ", matrix[i][j]);
  //    }
  //    printf("\n");
  //  }
  //}

  //free dataset
  for (int i=0; i<N; i++)
  {
    free(dataset[i]);
  }

  free(dataset);

  if (my_rank == 0) {
    for(size_t i = 0; i < N; ++i) {
      free(matrix[i]);
    }
    free(matrix);
  }

  MPI_Finalize();

  return 0;
}




int importDataset(char * fname, int N, double ** dataset)
{

    FILE *fp = fopen(fname, "r");

    if (!fp) {
        printf("Unable to open file\n");
        return(1);
    }

    char buf[4096];
    int rowCnt = 0;
    int colCnt = 0;
    while (fgets(buf, 4096, fp) && rowCnt<N) {
        colCnt = 0;

        char *field = strtok(buf, ",");
        double tmp;
        sscanf(field,"%lf",&tmp);
        dataset[rowCnt][colCnt]=tmp;

        
        while (field) {
          colCnt++;
          field = strtok(NULL, ",");
          
          if (field!=NULL)
          {
          double tmp;
          sscanf(field,"%lf",&tmp);
          dataset[rowCnt][colCnt]=tmp;
          }   

        }
        rowCnt++;
    }

    fclose(fp);

    return 0;


}