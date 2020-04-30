#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define Pi 3.1415926
void gauleg(const double x1, const double x2,const int N, double *x, double *w){
    const double eps = 1.0e-14;
    int m = (N+1)/2;
    double z1, z, xm,xl,pp, p3,p2,p1;
    xm = 0.5*(x2+x1);
    xl = 0.5*(x2-x1);
    for (int i = 0; i<m; i++) {
        z = cos(3.14*(i+0.75)/(N+0.5));
        do {
            p1 = 1.0;
            p2 = 0.0;
            for (int j=0; j<N; j++) {
                p3 = p2;
                p2 = p1;
                p1 = ((2.0*j+1.0)*z*p2-j*p3)/(j+1);
            }
            pp = N*(z*p1-p2)/(z*z-1.0);
            z1 = z;
            z = z1-p1/pp;
        } while (fabs(z-z1)>eps);
        x[i] = xm-xl*z;
        x[N-1-i] = xm+xl*z;
        w[i] = 2.0*xl/((1.0-z*z)*pp*pp);
        w[N-1-i]=w[i];
    }
}
double f_org(double x){
    return cos(log(x)/x)/x;
}
double I_org(double epsilon,const int N){
    if (epsilon > 1) {
        exit(1);
    }
    double x1 = epsilon;
    double x2 = 1.0;
    double *w = (double *)malloc(N*sizeof(double));
    double *x = (double *)malloc(N*sizeof(double));
    if (!w || !x) {
        exit(2);
    }
    gauleg(x1, x2, N, x, w);
    double I = 0.0;
    for (int i = 0; i<N; i++) {
        I += f_org(x[i])*w[i];
    }
    free(w);free(x);
    return I;
}
double x_y(double y){
    double x0 = 0.0;
    if (y<0) {
        exit(3);
    }
    if (y<=10) {
        x0= 1.0125+(0.8577*y)-(0.129013*y*y)+(0.0208645*y*y*y)-(0.00176148*y*y*y*y)+(0.000057941*y*y*y*y*y);
    }else{
        x0 = y/log(y)/(1.0-log(log(y))/(1+log(y)));
    }
    double xnp1, xn;
    xn = x0;
    xnp1 = x0;
    do {
        xn = xnp1;
        xnp1 = (xn+y)/(log(xn)+1);
    } while (fabs(xn-xnp1)>1e-13);
    return xnp1;
}
double sigma_term1(double z){
    return cos(z)/(z+x_y(z));
}
double sigma_term2(double z,unsigned int k){
    return cos(z)/(z+((double)k*3.1415926)+x_y(z+((double)k*3.1415926)));
}
double I_sigma(unsigned int kmax,int N){
    double T_1 = 0.0;
    double x1 = 0.0;
    double x2 = Pi/2.0;
    double *w = (double *)malloc(N*sizeof(double));
    double *z = (double *)malloc(N*sizeof(double));
    if (!w || !z) {
        exit(2);
    }
    gauleg(x1, x2, N, z, w);
    for (int i = 0; i<N; i++) {
        T_1 += sigma_term1(z[i])*w[i];
    }
    x1 = -Pi/2.0;
    x2 = Pi/2.0;
    gauleg(x1, x2, N, z, w);
    double T_2 = 0.0;
    double I_2 = 0.0;
    for (int k = 1; k<=kmax; k++) {
        I_2 = 0.0;
        for (int i = 0; i<N; i++) {
            I_2 += sigma_term2(z[i],k)*w[i];
        }
        if (k%2 == 0) {
            T_2 += I_2;
        }else{
            T_2 -= I_2;
        }
    }
    free(w);free(z);
    return T_1+T_2;
}
int main(){
    for (int i = 1; i <= 3; i++)
    {
        double epsilon = 1.0/(pow(10,(double)i));
        printf("epsilon = %lf, I = %f\n",epsilon,I_org(epsilon,10));
    }
    double I_ext =  0.323367431677778761399370087952170446651046625725469661681036;
    FILE *data = fopen("data.txt","w");
    for (int kmax = 1; kmax < 500; kmax++)
    {
        double I_num = I_sigma(kmax,10);
        fprintf(data,"%d,%e\n",kmax, fabs(I_num-I_ext)/I_ext);
    }
    fclose(data);
    return 0;
}
