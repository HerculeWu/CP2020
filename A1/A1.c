#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef enum{true=1, false=0} bool;
//read data file
void read_file(const char* filename, bool isBinary, unsigned int *numberOfdata, double **dataset){
    FILE * file = fopen(filename, "r");
    if (file == NULL) {
        perror("[read_file] Can not open file\n");
    }
    unsigned int n = 0;
    if (isBinary) {
        if (fread(&n, sizeof(unsigned int), 1, file) != 1){
            perror("[read_file] Can not read data\n");
        }
    }else{
        if (fscanf(file, "%u\n",&n) != 1){
            perror("[read_file] Can not read data\n");
        }
    }
    if (n == 0) {
        perror("[read_file] empty data set\n");
    }
    double *data = (double *)malloc(n*sizeof(double));
    if (!data) {
        perror("[read_dataset] Error from malloc\n");
    }
    if (isBinary) {
        if ( fread(data, sizeof ( double ),n,file)!= n ){
            perror("[read_dataset] data reading not successfully");
        }
    }else{
        for (unsigned int i=0; i<n; i++) {
            if (fscanf(file, "%lf\n",data+i) != 1) {
                perror("[read_dataset] data reading not successfully");
            }
        }
    }
    fclose(file);
    *numberOfdata = n;
    *dataset = data;
}
//display data set
void show_dataset(const double *dataset, const unsigned int sizeOfDataset,char *header){
    if (dataset == NULL || sizeOfDataset==0) {
        perror("[show_dataset] invalid data set");
    }
    printf("\n##Data set: %s##\n",header);
    for (unsigned int i=0; i<sizeOfDataset; ++i) {
        printf("%6d %25.16e\n",i,dataset[i]);
    }
}
//compute cumulative square sum
void cum_sqr_sum(const double *A, double *sigma, unsigned int size){
    if (A==NULL || sigma==NULL || size==0) {
        printf("[cum_sqr_sum]check the input\n");
        exit(1);
    }
    sigma[0] = pow(A[0],2);
    for (unsigned int k = 0; k<(size-1); ++k) {
        for (unsigned int i = 0; i<k; ++i) {
            sigma[k] += pow(A[i], 2);
        }
    }
}
int main(int argc, const char * argv[]) {
    unsigned int numberOfdata;
    double *dataset1 = NULL;
    //read data
    read_file("/Users/wenjie/Library/Mobile Documents/com~apple~CloudDocs/Study/Bechlor/SS20/CP-Tutorial/U1/a1/dataset1", false, &numberOfdata, &dataset1);
    show_dataset(dataset1, numberOfdata,"dataset1");
    double *sigma = (double *)malloc((numberOfdata-1)*sizeof(double));
    cum_sqr_sum(dataset1, sigma, numberOfdata);
    FILE *outputFile1 = fopen("/Users/wenjie/Library/Mobile Documents/com~apple~CloudDocs/Study/Bechlor/SS20/CP-Tutorial/U1/a1/dataset1_css", "w");
    if (outputFile1 == NULL){
        perror("Error:");
        exit(1);
    }
    fprintf(outputFile1, "%d\n",numberOfdata-1);
    for (unsigned int i = 0; i< (numberOfdata-1); ++i) {
        fprintf(outputFile1, "%e\n",sigma[i]);
    }
    fclose(outputFile1);
    free(sigma);free(dataset1);
    double *dataset2 = NULL;
    read_file("/Users/wenjie/Library/Mobile Documents/com~apple~CloudDocs/Study/Bechlor/SS20/CP-Tutorial/U1/a1/dataset2", true, &numberOfdata, &dataset2);
    FILE *outputFile2 = fopen("/Users/wenjie/Library/Mobile Documents/com~apple~CloudDocs/Study/Bechlor/SS20/CP-Tutorial/U1/a1/dataset2_sin", "w");
    if (outputFile2 == NULL){
        perror("Error:");
        exit(1);
    }
    for (unsigned int i = 0; i<numberOfdata; ++i) {
        fprintf(outputFile2, "%e,%e\n", dataset2[i], sin(dataset2[i]));
    }
    fclose(outputFile2);
    free(dataset2);
    return 0;
}
