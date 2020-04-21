//
//  main.c
//  CPA2
//
//  Created by 吴文杰 on 21.04.20.
//  Copyright © 2020 吴文杰. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef enum{true = 1, false = 0} bool;
void getEpsilonf(float *e){
    float epsilon = 1.0;
    while ((1.0f + epsilon) != 1.0f) {
        epsilon = epsilon * 0.5f;
    }
    *e = epsilon;
}
void getEpsilond(double *e){
    double epsilon = 1.0;
    while ((1.0 + (epsilon)) != 1.0) {
        epsilon = epsilon * 0.5;
    }
    *e = epsilon;
}
//TODO: machine epsilon of float128 with exponent 1;
double func(double x){
    return pow(log(x),1.0/x);
}
double funcdiff(double x, double h){
    return (func(x+h)-func(x-h))/(2.0*h);
}
double Extrmums(double h,double x0, double x1, unsigned int N){
    double x2;
    for (int i=0; i<N; i++) {
        x2 = x1 - funcdiff(x1, h)*(x1-x0)/(funcdiff(x1, h)-funcdiff(x0, h));
        x0 = x1;
        x1 = x2;
    }
    return x1;
}
double integrator(double z, double t){
    return exp(-t)*pow(t,z-1.0);
}
double Gamma(double z,double h, double epsilon){
    if (z == 0.0) {
        printf("gamma-function at z=0 is divergenc!");
        exit(1);
    }
    if (z>1.0) {
        exit(2);
    }
    double Int = 0.0;
    double A;
    double xn=0.0;
    double xnp1 = xn+h;
    do {
        A = h*integrator(z, (xn+xnp1)/2);
        Int += A;
        xn = xnp1;
        xnp1 += h;
    } while (A>epsilon);
    return Int;
}
//TODO: write a function to calculate gamma-function for all z
int main() {
    float epsilonf;
    getEpsilonf(&epsilonf);
    printf("%e\n",epsilonf);
    double epsilond;
    getEpsilond(&epsilond);
    printf("%e\n",epsilond);
    double xmax;
    double h = 6.63e-8;
    xmax = Extrmums(h, 6.0, 3.0, 5);
    printf("%e\n",xmax);
    double gamma;
    gamma = Gamma(3.0, 1e-3, 1e-16);
    printf("%e\n",gamma);
    return 0;
}
