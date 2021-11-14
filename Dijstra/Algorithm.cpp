#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <mpi.h>

#define N 64
#define SOURCE 1
#define MAXINT 99999


void SingleSource(int n, int source, int *wgt, int *lengths, MPI_Comm comm) {
    int temp[N];
    int i, j;
    int nlocal; 
    int marker; / Used to mark the vertices belonging to Vo */
    int firstvtx; 
    int lastvtx; 
    int u, udist;
    int lminpair[2], gminpair[2];
    int npes, myrank;
    MPI_Status status;
    MPI_Comm_size(comm, &npes);
    MPI_Comm_rank(comm, &myrank);
    nlocal = n / npes;
    firstvtx = myrank*nlocal;
    lastvtx = firstvtx + nlocal - 1;
        for (j = 0; j<nlocal; j++) {
        lengths[j] = wgt[source*nlocal + j];
    }
    
    marker = (int *)malloc(nlocal*sizeof(int));
    for (j = 0; j<nlocal; j++) {
        marker[j] = 1;
    }
    
    if (source >= firstvtx && source <= lastvtx) {
        marker[source - firstvtx] = 0;
    }
    
    for (i = 1; i<n; i++) {
        lminpair[0] = MAXINT; 
        lminpair[1] = -1;
        for (j = 0; j<nlocal; j++) {
            if (marker[j] && lengths[j] < lminpair[0]) {
                lminpair[0] = lengths[j];
                lminpair[1] = firstvtx + j;
            }
        }
        MPI_Allreduce(lminpair, gminpair, 1, MPI_2INT, MPI_MINLOC, comm);
        udist = gminpair[0];
        u = gminpair[1];
        if (u == lminpair[1]) {
            marker[u - firstvtx] = 0;
        }
        for (j = 0; j<nlocal; j++) {
            if (marker[j] && ((udist + wgt[u*nlocal + j]) < lengths[j])) {
                lengths[j] = udist + wgt[u*nlocal + j];
            }
        }
    }
    free(marker);
}

int main(int argc, char *argv[]) {

    int weight[N][N];
        int lower = 1, upper = 15, count = 10;

    srand(time(0));

    for (int i = 0; i < N; i++) {
        for(int j=0;j<N;j++){
            weight[i][j]=0;

        }
    }



    for (int i = 0; i < N; i++) {
        for(int j=0;j<N;j++){
            int IsEdge[]={(rand() % (upper - lower + 1)) + lower,99999};
            int picker=(rand() % (1 - 0 + 1)) + 0;
            if(i==j){
                weight[i][j]=0;
            }else
            if(weight[i][j]==0){
                if(picker==0){
                    weight[i][j]=IsEdge[0];
                    weight[j][i]=IsEdge[0];
                }else{
                    weight[i][j]=IsEdge[1];
                    weight[j][i]=IsEdge[1];
                }
            }
        }
    }




    int npes, myrank, nlocal;
    int distance[N]; /distance vector/
    int localWeight; 
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    nlocal = N/npes; 
    /allocate local weight and local disatance arrays for each prosess/
    localWeight = (int *)malloc(nlocal*N*sizeof(int));
    localDistance = (int *)malloc(nlocal*sizeof(int));
    
    if (myrank == SOURCE) {

        for(k=0; k<npes; ++k) {
            for(i=0; i<N;++i) {
                for(j=0; j<nlocal;++j) {
                sendbuf[k*N*nlocal+i*nlocal+j]=weight[i][k*nlocal+j];
                }
            }
        }
    }

/distribute data/
MPI_Scatter(sendbuf, nlocal*N, MPI_INT, localWeight, nlocal*N, MPI_INT, SOURCE, MPI_COMM_WORLD);
/Implement the single source dijkstra's algorithm/
SingleSource(N, SOURCE, localWeight, localDistance, MPI_COMM_WORLD);
/collect local distance vector at the source process/
MPI_Gather(localDistance, nlocal, MPI_INT, distance, nlocal, MPI_INT, SOURCE, MPI_COMM_WORLD);

    if (myrank == SOURCE) {
        printf("Nodes: %d\n", N);
        gettimeofday(&tv, &tz);
        time_end = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.00;
        printf("time cost is %1f\n", time_end - time_start);
    }
    free(localWeight);
    free(localDistance);
    MPI_Finalize();
    return 0;
}
