#include <bits/stdc++.h>
using namespace std;

int N;
vector<vector<char>> adj;

int main(){
    scanf("%d", &N);
    adj.assign(N+1, vector<char>(N+1, 0));
    cin.ignore();
    for(int i=1;i<=N;i++){
        string line;
        getline(cin, line);
        istringstream iss(line);
        int x;
        while(iss>>x){
            adj[i][x]=1;
            adj[x][i]=1;
        }
    }
    
    vector<int> cyc(N);
    for(int i=0;i<N;i++) cyc[i]=i+1;
    
    auto adjc=[&](int a,int b){return adj[a][b];};
    
    while(true){
        bool changed=false;
        for(int i=0;i<N;i++){
            int a=cyc[i], b=cyc[(i+1)%N];
            if(adjc(a,b)) continue;
            bool found=false;
            for(int k=0;k<N;k++){
                if(k==i) continue;
                int c=cyc[k], d=cyc[(k+1)%N];
                if(adjc(a,c) && adjc(b,d)){
                    int l=(i+1)%N;
                    int r=k;
                    int len = (r-l+N)%N +1;
                    for(int t=0;t<len/2;t++){
                        int p1=(l+t)%N;
                        int p2=(r-t+N)%N;
                        swap(cyc[p1],cyc[p2]);
                    }
                    found=true;
                    changed=true;
                    break;
                }
            }
            if(found) break;
        }
        if(!changed){
            bool ok=true;
            for(int i=0;i<N;i++){
                if(!adjc(cyc[i],cyc[(i+1)%N])){ok=false;break;}
            }
            if(ok) break;
            else { printf("No solution\n"); return 0; }
        }
    }
    
    for(int i=0;i<N;i++){
        if(!adjc(cyc[i],cyc[(i+1)%N])){
            printf("No solution\n"); return 0;
        }
    }
    
    int pos=0;
    for(int i=0;i<N;i++) if(cyc[i]==1){pos=i;break;}
    vector<int> res;
    for(int i=0;i<N;i++) res.push_back(cyc[(pos+i)%N]);
    res.push_back(1);
    for(int i=0;i<(int)res.size();i++){
        printf("%d%c", res[i], i+1<(int)res.size()?' ':'\n');
    }
    return 0;
}
