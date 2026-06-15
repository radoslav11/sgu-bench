#include <bits/stdc++.h>
using namespace std;
int N;
vector<int> adj[405];
int matchSon[405],matchGirl[405];
bool visited[405];
bool augment(int son){
    for(int girl:adj[son]){
        if(!visited[girl]){
            visited[girl]=true;
            if(matchGirl[girl]==0||augment(matchGirl[girl])){
                matchSon[son]=girl;matchGirl[girl]=son;return true;
            }
        }
    }
    return false;
}
int main(){
    ios_base::sync_with_stdio(false);cin.tie(NULL);
    cin>>N;
    vector<int> A(N+1);
    for(int i=1;i<=N;i++) cin>>A[i];
    for(int i=1;i<=N;i++){int k;cin>>k;for(int j=0;j<k;j++){int g;cin>>g;adj[i].push_back(g);}}
    vector<int> order(N);iota(order.begin(),order.end(),1);
    sort(order.begin(),order.end(),[&](int a,int b){return A[a]>A[b];});
    memset(matchSon,0,sizeof(matchSon));memset(matchGirl,0,sizeof(matchGirl));
    for(int son:order){memset(visited,false,sizeof(visited));augment(son);}
    for(int i=1;i<=N;i++){cout<<matchSon[i];if(i<N)cout<<" ";}cout<<endl;
}
