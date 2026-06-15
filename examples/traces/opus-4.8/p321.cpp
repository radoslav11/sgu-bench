#include <bits/stdc++.h>
using namespace std;

int main(){
    int N;
    scanf("%d",&N);
    vector<int> parent(N+1,0), idx(N+1,0), depth(N+1,0);
    vector<char> almost(N+1,0); // 1 if almost protected
    vector<vector<int>> children(N+1);
    for(int i=1;i<=N-1;i++){
        int a,b;
        char buf[64];
        scanf("%d %d",&a,&b);
        scanf("%s",buf);
        bool isAlmost;
        if(strcmp(buf,"almost")==0){
            scanf("%s",buf); // "protected"
            isAlmost=true;
        } else {
            isAlmost=false; // "protected"
        }
        // edge from a to b, parent[a]=b
        parent[a]=b;
        idx[a]=i;
        almost[a]= isAlmost?1:0;
    }
    // compute depths via BFS from root 1
    for(int v=2;v<=N;v++){
        children[parent[v]].push_back(v);
    }
    // depth via BFS
    {
        // BFS
        vector<int> q; q.reserve(N);
        q.push_back(1); depth[1]=0;
        for(size_t h=0;h<q.size();h++){
            int u=q[h];
            for(int c:children[u]){ depth[c]=depth[u]+1; q.push_back(c);}
        }
    }
    
    vector<char> flipped(N+1,0);
    vector<int> ans;
    set<pair<int,int>> S; // (depth, node) for almost && not flipped on current path
    
    long long sum=0;
    
    // val function inline
    // iterative DFS
    // stack of (node, childIndex)
    vector<pair<int,int>> st;
    st.reserve(N+1);
    
    auto valOf=[&](int v)->int{
        if(!almost[v] || flipped[v]) return 1; else return -1;
    };
    
    auto entry=[&](int v){
        if(v!=1){
            int val=valOf(v);
            sum+=val;
            if(almost[v] && !flipped[v]){
                S.insert({depth[v],v});
            }
        }
        while(sum<0){
            // pick min depth almost edge
            auto it=S.begin();
            // must exist
            int u=it->second;
            S.erase(it);
            flipped[u]=1;
            sum+=2;
            ans.push_back(idx[u]);
        }
    };
    auto exitNode=[&](int v){
        if(v!=1){
            int val=valOf(v);
            sum-=val;
            // remove from set if present (only if almost && not flipped)
            if(almost[v] && !flipped[v]){
                S.erase({depth[v],v});
            }
        }
    };
    
    st.push_back({1,0});
    entry(1);
    while(!st.empty()){
        auto &top=st.back();
        int v=top.first;
        if(top.second < (int)children[v].size()){
            int c=children[v][top.second];
            top.second++;
            st.push_back({c,0});
            entry(c);
        } else {
            exitNode(v);
            st.pop_back();
        }
    }
    
    printf("%d\n",(int)ans.size());
    for(size_t i=0;i<ans.size();i++){
        printf("%d%c", ans[i], (i+1<ans.size())?' ':'\n');
    }
    if(ans.empty()) printf("\n");
    return 0;
}
