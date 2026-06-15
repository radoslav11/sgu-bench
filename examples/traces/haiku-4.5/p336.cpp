#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    map<int, set<int>> knows;
    
    for(int i = 1; i <= N; i++){
        knows[i] = set<int>();
    }
    
    for(int i = 0; i < M; i++){
        int a, b;
        cin >> a >> b;
        knows[a].insert(b);
    }
    
    int Q;
    cin >> Q;
    
    int nextId = N + 1;
    
    for(int i = 0; i < Q; i++){
        int type;
        cin >> type;
        
        if(type == 1){
            int a, b;
            cin >> a >> b;
            if(knows[a].count(b)){
                cout << "YES\n";
            } else {
                cout << "NO\n";
            }
        } else {
            int a, b;
            cin >> a >> b;
            
            int c = nextId++;
            
            // New block c knows what a and b know
            knows[c] = knows[a];
            for(int x : knows[b]){
                knows[c].insert(x);
            }
            
            // Remove a and b from c's knowledge
            knows[c].erase(a);
            knows[c].erase(b);
            
            // Update all entities that knew about a or b
            for(auto& p : knows){
                if(p.first != a && p.first != b){
                    if(p.second.count(a) || p.second.count(b)){
                        p.second.erase(a);
                        p.second.erase(b);
                        p.second.insert(c);
                    }
                }
            }
        }
    }
    
    return 0;
}
