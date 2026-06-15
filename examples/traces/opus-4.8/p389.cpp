#include <bits/stdc++.h>
using namespace std;
const long long MOD=1000000007;
int main(){
    string A,B,C;
    getline(cin,A);getline(cin,B);getline(cin,C);
    // strip whitespace
    auto strip=[](string&s){string r;for(char c:s)if(c=='0'||c=='1')r+=c;s=r;};
    strip(A);strip(B);strip(C);
    int n=A.size();
    long long s=0,nA=0,nB=0,nC=0;
    for(int i=0;i<n;i++){
        int a=A[i]-'0',b=B[i]-'0',c=C[i]-'0';
        if(a==b&&b==c){s++;}
        else if(a!=b&&a!=c){ // a differs from both, b==c -> type A
            nA++;
        } else if(b!=a&&b!=c){ // b differs from both -> a==c, type B
            nB++;
        } else { // c differs -> type C
            nC++;
        }
    }
    // precompute factorials
    int maxN=max((long long)1,max(nA,max(nB,nC)));
    vector<long long> fact(maxN+1),inv(maxN+1);
    fact[0]=1;for(int i=1;i<=maxN;i++)fact[i]=fact[i-1]*i%MOD;
    // modpow
    function<long long(long long,long long)> pw=[&](long long b,long long e){
        long long r=1;b%=MOD;while(e){if(e&1)r=r*b%MOD;b=b*b%MOD;e>>=1;}return r;
    };
    inv[maxN]=pw(fact[maxN],MOD-2);
    for(int i=maxN;i>0;i--)inv[i-1]=inv[i]*i%MOD;
    auto Cc=[&](long long nn,long long kk)->long long{
        if(kk<0||kk>nn||nn<0)return 0;
        return fact[nn]*inv[kk]%MOD*inv[nn-kk]%MOD;
    };
    long long ans=0;
    // need (nB+nA) even and (nC+nA) even
    if(((nB+nA)%2==0)&&((nC+nA)%2==0)){
        long long offB=(nB+nA)/2;
        long long offC=(nC+nA)/2;
        for(long long i=0;i<=nA;i++){
            long long t=Cc(nA,i)*Cc(nB,offB-i)%MOD*Cc(nC,offC-i)%MOD;
            ans=(ans+t)%MOD;
        }
    }
    ans=ans*pw(2,s)%MOD;
    cout<<ans<<endl;
}
