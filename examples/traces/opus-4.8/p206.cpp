#include <bits/stdc++.h>
using namespace std;
int n,m;
int main(){
    scanf("%d %d",&n,&m);
    vector<int> A(m),B(m),C(m);
    for(int i=0;i<m;i++) scanf("%d %d %d",&A[i],&B[i],&C[i]);
    // tree edges 0..n-2
    // for non-tree edges find path tree edges
    vector<vector<int>> adj(n+1);
    for(int i=0;i<n-1;i++){adj[A[i]].push_back(i);adj[B[i]].push_back(i);}
    auto pathEdges=[&](int s,int t){
        vector<int> par(n+1,-1), pe(n+1,-1); 
        queue<int>q;q.push(s);par[s]=s;
        while(!q.empty()){int u=q.front();q.pop();for(int id:adj[u]){int v=A[id]==u?B[id]:A[id];if(par[v]==-1){par[v]=u;pe[v]=id;q.push(v);}}}
        vector<int>res;int cur=t;while(cur!=s){res.push_back(pe[cur]);cur=par[cur];}return res;
    };
    vector<int> D=C;
    // constraints: for non-tree edge e, each tree edge f on path: d_f<=d_e
    // iterative: this is small, use LP via repeated relaxation (Bellman-Ford style won't minimize abs)
    // Use parametric: we'll just do simple feasibility minimization via tree-edge lowering decision.
    // Approach: binary independent? Not exactly. Use min cost flow.
    // Simpler: For each tree edge, decide threshold. Use the model below.
    // We'll just output original adjusting greedily isn't optimal in general... 
    // Implement proper min cost flow:
    // Variables: lower tree edges down, lower non-tree edges down.
    // Constraint d_f<=d_e. Cost = sum decreases.
    // This LP's optimum: min cost flow. Build: for each pair (f,e), need d_f<=d_e.
    // Equivalent threshold problem solvable independently per "level". 
    // Use successive: for value v from high to low, this is a min-cut. 
    // Total sum |c-d| = integral over v of (cut). For each threshold v, define x_i = [d_i>=v]? 
    // d_f<=d_e means if d_f>=v then d_e>=v: monotone -> min cut per level.
    int maxc=10001;
    vector<long long> dec(m,0); // amount each variable lowered (we track final d)
    vector<long long> finalD(m,0);
    // For each level v from 1..maxc, decide for each edge whether d>=v (1) or <v (0).
    // base cost: if original c>=v, keeping 1 free; if 0 pay (lowering). if c<v keeping0 free; raising pay - but no raise. so c<v ->0 forced(can't be 1 cheaply? raising costs). Actually d can't exceed... can raise tree? not beneficial. set: var can be lowered only => d<=c. So d>=v possible only if c>=v.
    // closure: x_f=1 implies x_e=1. cost of x_f=0 when c_f>=v is 1. So minimize.
    // min cut: source->f? We want choose set S (x=1). For tree f with c_f>=v cost1 if not chosen. nontree e with c_e>=v: choosing 1 free. but constraint x_f<=x_e.
    return 0;
}
