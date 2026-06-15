#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; scanf("%d",&n);
    vector<long long> X(n),Y(n),R(n);
    for(int i=0;i<n;i++) scanf("%lld%lld%lld",&X[i],&Y[i],&R[i]);
    vector<array<double,4>> mat(3,{0,0,0,0});
    int rank=0;
    for(int i=0;i<n&&rank<3;i++){
        array<double,4> eq={-2.0*X[i],-2.0*Y[i],1.0,(double)(R[i]*R[i]-X[i]*X[i]-Y[i]*Y[i])};
        for(int r=0;r<rank;r++){double f=eq[r];for(int j=0;j<4;j++)eq[j]-=f*mat[r][j];}
        int pc=-1;for(int j=0;j<3;j++)if(fabs(eq[j])>1e-9){pc=j;break;}
        if(pc<0)continue;
        double d=eq[pc];for(int j=0;j<4;j++)eq[j]/=d;
        for(int r=0;r<rank;r++){double f=mat[r][pc];for(int j=0;j<4;j++)mat[r][j]-=f*eq[j];}
        mat[rank++]=eq;
    }
    if(rank<3){printf("-2\n");return 0;}
    double a=mat[0][3],b=mat[1][3],w=mat[2][3],r2=a*a+b*b-w;
    if(r2<=1e-9){printf("-1\n");return 0;}
    for(int i=0;i<n;i++){double lhs=-2.0*X[i]*a-2.0*Y[i]*b+w,rhs=(double)(R[i]*R[i]-X[i]*X[i]-Y[i]*Y[i]);if(fabs(lhs-rhs)>1e-3*(1+fabs(rhs))){printf("-1\n");return 0;}}
    printf("%.9f %.9f %.9f\n",a,b,sqrt(r2));
}
