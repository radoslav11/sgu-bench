#include <bits/stdc++.h>
using namespace std;
typedef vector<long long> Big;
const long long BASE=1000000000LL;

Big mulSmall(const Big&a, long long k){
    Big res(a.size()+2,0);
    long long carry=0;
    for(size_t i=0;i<a.size();i++){
        __int128 cur=(__int128)a[i]*k+carry;
        res[i]=(long long)(cur%BASE);
        carry=(long long)(cur/BASE);
    }
    size_t i=a.size();
    while(carry){ res[i]+=carry%BASE; carry/=BASE; i++; }
    while(res.size()>1&&res.back()==0) res.pop_back();
    return res;
}
Big add(const Big&a,const Big&b){
    Big res; long long carry=0;
    size_t n=max(a.size(),b.size());
    for(size_t i=0;i<n||carry;i++){
        long long cur=carry;
        if(i<a.size())cur+=a[i];
        if(i<b.size())cur+=b[i];
        res.push_back(cur%BASE);
        carry=cur/BASE;
    }
    while(res.size()>1&&res.back()==0) res.pop_back();
    return res;
}

int main(){
    long long n,m;
    cin>>n>>m;
    Big c(1,1), r(1,0);
    for(long long step=0;step<m;step++){
        Big nc=mulSmall(r,n);
        Big nr=add(c, mulSmall(r,2));
        c=nc; r=nr;
    }
    string s=to_string(c.back());
    for(int i=(int)c.size()-2;i>=0;i--){
        string t=to_string(c[i]);
        s+=string(9-t.size(),'0')+t;
    }
    cout<<s<<"\n";
}
