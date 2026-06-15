#include <bits/stdc++.h>
using namespace std;
int main(){
    int T; cin>>T;
    while(T--){
        int n1,m1,n2,m2; cin>>n1>>m1>>n2>>m2;
        double a=m1+1,b=n1-m1+1,c=m2+1,d=n2-m2+1;
        double logBab=lgamma(a)+lgamma(b)-lgamma(a+b);
        double logBcd=lgamma(c)+lgamma(d)-lgamma(c+d);
        int N=20000; double h=1.0/N;
        double cumP=0,ans=0,prev_fp=0;
        for(int i=1;i<=N;i++){
            double q=i*h;
            double lq=(a-1>0?log(q)*(a-1):0)+(b-1>0?log(1-q)*(b-1):0);
            double fp_q=exp(lq-logBab);
            double lq2=(c-1>0?log(q)*(c-1):0)+(d-1>0?log(1-q)*(d-1):0);
            double fq_q=exp(lq2-logBcd);
            cumP+=(prev_fp+fp_q)*h/2.0;
            ans+=cumP*fq_q*h;
            prev_fp=fp_q;
        }
        printf("%.10f\n",ans);
    }
}
