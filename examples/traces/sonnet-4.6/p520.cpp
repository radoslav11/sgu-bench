#include <bits/stdc++.h>
using namespace std;

int dist1[1001];
vector<int> adj[1001];
int n, m;
// returns true if current pilot (day odd=Nikolay, even=Vladimir) LOSES
map<pair<int,int>, bool> memo;

bool loses(int city, int day) {
    auto key = make_pair(city, day);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;
    
    vector<int> safe_moves;
    for (int v : adj[city]) {
        if (dist1[v] >= day) safe_moves.push_back(v);
    }
    if (safe_moves.empty()) { memo[key] = true; return true; }
    // current pilot wants to NOT lose: pick a move where next pilot loses
    for (int v : safe_moves) {
        if (loses(v, day+1)) { memo[key] = false; return false; }
    }
    memo[key] = true; return true;
}

int main(){
    cin >> n >> m;
    for(int i=0;i<m;i++){int a,b;cin>>a>>b;adj[a].push_back(b);adj[b].push_back(a);}
    fill(dist1,dist1+1001,-1); dist1[1]=0;
    queue<int>q; q.push(1);
    while(!q.empty()){int u=q.front();q.pop();for(int v:adj[u])if(dist1[v]==-1){dist1[v]=dist1[u]+1;q.push(v);}}
    // day 1, robot at city 1, Nikolay moves
    bool nikolay_loses = loses(1, 1);
    cout << (nikolay_loses ? "Nikolay" : "Vladimir") << endl;
}
