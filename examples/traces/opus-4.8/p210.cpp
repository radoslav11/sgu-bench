#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> adj[405];
int matchGirl[405];
bool used[405];

bool tryKuhn(int son){
    for(int g : adj[son]){
        if(!used[g]){
            used[g]=true;
            if(matchGirl[g]==-1 || tryKuhn(matchGirl[g])){
                matchGirl[g]=son;
                return true;
            }
        }
    }
    return false;
}

int main(){
    scanf("%d",&N);
    vector<int> A(N+1);
    for(int i=1;i<=N;i++) scanf("%d",&A[i]);
    for(int i=1;i<=N;i++){
        int k; scanf("%d",&k);
        for(int j=0;j<k;j++){
            int g; scanf("%d",&g);
            adj[i].push_back(g);
        }
    }
    vector<int> order(N);
    for(int i=0;i<N;i++) order[i]=i+1;
    sort(order.begin(),order.end(),[&](int a,int b){
        return A[a]>A[b];
    });
    
    for(int g=1;g<=N;g++) matchGirl[g]=-1;
    vector<int> result(N+1,0);
    
    for(int son : order){
        for(int g=1;g<=N;g++) used[g]=false;
        tryKuhn(son);
    }
    
    for(int g=1;g<=N;g++){
        if(matchGirl[g]!=-1) result[matchGirl[g]]=g;
    }
    
    for(int i=1;i<=N;i++){
        printf("%d", result[i]);
        if(i<N) printf(" ");
    }
    printf("\n");
    return 0;
}
