#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;

int N,M;
ll T;
int CC[105];
struct Grp{ll t,p,w,e;};
Grp G[10005];

struct Block{int start;int size;ll endtime;};
vector<Block> seated[105];

struct SeatOption{
    ll minsl,maxsl;
    int cnt,pos;
};

bool isInf(ll v){return v>=(ll)1e17;}

SeatOption evalSeat(int c,int pos,int p,ll cur){
    int leftwall=-1,rightwall=CC[c];
    for(auto&b:seated[c]){
        if(b.endtime>cur){
            int be=b.start+b.size-1;
            int bs=b.start;
            if(be<pos && be>leftwall) leftwall=be;
            if(bs>pos+p-1 && bs<rightwall) rightwall=bs;
        }
    }
    ll SL=(leftwall==-1)?(ll)2e18:(ll)(pos-leftwall-1);
    ll SR=(rightwall==CC[c])?(ll)2e18:(ll)(rightwall-(pos+p-1)-1);
    SeatOption s;
    s.cnt=c;s.pos=pos;
    s.minsl=min(SL,SR);
    s.maxsl=max(SL,SR);
    return s;
}

bool worse(const SeatOption&a,const SeatOption&b){
    if(a.minsl!=b.minsl) return a.minsl<b.minsl;
    if(a.maxsl!=b.maxsl) return a.maxsl<b.maxsl;
    if(a.cnt!=b.cnt) return a.cnt>b.cnt;
    return a.pos>b.pos;
}

pair<int,int> findSeat(int p,ll cur){
    SeatOption best;best.cnt=-1;best.minsl=best.maxsl=0;best.pos=0;
    for(int c=0;c<N;c++){
        if(CC[c]<p) continue;
        vector<bool> free(CC[c],true);
        for(auto&b:seated[c]){
            if(b.endtime>cur){
                for(int i=b.start;i<b.start+b.size;i++) free[i]=false;
            }
        }
        for(int i=0;i<=CC[c]-p;i++){
            bool ok=true;
            for(int j=i;j<i+p;j++) if(!free[j]){ok=false;i=j;break;}// skip
            if(ok){
                SeatOption s=evalSeat(c,i,p,cur);
                if(best.cnt==-1||worse(best,s)) best=s;
            }
        }
    }
    return {best.cnt,best.pos};
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin>>N>>M>>T;
    for(int i=0;i<N;i++) cin>>CC[i];
    for(int i=0;i<M;i++) cin>>G[i].t>>G[i].p>>G[i].w>>G[i].e;
    
    ld totalSat=0;
    int totalCust=0;
    
    // Events: (time, type)
    // type: 0=arrival(group idx), 1=departure(counter,start,size,endtime), 2=giveup(group idx)
    // But give-up is conditional on not being seated yet
    
    // Simulate step by step
    // Maintain waiting queue (FIFO of group indices)
    // At each time point:
    // 1. Add arrivals
    // 2. Process departures (remove from seated)
    // 3. Handle give-ups of front of queue if applicable
    // 4. Try to seat front of queue
    // Repeat 3-4
    
    // Key: give-up events need to be fired at the right time
    // A group gives up at time t+w IF t+w < T and they haven't been seated
    // We need to advance time to the next relevant event
    
    // Events priority queue: (time, type, id)
    // type 0=arrival, 1=departure, 2=giveup
    // When multiple events at same time: 
    //   process arrivals, then departures, then try seating (give-ups happen when we check)
    // Actually let's say: first arrivals join queue, then departures free seats,
    //   then we try to seat front (checking for give-ups first)
    
    // For give-ups: we should add give-up event when group joins queue
    // But if seated before give-up time, ignore the give-up event
    
    vector<bool> seated_flag(M,false);
    vector<bool> gaveup(M,false);
    
    // depQ: (endtime, counter, start, size)
    priority_queue<tuple<ll,int,int,int>,vector<tuple<ll,int,int,int>>,greater<>> depQ;
    
    // arrQ and giveupQ combined into event queue
    // (time, type[0=arr,1=dep,2=giveup], idx)
    // For departures: store in depQ separately
    
    queue<int> waitQ;
    
    // event queue for arrivals and give-ups
    // Actually let's just iterate
    
    int nextArr=0;
    ll now=-1;
    
    auto getNextTime=[&]()->ll{
        ll t=LLONG_MAX;
        if(nextArr<M) t=min(t,G[nextArr].t);
        if(!depQ.empty()) t=min(t,get<0>(depQ.top()));
        // give-up time for front of queue
        if(!waitQ.empty()){
            int gi=waitQ.front();
            ll gut=G[gi].t+G[gi].w;
            if(gut<T) t=min(t,gut);
        }
        return t;
    };
    
    while(nextArr<M||!waitQ.empty()||!depQ.empty()){
        ll t=getNextTime();
        if(t==LLONG_MAX) break;
        now=t;
        
        // Step 1: process arrivals at now
        while(nextArr<M&&G[nextArr].t==now){
            waitQ.push(nextArr);
            nextArr++;
        }
        
        // Step 2: process departures at now
        while(!depQ.empty()&&get<0>(depQ.top())==now){
            auto[et,c,bs,bsz]=depQ.top();depQ.pop();
            for(auto it=seated[c].begin();it!=seated[c].end();++it){
                if(it->start==bs&&it->size==bsz&&it->endtime==et){
                    seated[c].erase(it);break;
                }
            }
        }
        
        // Step 3-4: try to seat front of queue, handling give-ups
        while(!waitQ.empty()){
            int gi=waitQ.front();
            ll gut=G[gi].t+G[gi].w;
            
            // Check if give-up condition: now > gut AND gut < T
            // "within Wi (inclusive)" means wait time <= Wi, so give up if wait > Wi
            // wait = now - G[gi].t > G[gi].w => now > gut
            if(now>gut && gut<T){
                // gave up
                gaveup[gi]=true;
                totalSat+=(-1.0L)*(ld)G[gi].p;
                totalCust+=G[gi].p;
                waitQ.pop();
                continue;
            }
            
            // Try to seat
            auto[c,pos]=findSeat(G[gi].p,now);
            if(c==-1) break;// can't seat now
            
            // Seat them
            seated_flag[gi]=true;
            ll endtime=now+G[gi].e;
            seated[c].push_back({pos,(int)G[gi].p,endtime});
            depQ.push({endtime,c,pos,(int)G[gi].p});
            
            ll wait=now-G[gi].t;
            ld s=(ld)(G[gi].w-wait)/(ld)G[gi].w;
            totalSat+=s*(ld)G[gi].p;
            totalCust+=G[gi].p;
            waitQ.pop();
        }
    }
    
    // Remaining in wait queue (shop closed? Actually shop closing doesn't force them out during simulation)
    // Wait, if gut >= T, they don't give up before closing. But the shop closes at T.
    // "strictly before the closing time" - they give up only if they can do so before T
    // If gut >= T, do they ever get seated? Only if seats become available before T
    // But our simulation only runs until no more events. If gut >= T, they wait indefinitely?
    // Actually closing time T means: groups arriving before T can try to get seated.
    // If they can't be seated and gut>=T, they... never give up? That seems odd.
    // Looking at sample 4: group 1 at t=50,w=20,T=100 -> gut=70<T=100, gives up at 70
    // Let me re-read: "fails to take their seats within the time Wi (inclusive) from their coming 
    //   and strictly before the closing time, they give up"
    // I think "strictly before the closing time" means: the give-up action happens strictly before T
    // So if gut >= T, they don't give up (they wait until T and... then what?)
    // If gut < T, they give up at time gut (if not seated by then)
    // After simulation, any remaining in queue with gut>=T: they waited and shop closed
    // without being seated -> satisfaction = -1? Or they somehow still got seated?
    // Actually if gut>=T they keep waiting even past T, but shop closes at T.
    // I think they go away at T with -1 satisfaction? No...
    // "If a customer starts eating, he is allowed to finish even after the shop closes."
    // So closing just means no new seatings after T. Groups waiting when shop closes go away.
    // But give-up condition only triggers "strictly before closing time".
    // I'll treat: if gut>=T, they keep waiting but once shop closes they can't sit, so -1.
    // For now, after simulation ends, check remaining in queue.
    
    while(!waitQ.empty()){
        int gi=waitQ.front();waitQ.pop();
        // These groups never got seated
        totalSat+=(-1.0L)*(ld)G[gi].p;
        totalCust+=G[gi].p;
    }
    
    if(totalCust==0) cout<<fixed<<setprecision(16)<<0.0L<<endl;
    else cout<<fixed<<setprecision(16)<<totalSat/(ld)totalCust<<endl;
    
    return 0;
}
