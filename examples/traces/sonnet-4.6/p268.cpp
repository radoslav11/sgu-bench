#include <bits/stdc++.h>
using namespace std;
int main(){
    int N; cin>>N;
    string S1,S2; cin>>S1>>S2;
    // BFS: find shortest hyper-almost-permutative string containing S1 and S2
    // State: (last N-1 chars as string, found_S1, found_S2)
    // Start states: begin with S1 or S2 or any perm
    // Transitions: append a permutation overlapping by N-1
    map<tuple<string,int,int>,tuple<string,int,int,string>> prev;
    queue<tuple<string,int,int>> q;
    auto enqueue=[&](string node,int f1,int f2,string par_node,int pf1,int pf2,string edge){
        auto key=make_tuple(node,f1,f2);
        if(prev.count(key)) return;
        prev[key]=make_tuple(par_node,pf1,pf2,edge);
        q.push(key);
    };
    // Generate all permutations of first N letters
    string base=""; for(int i=0;i<N;i++) base+=(char)('A'+i);
    vector<string> perms;
    string p=base; sort(p.begin(),p.end());
    do{ perms.push_back(p); }while(next_permutation(p.begin(),p.end()));
    // Initial: start with each permutation
    for(auto&pp:perms){
        int f1=(pp==S1)?1:0, f2=(pp==S2)?1:0;
        string node=pp.substr(1);
        auto key=make_tuple(node,f1,f2);
        if(!prev.count(key)){
            prev[key]=make_tuple(string(""),- 1,-1,pp);
            q.push(key);
        }
    }
    tuple<string,int,int> goal;
    bool found=false;
    while(!q.empty()&&!found){
        auto cur=q.front();q.pop();
        auto[node,f1,f2]=cur;
        if(f1&&f2){goal=cur;found=true;break;}
        // transitions
        for(auto&pp:perms){
            if(pp.substr(0,N-1)==node){
                int nf1=f1||(pp==S1), nf2=f2||(pp==S2);
                string nnode=pp.substr(1);
                auto key=make_tuple(nnode,nf1,nf2);
                if(!prev.count(key)){
                    prev[key]=make_tuple(node,f1,f2,pp);
                    q.push(key);
                }
            }
        }
    }
    if(!found){// check queue leftovers
        for(auto&[k,v]:prev){auto[n,f1,f2]=k;if(f1&&f2){goal=k;found=true;break;}}}
    // reconstruct
    string result="";
    auto cur=goal;
    while(true){
        auto[node,f1,f2]=cur;
        auto[pnode,pf1,pf2,edge]=prev[cur];
        if(pf1==-1){result=edge+result;break;}
        result=edge.back()+result;
        cur=make_tuple(pnode,pf1,pf2);
    }
    cout<<result<<endl;
}
