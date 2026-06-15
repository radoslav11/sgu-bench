#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long tx, ty;
    int d2, d1, b;
    cin >> tx >> ty >> d2 >> d1 >> b;
    
    vector<pair<long long, long long>> dirs = {
        {1, 0}, {1, -1}, {0, -1}, {-1, 0}, {-1, 1}, {0, 1}
    };
    
    map<tuple<long long, long long, int, int>, int> visited;
    queue<tuple<long long, long long, int, int, int>> q;
    
    q.push({0, 0, d1, 0, 0});
    visited[{0, 0, d1, 0}] = 0;
    
    while(!q.empty()){
        auto [x, y, dir, cons, turns] = q.front();
        q.pop();
        
        if(x == tx && y == ty && dir == d2){
            cout << turns << endl;
            return 0;
        }
        
        auto key = make_tuple(x, y, dir, cons);
        if(visited.count(key) && visited[key] < turns) continue;
        
        if(cons < b){
            long long nx = x + dirs[dir].first;
            long long ny = y + dirs[dir].second;
            auto nkey = make_tuple(nx, ny, dir, cons + 1);
            if(!visited.count(nkey)){
                visited[nkey] = turns + 1;
                q.push({nx, ny, dir, cons + 1, turns + 1});
            }
        }
        
        int ndir = (dir + 1) % 6;
        long long nx = x + dirs[ndir].first;
        long long ny = y + dirs[ndir].second;
        auto nkey = make_tuple(nx, ny, ndir, 1);
        if(!visited.count(nkey)){
            visited[nkey] = turns + 1;
            q.push({nx, ny, ndir, 1, turns + 1});
        }
    }
    
    return 0;
}
