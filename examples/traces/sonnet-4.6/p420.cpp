#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll fact[17];

ll countLE(int cnt[10], const string& X){
    int n=X.size();
    ll res=0;
    int cur[10];
    for(int i=0;i<10;i++) cur[i]=cnt[i];
    int rem=n;
    for(int pos=0;pos<n;pos++){
        int lim=X[pos]-'0';
        int start=(pos==0)?1:0;
        for(int d=start;d<lim;d++){
            if(cur[d]>0){
                cur[d]--;
                ll w=fact[rem-1];
                for(int i=0;i<10;i++) w/=fact[cur[i]];
                res+=w;
                cur[d]++;
            }
        }
        if(lim<start || cur[lim]==0) break;
        cur[lim]--;
        rem--;
        if(pos==n-1) res++;
    }
    return res;
}

ll L, R, ans;

ll countInRange(int cnt[10], int n){
    ll nlo=1;
    for(int i=1;i<n;i++) nlo*=10;
    ll nhi=nlo*10-1;
    ll effL=max(L,nlo), effR=min(R,nhi);
    if(effL>effR) return 0;
    
    string sEffL=to_string(effL), sEffR=to_string(effR);
    
    ll cr=countLE(cnt,sEffR);
    ll cl=countLE(cnt,sEffL);
    int tmp[10]={};
    for(char ch:sEffL) tmp[ch-'0']++;
    bool isL=true;
    for(int i=0;i<10;i++) if(tmp[i]!=cnt[i]){isL=false;break;}
    return cr-cl+(isL?1:0);
}

int cnt[10];

void enumerate(int minDigit, int used, int total){
    if(used==total){
        ll c=countInRange(cnt,total);
        if(c==2) ans+=2;
        return;
    }
    for(int d=minDigit;d<=9;d++){
        cnt[d]++;
        enumerate(d,used+1,total);
        cnt[d]--;
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    fact[0]=1;
    for(int i=1;i<=16;i++) fact[i]=fact[i-1]*i;
    cin>>L>>R;
    ans=0;
    memset(cnt,0,sizeof(cnt));
    for(int len=1;len<=16;len++){
        ll nlo=1;
        for(int i=1;i<len;i++) nlo*=10;
        ll nhi=nlo*10-1;
        if(nlo>R || nhi<L) continue;
        enumerate(0,0,len);
    }
    cout<<ans<<endl;
    return 0;
}
