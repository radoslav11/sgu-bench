#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef __int128 lll;

long double ld128(lll v){
    bool neg=false;
    if(v<0){neg=true; v=-v;}
    long double r=0;
    // split
    unsigned long long low = (unsigned long long)(v & ((((lll)1)<<64)-1));
    unsigned long long high = (unsigned long long)(v >> 64);
    r = (long double)high * 18446744073709551616.0L + (long double)low;
    return neg? -r : r;
}

int main(){
    int n;
    if(scanf("%d",&n)!=1) return 0;
    vector<ll> X(n),Y(n),R(n);
    for(int i=0;i<n;i++){
        scanf("%lld %lld %lld",&X[i],&Y[i],&R[i]);
    }
    ll x1=X[0],y1=Y[0],r1=R[0];
    // rows: A x + B y = C  where A=2(xi-x1),B=2(yi-y1), C=(xi^2+yi^2-ri^2)-(x1^2+y1^2-r1^2)
    vector<ll> A,B,C;
    ll base = x1*x1 + y1*y1 - r1*r1;
    for(int i=1;i<n;i++){
        ll a=2*(X[i]-x1);
        ll b=2*(Y[i]-y1);
        ll c=(X[i]*X[i]+Y[i]*Y[i]-R[i]*R[i]) - base;
        A.push_back(a); B.push_back(b); C.push_back(c);
    }
    int m=A.size();
    
    // find first row with (A,B)!=(0,0)
    int idx1=-1;
    for(int i=0;i<m;i++){
        if(A[i]!=0 || B[i]!=0){ idx1=i; break;}
    }
    
    if(idx1==-1){
        // all coefficient rows zero
        bool inconsistent=false;
        for(int i=0;i<m;i++){
            if(C[i]!=0){ inconsistent=true; break;}
        }
        if(inconsistent){ printf("-1\n"); }
        else { printf("-2\n"); } // free plane
        return 0;
    }
    
    ll A1=A[idx1],B1=B[idx1],C1=C[idx1];
    // find row2 not parallel
    int idx2=-1;
    for(int i=0;i<m;i++){
        ll cr = A1*B[i]-A[i]*B1;
        if(cr!=0){ idx2=i; break;}
    }
    
    if(idx2==-1){
        // rank 1, check consistency
        bool inconsistent=false;
        for(int i=0;i<m;i++){
            if(A[i]==0 && B[i]==0){
                if(C[i]!=0){ inconsistent=true; break;}
            } else {
                // must be scalar multiple including C
                // A*C1 == C*A1 and B*C1 == C*B1
                lll t1 = (lll)A[i]*C1 - (lll)C[i]*A1;
                lll t2 = (lll)B[i]*C1 - (lll)C[i]*B1;
                if(t1!=0 || t2!=0){ inconsistent=true; break;}
            }
        }
        if(inconsistent){ printf("-1\n"); }
        else { printf("-2\n"); } // line of solutions, R^2 unbounded
        return 0;
    }
    
    // rank 2: unique solution
    ll A2=A[idx2],B2=B[idx2],C2=C[idx2];
    lll D = (lll)A1*B2 - (lll)A2*B1;
    lll Nx = (lll)C1*B2 - (lll)C2*B1;
    lll Ny = (lll)A1*C2 - (lll)A2*C1;
    
    // verify all
    bool inconsistent=false;
    for(int i=0;i<m;i++){
        lll lhs = (lll)A[i]*Nx + (lll)B[i]*Ny;
        lll rhs = (lll)C[i]*D;
        if(lhs!=rhs){ inconsistent=true; break;}
    }
    if(inconsistent){ printf("-1\n"); return 0; }
    
    long double Dl = ld128(D);
    long double x = ld128(Nx)/Dl;
    long double y = ld128(Ny)/Dl;
    long double dx = x - (long double)x1;
    long double dy = y - (long double)y1;
    long double R2 = dx*dx + dy*dy - (long double)r1*(long double)r1;
    
    if(R2 > 1e-6L){
        long double Rad = sqrtl(R2);
        printf("%.10Lf %.10Lf %.10Lf\n", x, y, Rad);
    } else {
        printf("-1\n");
    }
    return 0;
}
