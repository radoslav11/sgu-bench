#include <bits/stdc++.h>
using namespace std;

int main(){
    int T;
    scanf("%d",&T);
    while(T--){
        long long n1,m1,n2,m2;
        scanf("%lld %lld %lld %lld",&n1,&m1,&n2,&m2);
        double a1=m1+1, b1=n1-m1+1, a2=m2+1, b2=n2-m2+1;
        double n=n1+1; // a1+b1-1
        double constPart = -lgamma(a2+b2+n) - (lgamma(a2)+lgamma(b2)-lgamma(a2+b2));
        double lgn1 = lgamma(n+1);
        double P=0;
        for(long long j=m1+1;j<=n1+1;j++){
            double lj=(double)j;
            double t = lgn1 - lgamma(lj+1) - lgamma(n-lj+1)
                      + lgamma(a2+lj) + lgamma(b2+n-lj) + constPart;
            P += exp(t);
        }
        printf("%.10f\n", P);
    }
    return 0;
}
