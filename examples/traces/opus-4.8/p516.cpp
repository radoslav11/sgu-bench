#include <bits/stdc++.h>
using namespace std;

static long long toMin(const string&d,const string&t){
    int mon=(d[5]-'0')*10+(d[6]-'0');
    int day=(d[8]-'0')*10+(d[9]-'0');
    int hh=(t[0]-'0')*10+(t[1]-'0');
    int mm=(t[3]-'0')*10+(t[4]-'0');
    static const int cum[]={0,31,59,90,120,151,181,212,243,273,304,334};
    long long dayIdx = cum[mon-1] + (day-1);
    return dayIdx*1440LL + hh*60 + mm;
}

long long T1g, T2g;
long long thr;
const int W=53;
vector<long long> counts;
long long violationCount=0;
map<long long,int> mp;

inline long long weekOf(long long t){ return (t+4320)/10080; }
inline long long weekStart(long long w){ return w*10080 - 4320; }

inline long long working(long long a,long long b,long long Ws){
    long long ra=a-Ws, rb=b-Ws;
    long long wk=0;
    for(int dday=0;dday<5;dday++){
        long long ws=(long long)dday*1440+540, we=(long long)dday*1440+1080;
        long long lo=max(ra,ws), hi=min(rb,we);
        if(hi>lo) wk+=hi-lo;
    }
    return wk;
}

void contribute(long long l,long long r,int g,int sign){
    if(l>=r) return;
    long long w0=weekOf(l), w1=weekOf(r-1);
    for(long long w=w0; w<=w1; ++w){
        long long Ws=weekStart(w), We=Ws+10080;
        long long a=max(l,Ws), b=min(r,We);
        if(a>=b) continue;
        long long nw=(b-a)-working(a,b,Ws);
        if(nw==0) continue;
        long long key=(long long)g*W + w;
        long long old=counts[key];
        long long nv=old + (long long)sign*nw;
        bool wasV = old>thr;
        bool isV = nv>thr;
        if(wasV && !isV) violationCount--;
        if(!wasV && isV) violationCount++;
        counts[key]=nv;
    }
}

void split(long long pos){
    if(pos<=T1g || pos>=T2g) return;
    auto it=mp.upper_bound(pos);
    --it;
    if(it->first==pos) return;
    mp[pos]=it->second;
}

void assign(long long B,long long E,int g){
    if(B<T1g) B=T1g;
    if(E>T2g) E=T2g;
    if(B>=E) return;
    split(B);
    split(E);
    auto it=mp.lower_bound(B);
    while(it!=mp.end() && it->first<E){
        auto nx=next(it);
        long long iend=(nx==mp.end()? T2g : nx->first);
        contribute(it->first, iend, it->second, -1);
        it=mp.erase(it);
    }
    mp[B]=g;
    contribute(B,E,g,+1);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N,M; long long Th;
    cin>>N>>M>>Th;
    thr = Th*60;
    string t1d,t1t,t2d,t2t;
    cin>>t1d>>t1t>>t2d>>t2t;
    T1g=toMin(t1d,t1t);
    T2g=toMin(t2d,t2t);
    
    unordered_map<string,int> gidMap;
    gidMap.reserve((N+M)*2+10);
    
    vector<int> initGid(N);
    vector<long long> initStart(N);
    for(int i=0;i<N;i++){
        string d,t,name;
        cin>>d>>t>>name;
        initStart[i]=toMin(d,t);
        auto it=gidMap.find(name);
        int g;
        if(it==gidMap.end()){ g=gidMap.size(); gidMap[name]=g; }
        else g=it->second;
        initGid[i]=g;
    }
    
    struct Upd{ long long A,B,E; int g; };
    vector<Upd> ups(M);
    for(int i=0;i<M;i++){
        string aD,aT,bD,bT,eD,eT,name;
        cin>>aD>>aT>>bD>>bT>>eD>>eT>>name;
        ups[i].A=toMin(aD,aT);
        ups[i].B=toMin(bD,bT);
        ups[i].E=toMin(eD,eT);
        auto it=gidMap.find(name);
        int g;
        if(it==gidMap.end()){ g=gidMap.size(); gidMap[name]=g; }
        else g=it->second;
        ups[i].g=g;
    }
    
    int numGuard=gidMap.size();
    counts.assign((long long)numGuard*W, 0);
    
    // build base map
    const long long INF = LLONG_MAX/2;
    for(int i=0;i<N;i++){
        long long segS=initStart[i];
        long long segE=(i+1<N? initStart[i+1] : INF);
        long long s=max(segS,T1g), e=min(segE,T2g);
        if(s<e){ mp[s]=initGid[i]; }
    }
    // add contributions for base
    for(auto it=mp.begin(); it!=mp.end(); ++it){
        auto nx=next(it);
        long long end=(nx==mp.end()? T2g : nx->first);
        contribute(it->first, end, it->second, +1);
    }
    
    // apply updates with A<=T1
    int idx=0;
    for(; idx<M && ups[idx].A<=T1g; idx++){
        assign(ups[idx].B, ups[idx].E, ups[idx].g);
    }
    
    long long prev=T1g;
    long long total=0;
    for(; idx<M; idx++){
        long long A=ups[idx].A;
        if(A>=T2g) break;
        if(violationCount==0) total += (A-prev);
        prev=A;
        assign(ups[idx].B, ups[idx].E, ups[idx].g);
    }
    if(violationCount==0) total += (T2g-prev);
    
    double ans = (double)total/(double)(T2g-T1g);
    cout<<setprecision(17)<<ans<<"\n";
    return 0;
}
