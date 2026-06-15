#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<long long> s(n), d(n);
    for(int i = 0; i < n; i++) cin >> s[i];
    for(int i = 0; i < n; i++) cin >> d[i];
    
    long long sum_s = 0, sum_d = 0;
    for(int i = 0; i < n; i++){
        sum_s += s[i];
        sum_d += d[i];
    }
    
    if(sum_s != sum_d){
        cout << -1 << endl;
        return 0;
    }
    
    if(s == d){
        cout << 0 << endl;
        return 0;
    }
    
    map<vector<long long>, int> dist;
    queue<vector<long long>> q;
    
    q.push(s);
    dist[s] = 0;
    
    while(!q.empty()){
        vector<long long> cur = q.front();
        q.pop();
        
        if(cur == d){
            cout << dist[cur] << endl;
            return 0;
        }
        
        int cur_dist = dist[cur];
        if(cur_dist > 100) continue; // Pruning
        
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(i == j || cur[i] == 0) continue;
                
                vector<long long> next = cur;
                next[j] += next[i];
                next[i] = 0;
                
                if(dist.find(next) == dist.end()){
                    dist[next] = cur_dist + 1;
                    q.push(next);
                }
            }
        }
    }
    
    cout << -1 << endl;
    return 0;
}
