#include <bits/stdc++.h>
using double_t = double;
const int MAXN = 11;
double V1[MAXN][MAXN], V2[MAXN][MAXN]; // expected payoff
double P1[MAXN][MAXN]; // prob player 1 wins
int N; double F1, F2, V;

int main(){
    scanf("%d %lf %lf %lf",&N,&F1,&F2,&V);
    double a1 = log(1.0/pow(0.99,1-F1));
    double a2 = log(1.0/pow(0.99,1-F2));
    // boundary
    for(int j=1;j<=N;j++){V1[0][j]=V;V2[0][j]=0;P1[0][j]=1;}
    for(int i=1;i<=N;i++){V1[i][0]=0;V2[i][0]=V;P1[i][0]=0;}
    V1[0][0]=V/2;V2[0][0]=V/2;P1[0][0]=0.5;
    for(int s=1;s<=2*N;s++) for(int i=1;i<=N;i++){int j=s-i;if(j<1||j>N)continue;
        double p1=0.5,p2=0.5;
        for(int it=0;it<10000;it++){
            double mv1=(1-p2)*(V1[i-1][j]-V1[i][j])+p2*(V1[i-1][j-1]-V1[i][j-1]);
            double mv2=(1-p1)*(V2[i][j-1]-V2[i][j])+p1*(V2[i-1][j-1]-V2[i-1][j]);
            double np1=mv1>0?1-1.0/(a1*mv1):0; np1=max(0.0,min(0.99,np1));
            double np2=mv2>0?1-1.0/(a2*mv2):0; np2=max(0.0,min(0.99,np2));
            p1=np1;p2=np2;
        }
        double b1=p1>0?log(1-p1)/log(pow(0.99,1-F1)):0;
        double b2=p2>0?log(1-p2)/log(pow(0.99,1-F2)):0;
        V1[i][j]=-b1+p1*(1-p2)*V1[i-1][j]+p2*(1-p1)*V1[i][j-1]+p1*p2*V1[i-1][j-1]+(1-p1)*(1-p2)*V1[i][j];
        V1[i][j]/=(p1+p2-p1*p2);
        V2[i][j]=-b2+p1*(1-p2)*V2[i-1][j]+p2*(1-p1)*V2[i][j-1]+p1*p2*V2[i-1][j-1]+(1-p1)*(1-p2)*V2[i][j];
        V2[i][j]/=(p1+p2-p1*p2);
        P1[i][j]=p1*(1-p2)*P1[i-1][j]+p2*(1-p1)*P1[i][j-1]+p1*p2*P1[i-1][j-1];
        P1[i][j]/=(p1+p2-p1*p2);
    }
    printf("%.9f %.9f\n",P1[N][N],1-P1[N][N]);
}
