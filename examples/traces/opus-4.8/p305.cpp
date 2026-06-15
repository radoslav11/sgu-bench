#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll cdiv(ll p, ll q){ // q>0
    if(p>=0) return (p+q-1)/q;
    else return -((-p)/q);
}
ll fdiv(ll p, ll q){ // q>0
    if(p>=0) return p/q;
    else return -(((-p)+q-1)/q);
}

int N;
ll M;
vector<vector<int>> adj; // progression -> list of value ids
vector<int> matchVal; // value id -> progression
vector<int> matchProg; // progression -> value id
vector<char> used;

bool tryKuhn(int v){
    for(int to : adj[v]){
        if(!used[to]){
            used[to]=1;
            if(matchVal[to]==-1 || tryKuhn(matchVal[to])){
                matchVal[to]=v;
                matchProg[v]=to;
                return true;
            }
        }
    }
    return false;
}

int main(){
    scanf("%d %lld", &N, &M);
    vector<ll> As(N), Bs(N);
    vector<vector<ll>> cands(N);
    map<ll,int> valId;
    vector<ll> idVal;
    
    for(int i=0;i<N;i++){
        ll A,B;
        scanf("%lld %lld",&A,&B);
        As[i]=A; Bs[i]=B;
        vector<ll> cand;
        if(A==0){
            if(B>=1 && B<=M) cand.push_back(B);
        } else if(A>0){
            ll Kmin = max(0LL, cdiv(1-B, A));
            ll Kmax = fdiv(M-B, A);
            for(ll k=Kmin; k<=Kmax && (int)cand.size()<N; k++) cand.push_back(A*k+B);
        } else {
            ll a=-A;
            ll Kmin = max(0LL, cdiv(B-M, a));
            ll Kmax = fdiv(B-1, a);
            for(ll k=Kmax; k>=Kmin && (int)cand.size()<N; k--) cand.push_back(B - a*k);
        }
        cands[i]=cand;
    }
    
    // assign ids
    for(int i=0;i<N;i++){
        for(ll v : cands[i]){
            if(valId.find(v)==valId.end()){
                int id = idVal.size();
                valId[v]=id;
                idVal.push_back(v);
            }
        }
    }
    
    int V = idVal.size();
    adj.assign(N, {});
    for(int i=0;i<N;i++){
        for(ll v : cands[i]){
            adj[i].push_back(valId[v]);
        }
        // dedup
        sort(adj[i].begin(), adj[i].end());
        adj[i].erase(unique(adj[i].begin(), adj[i].end()), adj[i].end());
    }
    
    matchVal.assign(V, -1);
    matchProg.assign(N, -1);
    
    for(int i=0;i<N;i++){
        used.assign(V, 0);
        tryKuhn(i);
    }
    
    // results
    vector<ll> result(N, -1);
    set<ll> usedNums;
    for(int i=0;i<N;i++){
        if(matchProg[i]!=-1){
            result[i]=idVal[matchProg[i]];
            usedNums.insert(result[i]);
        }
    }
    // assign leftover numbers to unmatched
    ll counter=1;
    for(int i=0;i<N;i++){
        if(result[i]==-1){
            while(usedNums.count(counter)) counter++;
            result[i]=counter;
            usedNums.insert(counter);
            counter++;
        }
    }
    
    for(int i=0;i<N;i++){
        printf("%lld", result[i]);
        if(i+1<N) printf(" ");
    }
    printf("\n");
    return 0;
}
