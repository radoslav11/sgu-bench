#include <bits/stdc++.h>
using namespace std;

int D[200005], Dlen;

long long solveLen(int L){
    bool odd = (L%2==1);
    int mid = L/2;
    function<long long(int,int,int)> rec=[&](int i,int clIn,int chOutReq)->long long{
        int hi=L-1-i;
        if(i==mid){
            if(odd){
                long long r=0;
                for(int b=0;b<10;b++){int v=2*b+clIn; if(v%10==D[mid] && v/10==chOutReq) r++;}
                return r;
            } else {
                return (clIn==chOutReq)?1:0;
            }
        }
        long long res=0;
        for(int s=0;s<=18;s++){
            int vl=s+clIn;
            if(vl%10!=D[i]) continue;
            int clToInner=vl/10;
            for(int chFromInner=0;chFromInner<2;chFromInner++){
                int vh=s+chFromInner;
                if(vh%10!=D[hi]) continue;
                if(vh/10!=chOutReq) continue;
                int lo=max(0,s-9),hh=min(9,s);
                long long ways=0;
                if(i==0){
                    for(int bi=lo;bi<=hh;bi++){int bh=s-bi; if(bh>=1) ways++;}
                } else ways=hh-lo+1;
                if(!ways) continue;
                res += ways*rec(i+1,clToInner,chFromInner);
            }
        }
        return res;
    };
    return rec(0,0,0);
}

int main(){
    string s;
    while(getline(cin,s)){
        while(!s.empty() && (s.back()=='\r'||s.back()==' '||s.back()=='\t')) s.pop_back();
        if(s.empty()) continue;
        if(s=="0") break;
        Dlen=(int)s.size();
        for(int i=0;i<Dlen;i++) D[i]=s[Dlen-1-i]-'0';
        long long ans=0;
        for(int L=Dlen-1; L<=Dlen; L++){
            if(L<1) continue;
            ans+=solveLen(L);
        }
        cout<<ans<<"\n";
    }
    return 0;
}
