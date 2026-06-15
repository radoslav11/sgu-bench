#include<cstdio>
#include<map>
#include<cstring>
using namespace std;
typedef long long ll;
const int MAXN=1000002;
ll cell[MAXN];
int fenC[MAXN];
ll fenS[MAXN];
map<int,int>L2R;
map<int,ll>L2S;
void updC(int i,int v){for(;i<MAXN;i+=i&-i)fenC[i]+=v;}
int qC(int i){int s=0;for(;i>0;i-=i&-i)s+=fenC[i];return s;}
void updS(int i,ll v){for(;i<MAXN;i+=i&-i)fenS[i]+=v;}
ll qS(int i){ll s=0;for(;i>0;i-=i&-i)s+=fenS[i];return s;}
int findKth(int k){
    int pos=0;
    for(int pw=(1<<20);pw;pw>>=1){
        int np=pos+pw;
        if(np<MAXN&&fenC[np]<k){k-=fenC[np];pos=np;}
    }
    return pos+1;
}
void addTow(int l,int r,ll s){L2R[l]=r;L2S[l]=s;updC(l,1);updS(l,s);}
void remTow(int l){ll s=L2S[l];L2R.erase(l);L2S.erase(l);updC(l,-1);updS(l,-s);}
int findTow(int x){
    auto it=L2R.upper_bound(x);
    if(it==L2R.begin())return -1;
    --it;
    if(it->second>=x)return it->first;
    return -1;
}
void putCell(int x,ll c){
    ll old=cell[x];
    cell[x]+=c;
    if(old>0){
        int L=findTow(x);
        L2S[L]+=c;updS(L,c);
    } else {
        int lL=-1,rL=-1;
        if(x>1&&cell[x-1]>0)lL=findTow(x-1);
        if(x<1000001&&cell[x+1]>0)rL=findTow(x+1);
        int nl=x,nr=x;ll ns=c;
        if(lL!=-1){nl=lL;ns+=L2S[lL];remTow(lL);}
        if(rL!=-1){nr=L2R[rL];ns+=L2S[rL];remTow(rL);}
        addTow(nl,nr,ns);
    }
}
static char ibuf[1<<25];
static int ipos=0,ilen=0;
inline int gc(){
    if(ipos==ilen){ilen=(int)fread(ibuf,1,sizeof(ibuf),stdin);ipos=0;}
    return ipos<ilen?(unsigned char)ibuf[ipos++]:-1;
}
inline ll readInt(){
    int c;ll v=0;
    while((c=gc())<'0'||c>'9');
    do{v=v*10+(c-'0');}while((c=gc())>='0'&&c<='9');
    return v;
}
inline void readWord(char*s){
    int c,i=0;
    while((c=gc())<'a'||c>'z');
    do{s[i++]=(char)c;}while((c=gc())>='a'&&c<='z');
    s[i]=0;
}
static char obuf[1<<25];
static int opos=0;
inline void flush(){fwrite(obuf,1,opos,stdout);opos=0;}
inline void pc(char c){obuf[opos++]=(unsigned char)c;if(opos==(1<<25))flush();}
inline void writeLL(ll v){
    if(v==0){pc('0');return;}
    char tmp[25];int i=0;
    while(v){tmp[i++]='0'+v%10;v/=10;}
    while(i--)pc(tmp[i+1]... 
