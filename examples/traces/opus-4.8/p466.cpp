#include <bits/stdc++.h>
using namespace std;

int N;
unordered_map<int,int> byStart, byEnd; // start->len, end->start
set<pair<int,int>> bySize; // (len,start)

void addInt(int s,int l){
    int e=(s+l-1)%N;
    byStart[s]=l; byEnd[e]=s; bySize.insert({l,s});
}
void remInt(int s,int l){
    int e=(s+l-1)%N;
    byStart.erase(s); byEnd.erase(e); bySize.erase({l,s});
}

string fmtRange(int a,int b){
    if(a==b) return to_string(a);
    return to_string(a)+"-"+to_string(b);
}

string formatAlloc(int start,int S){
    // start 0-indexed
    int s1=start+1;
    if(s1+S-1<=N){
        return fmtRange(s1, s1+S-1);
    } else {
        int firstCount = N - s1 + 1;
        int secondCount = S - firstCount;
        string sec = fmtRange(1, secondCount);
        string fir = fmtRange(s1, N);
        return sec + "," + fir;
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int Q;
    cin >> N >> Q;
    string grid;
    cin >> grid;
    
    byStart.reserve(2*N+10);
    byEnd.reserve(2*N+10);
    
    // parse free intervals
    int firstX = -1;
    for(int i=0;i<N;i++) if(grid[i]=='X'){ firstX=i; break; }
    if(firstX==-1){
        // all free
        addInt(0,N);
    } else {
        int p0=firstX;
        int runStart=-1, runLen=0;
        for(int i=0;i<N;i++){
            int pos=(p0+i)%N;
            if(grid[pos]=='.'){
                if(runStart==-1){ runStart=pos; runLen=0; }
                runLen++;
            } else {
                if(runStart!=-1){
                    addInt(runStart, runLen);
                    runStart=-1; runLen=0;
                }
            }
        }
        if(runStart!=-1){
            addInt(runStart, runLen);
        }
    }
    
    vector<pair<int,int>> alloc(Q+1, {-1,-1});
    
    string out;
    out.reserve(1<<20);
    
    for(int q=1;q<=Q;q++){
        string cmd;
        cin >> cmd;
        if(cmd=="PARK"){
            int S; cin >> S;
            auto it = bySize.lower_bound({S, -1});
            if(it==bySize.end()){
                out += "NO ROOM\n";
            } else {
                int len = it->first;
                int start = it->second;
                remInt(start, len);
                if(len > S){
                    int ns = (start+S)%N;
                    int nl = len - S;
                    addInt(ns, nl);
                }
                alloc[q] = {start, S};
                out += formatAlloc(start, S);
                out += "\n";
            }
        } else {
            int qi; cin >> qi;
            int st = alloc[qi].first;
            int S = alloc[qi].second;
            // free positions st..st+S-1
            int leftPos = (st - 1 + N) % N;
            int rightPos = (st + S) % N;
            
            bool hasL=false; int Ls=0, Ll=0;
            auto itE = byEnd.find(leftPos);
            if(itE != byEnd.end()){ Ls=itE->second; Ll=byStart[Ls]; hasL=true; }
            
            bool hasR=false; int Rs=0, Rl=0;
            auto itS = byStart.find(rightPos);
            if(itS != byStart.end()){ Rs=rightPos; Rl=itS->second; hasR=true; }
            
            if(hasL && hasR && Ls==Rs){
                // same interval -> full circle
                remInt(Ls, Ll);
                addInt(0, N);
            } else {
                int ns=st, nl=S;
                if(hasL){ remInt(Ls,Ll); ns=Ls; nl+=Ll; }
                if(hasR){ remInt(Rs,Rl); nl+=Rl; }
                if(nl==N) ns=0;
                addInt(ns, nl);
            }
        }
    }
    
    cout << out;
    return 0;
}
