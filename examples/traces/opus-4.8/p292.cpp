#include <bits/stdc++.h>
using namespace std;
struct Big {
    vector<int> d;
    static const int BASE=1000000000;
    Big(){}
    Big(const string&s){
        for(int i=(int)s.size();i>0;i-=9){
            int st=max(0,i-9);
            d.push_back(stoi(s.substr(st,i-st)));
        }
        trim();
    }
    void trim(){ while(d.size()>1&&d.back()==0)d.pop_back(); if(d.empty())d.push_back(0);}
};
Big add(const Big&a,const Big&b){
    Big r; long long carry=0;
    for(size_t i=0;i<a.d.size()||i<b.d.size()||carry;i++){
        long long cur=carry;
        if(i<a.d.size())cur+=a.d[i];
        if(i<b.d.size())cur+=b.d[i];
        r.d.push_back(cur%Big::BASE); carry=cur/Big::BASE;
    }
    r.trim(); return r;
}
Big mul(const Big&a,const Big&b){
    vector<long long> tmp(a.d.size()+b.d.size(),0);
    for(size_t i=0;i<a.d.size();i++){
        long long carry=0;
        for(size_t j=0;j<b.d.size()||carry;j++){
            long long cur=tmp[i+j]+carry+(j<b.d.size()?(long long)a.d[i]*b.d[j]:0);
            tmp[i+j]=cur%Big::BASE; carry=cur/Big::BASE;
        }
    }
    Big r; for(auto x:tmp)r.d.push_back((int)x); r.trim(); return r;
}
pair<Big,Big> divmod(const Big&a,const Big&b){
    Big q; q.d.assign(a.d.size(),0);
    Big cur; cur.d.assign(1,0);
    for(int i=(int)a.d.size()-1;i>=0;i--){
        cur.d.insert(cur.d.begin(),a.d[i]); cur.trim();
        int lo=0,hi=Big::BASE-1,x=0;
        while(lo<=hi){
            int mid=(lo+hi)/2;
            Big m; m.d.assign(1,mid); m.trim();
            Big prod=mul(b,m);
            bool le;
            if(prod.d.size()!=cur.d.size())le=prod.d.size()<cur.d.size();
            else{ le=true; for(int k=prod.d.size()-1;k>=0;k--){ if(prod.d[k]!=cur.d[k]){le=prod.d[k]<cur.d[k];break;}}}
            if(le){x=mid;lo=mid+1;}else hi=mid-1;
        }
        q.d[i]=x;
        Big m; m.d.assign(1,x); m.trim();
        Big prod=mul(b,m);
        long long borrow=0;
        for(size_t k=0;k<cur.d.size();k++){
            long long sub=cur.d[k]-borrow-(k<prod.d.size()?prod.d[k]:0);
            if(sub<0){sub+=Big::BASE;borrow=1;}else borrow=0;
            cur.d[k]=(int)sub;
        }
        cur.trim();
    }
    q.trim();
    return {q,cur};
}
string toStr(const Big&a){
    string s=to_string(a.d.back());
    for(int i=(int)a.d.size()-2;i>=0;i--){ string t=to_string(a.d[i]); s+=string(9-t.size(),'0')+t;}
    return s;
}
int main(){
    string qs,cs,ns; cin>>qs>>cs>>ns;
    Big q(qs),c(cs),n(ns);
    auto cdv=divmod(c,n);
    auto qdv=divmod(q,n);
    Big r1=mul(cdv.first,q);
    Big r2=mul(qdv.first,cdv.second);
    Big ans=add(r1,r2);
    cout<<toStr(ans)<<"\n";
}
