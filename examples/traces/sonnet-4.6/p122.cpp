#include<bits/stdc++.h>
using namespace std;
int n;
vector<set<int>>adj;
vector<int>path;
vector<bool>inPath;
bool tryExtend(){int b=path.back();for(int v:adj[b])if(!inPath[v]){path.push_back(v);inPath[v]=true;return true;}return false;}
int main(){
    cin>>n;adj.resize(n+1);inPath.resize(n+1,false);
    for(int i=1;i<=n;i++){string l;getline(cin>>ws,l);istringstream ss(l);int x;while(ss>>x){adj[i].insert(x);adj[x].insert(i);}}
    path.push_back(1);inPath[1]=true;
    while(tryExtend());
    for(int iter=0;iter<n*n*4&&(int)path.size()<n;iter++){
        int bk=path.back();bool done=false;
        for(int i=0;i+2<(int)path.size();i++){if(adj[bk].count(path[i])){reverse(path.begin()+i+1,path.end());done=true;break;}}
        if(done){while(tryExtend());}
        if((int)path.size()<n){int fr=path[0],bk2=path.back();if(adj[fr].count(bk2)){for(int i=0;i<(int)path.size();i++){bool found=false;for(int v:adj[path[i]])if(!inPath[v]){rotate(path.begin(),path.begin()+i+1,path.end());path.push_back(v);inPath[v]=true;while(tryExtend());found=true;break;}if(found)break;}}}
    }
    if((int)path.size()==n&&adj[path[0]].count(path.back())){auto it=find(path.begin(),path.end(),1);rotate(path.begin(),it,path.end());for(int x:path)cout<<x<<" ";cout<<1<<"\n";}
    else cout<<"No solution"<<"\n";
}
