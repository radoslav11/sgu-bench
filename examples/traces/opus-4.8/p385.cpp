#include <bits/stdc++.h>
using namespace std;

int n;
long double fact[105];

long double binom(int a, int b){
    if(b<0||b>a) return 0;
    return fact[a]/(fact[b]*fact[a-b]);
}

int main(){
    cin>>n;
    fact[0]=1;
    for(int i=1;i<=n;i++) fact[i]=fact[i-1]*i;
    
    // g(m,k): permutations of k labeled elements with all cycle lengths in [2,m]
    // we need g(L, n-L) for each L. and D = g(n,n).
    // compute g for each m via DP up to k = n.
    // store g[m][k]
    // g(m,k) = sum_{j=2}^{min(m,k)} binom(k-1,j-1)*fact[j-1]*g(m,k-j)
    
    auto computeG = [&](int m)->vector<long double>{
        vector<long double> g(n+1, 0.0L);
        g[0]=1;
        for(int k=1;k<=n;k++){
            long double s=0;
            for(int j=2;j<=min(m,k);j++){
                s += binom(k-1,j-1)*fact[j-1]*g[k-j];
            }
            g[k]=s;
        }
        return g;
    };
    
    // D = g(n,n)
    vector<long double> gn = computeG(n);
    long double D = gn[n];
    
    long double S=0;
    for(int L=2;L<=n;L++){
        vector<long double> g = computeG(L);
        // g(L, n-L)
        long double term = (long double)L * binom(n,L) * fact[L-1] * g[n-L];
        S += term;
    }
    
    long double E = S / D;
    cout<<fixed<<setprecision(10)<<(double)E<<"\n";
    return 0;
}
