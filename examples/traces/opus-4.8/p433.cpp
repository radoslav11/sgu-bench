#include <bits/stdc++.h>
using namespace std;

int N, M, K;
vector<int> lens;
vector<int> cellBrick; // brick id per cell, -1 empty
vector<char> filledArr;
int nextId;
unordered_set<string> failedSet;

string packKey(){
    int total = N*M;
    int bytes = (total+7)/8;
    string s(bytes, 0);
    for(int i=0;i<total;i++){
        if(filledArr[i]) s[i>>3] |= (1<<(i&7));
    }
    return s;
}

bool solve(){
    int total = N*M;
    int pos=-1;
    for(int i=0;i<total;i++) if(!filledArr[i]){ pos=i; break; }
    if(pos==-1) return true;
    string key = packKey();
    if(failedSet.count(key)) return false;
    int r = pos/M, c = pos%M;
    for(int L : lens){
        // horizontal
        if(c + L <= M){
            bool ok=true;
            for(int j=0;j<L;j++){ if(filledArr[r*M + c+j]){ ok=false; break; } }
            if(ok){
                int id = nextId++;
                for(int j=0;j<L;j++){ int idx=r*M + c+j; filledArr[idx]=1; cellBrick[idx]=id; }
                if(solve()) return true;
                for(int j=0;j<L;j++){ int idx=r*M + c+j; filledArr[idx]=0; cellBrick[idx]=-1; }
                nextId--;
            }
        }
        // vertical
        if(L>1 && r + L <= N){
            bool ok=true;
            for(int j=0;j<L;j++){ if(filledArr[(r+j)*M + c]){ ok=false; break; } }
            if(ok){
                int id = nextId++;
                for(int j=0;j<L;j++){ int idx=(r+j)*M + c; filledArr[idx]=1; cellBrick[idx]=id; }
                if(solve()) return true;
                for(int j=0;j<L;j++){ int idx=(r+j)*M + c; filledArr[idx]=0; cellBrick[idx]=-1; }
                nextId--;
            }
        }
    }
    failedSet.insert(key);
    return false;
}

int main(){
    // input order: N M K  (but output is N lines of M)
    if(!(cin>>N>>M>>K)) return 0;
    set<int> ls;
    for(int i=0;i<K;i++){ int x; cin>>x; ls.insert(x); }
    int mx = max(N,M);
    for(int x : ls){ if(x<=mx) lens.push_back(x); }
    // sort lengths maybe larger first for better pruning
    sort(lens.begin(), lens.end(), greater<int>());

    int total=N*M;
    cellBrick.assign(total,-1);
    filledArr.assign(total,0);
    nextId=0;

    if(!solve()){
        cout<<"NO\n";
        return 0;
    }

    // coloring
    int B = nextId;
    vector<set<int>> adj(B);
    for(int r=0;r<N;r++){
        for(int c=0;c<M;c++){
            int id = cellBrick[r*M+c];
            // right neighbor
            if(c+1<M){
                int o = cellBrick[r*M+c+1];
                if(o!=id){ adj[id].insert(o); adj[o].insert(id); }
            }
            if(r+1<N){
                int o = cellBrick[(r+1)*M+c];
                if(o!=id){ adj[id].insert(o); adj[o].insert(id); }
            }
        }
    }
    // degeneracy ordering
    vector<int> deg(B);
    for(int i=0;i<B;i++) deg[i]=adj[i].size();
    vector<char> removed(B,0);
    vector<int> order;
    order.reserve(B);
    for(int it=0;it<B;it++){
        int best=-1;
        for(int i=0;i<B;i++){
            if(!removed[i]){
                if(best==-1 || deg[i]<deg[best]) best=i;
            }
        }
        removed[best]=1;
        order.push_back(best);
        for(int nb: adj[best]) if(!removed[nb]) deg[nb]--;
    }
    reverse(order.begin(), order.end());
    vector<int> color(B,-1);
    for(int v: order){
        set<int> used;
        for(int nb: adj[v]) if(color[nb]!=-1) used.insert(color[nb]);
        int col=0;
        while(used.count(col)) col++;
        color[v]=col;
    }

    cout<<"YES\n";
    for(int r=0;r<N;r++){
        string line(M,' ');
        for(int c=0;c<M;c++){
            int id=cellBrick[r*M+c];
            line[c]=(char)('a'+color[id]);
        }
        cout<<line<<"\n";
    }
    return 0;
}
