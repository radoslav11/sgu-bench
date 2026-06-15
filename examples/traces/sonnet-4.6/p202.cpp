#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;
const ull INF = ULLONG_MAX/2;
ull dp[65][66];
bool computed[65][66];
int N,M;
stack<int> pegs[66];

ull T(int n,int m){
    if(n==0)return 0;
    if(m<=1)return INF;
    if(m==2)return n==0?0:INF;
    if(m==3)return n<64?(1ULL<<n)-1:INF;
    if(computed[n][m])return dp[n][m];
    computed[n][m]=true;
    ull best=INF;
    for(int k=1;k<n;k++){
        ull a=T(k,m),b=T(n-k,m-1);
        if(a==INF||b==INF)continue;
        if(a>(INF-b)/2)continue;
        ull v=2*a+b;
        if(v<best)best=v;
    }
    return dp[n][m]=best;
}

void do_move(int disc,int src,int dst){
    pegs[src].pop();
    if(pegs[dst].empty())printf("move %d from %d to %d\n",disc,src,dst);
    else printf("move %d from %d to %d atop %d\n",disc,src,dst,pegs[dst].top());
    pegs[dst].push(disc);
}

// solve: move top `discs` vector (discs[0]=largest, discs.back()=smallest) from src to dst
// using extra[] as intermediate pegs
// total pegs available = extra.size() + 2
void solve(vector<int>&discs, int src, int dst, vector<int> extra){
    int n=discs.size();
    if(n==0)return;
    int m=extra.size()+2;
    if(m==3){
        // classic hanoi with one extra peg
        int aux=extra[0];
        // move n-1 from src to aux, move discs[0] from src to dst, move n-1 from aux to dst
        // but recursively
        vector<int> sub(discs.begin()+1,discs.end());
        vector<int> e2={dst}; // aux for sub-problem: src, dst=aux, extra=dst
        // hanoi3: move sub from src to aux using {dst} as extra
        // wait m=3 means extra has 1 element
        solve(sub,src,aux,{dst});
        do_move(discs[0],src,dst);
        solve(sub,aux,dst,{src});
        return;
    }
    // find optimal k
    int best_k=1;
    ull best_val=INF;
    for(int k=1;k<n;k++){
        ull a=T(k,m),b=T(n-k,m-1);
        if(a==INF||b==INF)continue;
        if(a>(INF-b)/2)continue;
        ull v=2*a+b;
        if(v<best_val){best_val=v;best_k=k;}
    }
    int k=best_k;
    // move top k discs from src to some intermediate peg p, using all m pegs
    // then move bottom n-k discs from src to dst using m-1 pegs (without p)
    // then move k discs from p to dst using all m pegs
    int p=extra[0]; // pick first extra peg as the staging peg
    vector<int> extra2(extra.begin()+1,extra.end()); // extra without p
    // extra2 now has m-3 pegs, total for second step: extra2+src+dst = m-1 pegs
    
    vector<int> top_k(discs.begin()+discs.size()-k, discs.end()); // smallest k
    vector<int> bot(discs.begin(), discs.begin()+discs.size()-k); // largest n-k
    
    // step1: move top_k from src to p, using all m pegs
    // extra for this: extra without p, plus dst
    vector<int> extra_step1 = extra2;
    extra_step1.push_back(dst);
    solve(top_k, src, p, extra_step1);
    
    // step2: move bot from src to dst, using m-1 pegs (extra2 + src + dst, no p)
    solve(bot, src, dst, extra2);
    
    // step3: move top_k from p to dst, using all m pegs
    vector<int> extra_step3 = extra2;
    extra_step3.push_back(src);
    solve(top_k, p, dst, extra_step3);
}

int main(){
    scanf("%d%d",&N,&M);
    memset(computed,0,sizeof(computed));
    
    // initialize pegs
    for(int i=1;i<=N;i++) pegs[1].push(N+1-i); // disc N at bottom... wait
    // pegs[1]: bottom is largest (N), top is smallest (1)
    // but stack top should be 1
    // Actually push in order N, N-1, ..., 1 so top=1
    while(!pegs[1].empty())pegs[1].pop();
    for(int i=N;i>=1;i--) pegs[1].push(i); // top=1, bottom=N... wait
    // push N first then N-1 ... 1, so top=1
    // Hmm: push(N), push(N-1), ..., push(1) -> top=1. Yes.
    
    ull ans=T(N,M);
    printf("%llu\n",ans);
    
    // discs vector: discs[0]=largest=N, discs[N-1]=smallest=1
    vector<int> discs;
    for(int i=N;i>=1;i--) discs.push_back(i); // discs[0]=N, discs[N-1]=1
    
    vector<int> extra;
    for(int i=2;i<M;i++) extra.push_back(i);
    
    solve(discs, 1, M, extra);
    
    return 0;
}
