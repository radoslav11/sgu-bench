#include <bits/stdc++.h>
using namespace std;

long long ext(long long a, long long b, long long &x, long long &y){
    if(b==0){x=1;y=0;return a;}
    long long x1,y1;
    long long g=ext(b,a%b,x1,y1);
    x=y1;
    y=x1-(a/b)*y1;
    return g;
}

int main(){
    int n; long long P,B;
    cin>>n>>P>>B;
    vector<long long> A(n);
    for(auto &a:A)cin>>a;
    // find coefficients c_i with sum A_i c_i = g
    vector<long long> coef(n,0);
    long long g=0;
    // g starts 0, coef all 0
    for(int i=0;i<n;i++){
        // combine current g with A[i]
        long long x,y;
        long long ng=ext(g,A[i],x,y);
        // new combination = x*g + y*A[i] = ng
        // existing coef multiplied by x, plus y for index i
        for(int j=0;j<i;j++) coef[j]*=x;
        coef[i]=y;
        g=ng;
    }
    // now sum A_i coef_i = g
    // solve g*k = B (mod P)
    if(g==0){
        // all A zero, sum =0, need B==0
        if(B%P==0){
            cout<<"YES\n";
            for(int i=0;i<n;i++)cout<<0<<(i+1<n?' ':'\n');
            if(n==0)cout<<"\n";
        } else cout<<"NO\n";
        return 0;
    }
    long long d=__gcd(g,P);
    if(B%d!=0){
        cout<<"NO\n";
        return 0;
    }
    long long Pp=P/d;
    long long gp=((g/d)%Pp+Pp)%Pp;
    // inverse of gp mod Pp
    long long x,y;
    ext(gp,Pp,x,y);
    long long inv=((x%Pp)+Pp)%Pp;
    long long k=( ( (B/d)%Pp ) * inv )%Pp;
    // X_i = coef_i * k mod P
    cout<<"YES\n";
    for(int i=0;i<n;i++){
        long long v=((coef[i]%P)*(k%P))%P;
        v=((v%P)+P)%P;
        cout<<v<<(i+1<n?' ':'\n');
    }
    return 0;
}
