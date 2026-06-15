#include <bits/stdc++.h>
using namespace std;
int B,N;
typedef vector<int> BigN;
int main(){
    cin>>B>>N;
    string alpha="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    vector<BigN> cands;
    for(int x=0;x<B;x++) if((long long)x*x%B==x%B){BigN v(N,0);v[0]=x;cands.push_back(v);}
    for(int k=1;k<N;k++){
        vector<BigN> next;
        for(auto& x:cands){
            // compute digit k of x^2 - x
            long long s=0;
            for(int i=0;i<=k;i++) s+=(long long)x[i]*x[k-i];
            // subtract x[k]
            s-=x[k];
            // but we need carry from lower digits
            // Actually we need to track carries properly
            // Let's compute x^2 digit by digit up to k
            // digit j of x^2 = sum_{i=0}^{j} x[i]*x[j-i], plus carry
            long long carry=0;
            int qk=0;
            for(int j=0;j<=k;j++){
                long long col=carry;
                for(int i=0;i<=j;i++) col+=(long long)x[i]*x[j-i];
                if(j<k){carry=col/B;}
                else{qk=(col-x[k])%B; if(qk<0)qk+=B;} // wait need carry subtraction of x
            }
            // Hmm this is getting complex, let me just do it simply
            // recompute carry-aware
            // Actually let me just track digit k of x^2 with carries
            // Restart: compute x^2 mod B^(k+1) then subtract x mod B^(k+1), get digit k
            // but that's O(k^2) anyway
            // compute digit k of x^2 including carry propagation
            vector<long long> tmp(k+1,0);
            for(int i=0;i<=k;i++) for(int j=0;j<=k-i;j++) tmp[i+j]+=(long long)x[i]*x[j];
            long long cr=0;
            for(int j=0;j<=k;j++){tmp[j]+=cr;cr=tmp[j]/B;tmp[j]%=B;}
            int q=(tmp[k]-x[k]%B+B)%B;
            int twox1=(2LL*x[0]%B-1+B)%B;
            for(int d=0;d<B;d++){
                if((q+(long long)d*twox1)%B==0){
                    BigN nx=x;nx[k]=d;next.push_back(nx);
                }
            }
        }
        cands=next;
    }
    vector<BigN> res;
    for(auto& x:cands) if(x[N-1]!=0) res.push_back(x);
    cout<<res.size()<<"\n";
    for(auto& x:res){string s(N,'0');for(int i=0;i<N;i++)s[N-1-i]=alpha[x[i]];cout<<s<<"\n";}
}
