#include <bits/stdc++.h>
using namespace std;

int main(){
    double R1,R2;
    cin>>R1>>R2;
    double m=min(R1,R2);
    int N=2000000;
    double a=-m,b=m;
    double h=(b-a)/N;
    double sum=0;
    for(int i=0;i<=N;i++){
        double z=a+i*h;
        double v1=R1*R1-z*z; if(v1<0)v1=0;
        double v2=R2*R2-z*z; if(v2<0)v2=0;
        double f=4*sqrt(v1)*sqrt(v2);
        if(i==0||i==N) sum+=f;
        else if(i%2==1) sum+=4*f;
        else sum+=2*f;
    }
    double vol=sum*h/3.0;
    printf("%.4f\n",vol);
    return 0;
}
