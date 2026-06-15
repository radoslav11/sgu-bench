#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,M;
    cin>>N>>M;
    vector<vector<int>> col(N,vector<int>(N));
    for(int i=0;i<N;i++) for(int j=0;j<N;j++) cin>>col[i][j];
    int budget=(M+1)/2;
    // Try each vertex v as center: pick all colors on edges from v
    for(int v=0;v<N;v++){
        set<int> chosen;
        for(int u=0;u<N;u++) if(u!=v) chosen.insert(col[v][u]);
        if((int)chosen.size()<=budget){
            // diameter <= 2, done
            cout<<chosen.size()<<"\n";
            for(int c:chosen) cout<<c<<" ";
            cout<<"\n";
            return 0;
        }
    }
    cout<<-1<<"\n";
}
