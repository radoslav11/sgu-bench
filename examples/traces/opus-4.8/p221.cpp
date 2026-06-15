#include <bits/stdc++.h>
using namespace std;
typedef vector<long long> BigInt;
const long long BASE=1000000000LL;
BigInt fromInt(long long x){BigInt r; if(x==0){r.push_back(0);return r;} while(x){r.push_back(x%BASE);x/=BASE;} return r;}
BigInt addB(const BigInt&a,const BigInt&b){BigInt r;long long c=0;for(size_t i=0;i<max(a.size(),b.size())||c;i++){long long s=c;if(i<a.size())s+=a[i];if(i<b.size())s+=b[i];r.push_back(s%BASE);c=s/BASE;}while(r.size()>1&&r.back()==0)r.pop_back();return r;}
BigInt mulSmall(const BigInt&a,long long m){if(m==0)return fromInt(0);BigInt r;long long c=0;for(size_t i=0;i<a.size()||c;i++){long long s=c;if(i<a.size())s+=a[i]*m;r.push_back(s%BASE);c=s/BASE;}while(r.size()>1&&r.back()==0)r.pop_back();return r;}
BigInt mulB(const BigInt&a,const BigInt&b){vector<long long> tmp(a.size()+b.size(),0);for(size_t i=0;i<a.size();i++){long long c=0;for(size_t j=0;j<b.size()||c;j++){long long cur=tmp[i+j]+c+(j<b.size()?a[i]*b[j]:0);tmp[i+j]=cur%BASE;c=cur/BASE;}}BigInt r(tmp.begin(),tmp.end());while(r.size()>1&&r.back()==0)r.pop_back();return r;}
void printB(const BigInt&a){printf("%lld",a.back());for(int i=(int)a.size()-2;i>=0;i--)printf("%09lld",a[i]);printf("\n");}
int main(){
    int n,k;
    if(scanf("%d %d",&n,&k)!=2)return 0;
    if(k==0){printf("1\n");return 0;}
    vector<int> even, odd;
    for(int s=0;s<=2*n-2;s++){
        int cnt = (s< n)? s+1 : 2*n-1-s;
        if(s%2==0) even.push_back(cnt); else odd.push_back(cnt);
    }
    sort(even.begin(),even.end());
    sort(odd.begin(),odd.end());
    auto solve=[&](vector<int>&v)->vector<BigInt>{
        int m=v.size();
        vector<BigInt> dp(m+1);
        dp[0]=fromInt(1);
        for(int j=1;j<=m;j++)dp[j]=fromInt(0);
        for(int idx=0;idx<m;idx++){
            int len=v[idx];
            for(int j=min(idx+1,m);j>=1;j--){
                long long avail = len-(j-1);
                if(avail<0)avail=0;
                BigInt add = mulSmall(dp[j-1], avail);
                dp[j]=addB(dp[j],add);
            }
        }
        return dp;
    };
    vector<BigInt> de=solve(even);
    vector<BigInt> dox=solve(odd);
    int me=even.size(), mo=odd.size();
    BigInt total=fromInt(0);
    for(int a=0;a<=me&&a<=k;a++){
        int b=k-a;
        if(b<0||b>mo)continue;
        BigInt prod=mulB(de[a],dox[b]);
        total=addB(total,prod);
    }
    printB(total);
    return 0;
}
