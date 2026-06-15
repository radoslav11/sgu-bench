#include <bits/stdc++.h>
using namespace std;
typedef vector<int> BigInt;
const int BASE = 1000000000;
BigInt fromInt(long long x){BigInt r;if(!x){r.push_back(0);return r;}while(x){r.push_back(x%BASE);x/=BASE;}return r;}
BigInt add(const BigInt&a,const BigInt&b){BigInt r;int c=0;for(int i=0;i<(int)max(a.size(),b.size())||c;i++){int s=c+(i<(int)a.size()?a[i]:0)+(i<(int)b.size()?b[i]:0);r.push_back(s%BASE);c=s/BASE;}return r;}
BigInt mul(const BigInt&a,long long b){BigInt r;long long c=0;for(int i=0;i<(int)a.size()||c;i++){long long s=c+(i<(int)a.size()?(long long)a[i]*b:0);r.push_back(s%BASE);c=s/BASE;}while(r.size()>1&&r.back()==0)r.pop_back();return r;}
void print(const BigInt&a){printf("%d",a.back());for(int i=(int)a.size()-2;i>=0;i--)printf("%09d",a[i]);}
int main(){
    int n,m;scanf("%d%d",&n,&m);
    BigInt A=fromInt(1),B=fromInt(0);
    for(int i=0;i<m;i++){BigInt nA=B;BigInt nB=add(mul(A,n),mul(B,2));A=nA;B=nB;}
    print(A);printf("\n");
}
