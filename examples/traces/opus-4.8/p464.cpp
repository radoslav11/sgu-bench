#include <bits/stdc++.h>
using namespace std;
int N; double F1,F2,V,L1,L2;
double U1[12][12],U2[12][12];
double cost(double p,double L){double b=(log(1-p)-log(0.99))/(-L);return b;}
int main(){
    cin>>N>>F1>>F2>>V;L1=-log(1-F1);L2=-log(1-F2);
    auto T1=[&](int i,int j){if(i==0&&j==0)return V/2;if(i==0)return V;return 0.0;};
    auto T2=[&](int i,int j){if(i==0&&j==0)return V/2;if(j==0)return V;return 0.0;};
    auto val1=[&](int i,int j){if(i<=0||j<=0)return T1(i,j);return U1[i][j];};
    auto val2=[&](int i,int j){if(i<=0||j<=0)return T2(i,j);return U2[i][j];};
    for(int i=1;i<=N;i++)for(int j=1;j<=N;j++){
        double p1=0.5,p2=0.5;
        for(int it=0;it<200000;it++){
            double u1=U1[i][j],u2=U2[i][j];
            // fixed point of stay value with current p1,p2
            for(int k=0;k<100;k++){
                double g11=val1(i-1,j-1),g10=val1(i-1,j),g01=val1(i,j-1);
                double nu1=p1*(p2*g11+(1-p2)*g10)+(1-p1)*p2*g01+(1-p1)*(1-p2)*u1-cost(p1,L1);
                double h11=val2(i-1,j-1),h10=val2(i-1,j),h01=val2(i,j-1);
                double nu2=p2*(p1*h11+(1-p1)*h01)+(1-p2)*p1*h10+(1-p1)*(1-p2)*u2-cost(p2,L2);
                u1=nu1;u2=nu2;
            }
            double g10=val1(i-1,j),g11=val1(i-1,j-1),g01=val1(i,j-1);
            double M1=p2*g11+(1-p2)*g10-p2*g01-(1-p2)*u1;
            double np1=(M1*L1>0)?1-1/(M1*L1):0.01;np1=max(0.01,min(0.999999,np1));
            double h10=val2(i-1,j),h11=val2(i-1,j-1),h01=val2(i,j-1);
            double M2=p1*h11+(1-p1)*h01-p1*h10-(1-p1)*u2;
            double np2=(M2*L2>0)?1-1/(M2*L2):0.01;np2=max(0.01,min(0.999999,np2));
            p1+=0.3*(np1-p1);p2+=0.3*(np2-p2);
            U1[i][j]=u1;U2[i][j]=u2;
            if(fabs(np1-p1)<1e-12&&fabs(np2-p2)<1e-12&&it>1000)break;
        }
        // store p for winprob? recompute via separate array - reuse by recompute
        // store probabilities
    }
    // compute win prob using value-derived: need p's; redo computing winprob with same p logic
    // Instead recompute winprobs separately storing p1,p2 per state
    static double WP1[12][12],WP2[12][12],PP1[12][12],PP2[12][12];
    // recompute p's
    for(int i=1;i<=N;i++)for(int j=1;j<=N;j++){
        double p1=0.5,p2=0.5;
        for(int it=0;it<200000;it++){
            double u1=U1[i][j],u2=U2[i][j];
            for(int k=0;k<100;k++){
                double g11=val1(i-1,j-1),g10=val1(i-1,j),g01=val1(i,j-1);
                u1=p1*(p2*g11+(1-p2)*g10)+(1-p1)*p2*g01+(1-p1)*(1-p2)*u1-cost(p1,L1);
                double h11=val2(i-1,j-1),h10=val2(i-1,j),h01=val2(i,j-1);
                u2=p2*(p1*h11+(1-p1)*h01)+(1-p2)*p1*h10+(1-p1)*(1-p2)*u2-cost(p2,L2);
            }
            double g10=val1(i-1,j),g11=val1(i-1,j-1),g01=val1(i,j-1);
            double M1=p2*g11+(1-p2)*g10-p2*g01-(1-p2)*u1;
            double np1=(M1*L1>0)?1-1/(M1*L1):0.01;np1=max(0.01,min(0.999999,np1));
            double h10=val2(i-1,j),h11=val2(i-1,j-1),h01=val2(i,j-1);
            double M2=p1*h11+(1-p1)*h01-p1*h10-(1-p1)*u2;
            double np2=(M2*L2>0)?1-1/(M2*L2):0.01;np2=max(0.01,min(0.999999,np2));
            p1+=0.3*(np1-p1);p2+=0.3*(np2-p2);
            if(fabs(np1-p1)<1e-12&&it>2000)break;
        }
        PP1[i][j]=p1;PP2[i][j]=p2;
    }
    auto w1=[&](int i,int j){if(i==0&&j==0)return 0.5;if(i==0)return 1.0;if(j==0)return 0.0;return WP1[i][j];};
    for(int i=1;i<=N;i++)for(int j=1;j<=N;j++){
        double p1=PP1[i][j],p2=PP2[i][j];
        double w=0.5;
        for(int k=0;k<200;k++){
            w=p1*p2*w1(i-1,j-1)+p1*(1-p2)*w1(i-1,j)+(1-p1)*p2*w1(i,j-1)+(1-p1)*(1-p2)*w;
        }
        WP1[i][j]=w;
    }
    printf("%.9f %.9f\n",WP1[N][N],1-WP1[N][N]);
}
