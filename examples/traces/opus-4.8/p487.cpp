#include <bits/stdc++.h>
using namespace std;

int town(int g){ return (g-1)/4 + 1; }

int main(){
    int N;
    scanf("%d",&N);
    int M = 2*N; // roads
    struct DE { int to, gateFrom, gateTo, roadId; };
    vector<DE> de;
    de.reserve(2*M);
    vector<vector<int>> adj(N+1);
    vector<char> used(M, 0);
    for(int i=0;i<M;i++){
        int a,b;
        scanf("%d %d",&a,&b);
        int ta = town(a), tb = town(b);
        // directed from ta
        int idx1 = de.size();
        de.push_back({tb, a, b, i});
        adj[ta].push_back(idx1);
        int idx2 = de.size();
        de.push_back({ta, b, a, i});
        adj[tb].push_back(idx2);
    }
    
    // Hierholzer from town 1
    vector<int> ptr(N+1, 0);
    vector<pair<int,int>> st; // vertex, arrivedEdge
    st.push_back({1, -1});
    vector<int> circ;
    while(!st.empty()){
        int v = st.back().first;
        while(ptr[v] < (int)adj[v].size() && used[de[adj[v][ptr[v]]].roadId]) ptr[v]++;
        if(ptr[v] < (int)adj[v].size()){
            int eid = adj[v][ptr[v]++];
            used[de[eid].roadId] = 1;
            st.push_back({de[eid].to, eid});
        } else {
            circ.push_back(st.back().second);
            st.pop_back();
        }
    }
    
    reverse(circ.begin(), circ.end());
    // circ[0] should be -1
    // remaining are directed edges in tour order
    int edgeCount = (int)circ.size() - 1;
    if(edgeCount != M){
        // disconnected
        printf("No\n");
        return 0;
    }
    
    printf("Yes\n");
    // build route
    bool first = true;
    for(int i=1;i<(int)circ.size();i++){
        int eid = circ[i];
        if(!first) printf(" ");
        first=false;
        printf("%d %d", de[eid].gateFrom, de[eid].gateTo);
    }
    printf("\n");
    return 0;
}
