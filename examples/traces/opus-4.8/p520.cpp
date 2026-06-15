#include <bits/stdc++.h>
using namespace std;

int n, m;
vector<int> adj[1005];
int dist_[1005];
// memo[u][d], d from 1..n+1
vector<vector<int>> memo;

int win(int u, int d) {
    if (d > n + 1) return 0; // no safe move possible
    int &r = memo[u][d];
    if (r != -1) return r;
    r = 0; // assume lose
    for (int w : adj[u]) {
        if (dist_[w] >= d) {
            // safe move
            if (win(w, d + 1) == 0) {
                r = 1;
                break;
            }
        }
    }
    return r;
}

int main(){
    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; i++){
        int a,b; scanf("%d %d",&a,&b);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    // BFS from 1
    for (int i = 1; i <= n; i++) dist_[i] = -1;
    queue<int> q;
    dist_[1] = 0; q.push(1);
    while(!q.empty()){
        int u = q.front(); q.pop();
        for (int w : adj[u]) if (dist_[w] == -1){ dist_[w] = dist_[u]+1; q.push(w);}
    }
    memo.assign(n+1, vector<int>(n+2, -1));
    int res = win(1, 1);
    // res==1 means Nikolay avoids being destroyed -> Vladimir pays
    if (res == 1) printf("Vladimir\n");
    else printf("Nikolay\n");
    return 0;
}
