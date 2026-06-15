#include<bits/stdc++.h>
using std::cin, std::cout, std::vector;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<vector<int>> adj(N + 1);
    
    for(int i = 0; i < M; i++){
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    
    vector<int> color(N + 1, -1);
    bool is_bipartite = true;
    
    // Try to 2-color the graph
    for(int start = 1; start <= N && is_bipartite; start++){
        if(color[start] == -1){
            queue<int> q;
            q.push(start);
            color[start] = 0;
            
            while(!q.empty() && is_bipartite){
                int u = q.front();
                q.pop();
                
                for(int v : adj[u]){
                    if(color[v] == -1){
                        color[v] = 1 - color[u];
                        q.push(v);
                    } else if(color[v] == color[u]){
                        is_bipartite = false;
                        break;
                    }
                }
            }
        }
    }
    
    if(!is_bipartite){
        cout << "no\n";
    } else {
        cout << "yes\n";
        vector<int> day1, day2;
        
        for(int i = 1; i <= N; i++){
            if(color[i] == 0){
                day1.push_back(i);
            } else {
                day2.push_back(i);
            }
        }
        
        cout << day1.size() << "\n";
        for(int i = 0; i < day1.size(); i++){
            if(i > 0) cout << " ";
            cout << day1[i];
        }
        cout << "\n";
    }
    
    return 0;
}
