#include<bits/stdc++.h>
using namespace std;

const double EPS = 1e-6;

struct Point {
    double x, y, z;
    string original;
    
    bool operator==(const Point& p) const {
        return abs(x - p.x) < EPS && abs(y - p.y) < EPS && abs(z - p.z) < EPS;
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int f;
    cin >> f;
    cin.ignore();
    
    map<tuple<double,double,double>, int> pointMap;
    vector<Point> vertices;
    set<pair<int,int>> edges;
    
    for(int i = 0; i < f; i++){
        int k;
        cin >> k;
        cin.ignore();
        
        vector<int> faceVertices;
        for(int j = 0; j < k; j++){
            string line;
            getline(cin, line);
            double x, y, z;
            sscanf(line.c_str(), "%lf %lf %lf", &x, &y, &z);
            
            auto key = make_tuple(x, y, z);
            if(pointMap.find(key) == pointMap.end()){
                pointMap[key] = vertices.size();
                Point p;
                p.x = x;
                p.y = y;
                p.z = z;
                p.original = line;
                vertices.push_back(p);
            }
            faceVertices.push_back(pointMap[key]);
        }
        
        // Add edges from this face
        for(int j = 0; j < k; j++){
            int u = faceVertices[j];
            int v = faceVertices[(j+1)%k];
            if(u > v) swap(u, v);
            edges.insert({u, v});
        }
    }
    
    int n = vertices.size();
    vector<vector<int>> adj(n);
    
    for(auto [u, v] : edges){
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    // Try to find Hamiltonian cycle using DFS
    vector<int> path;
    vector<bool> visited(n, false);
    
    function<bool(int, int)> dfs = [&](int u, int start) -> bool {
        visited[u] = true;
        path.push_back(u);
        
        if(path.size() == n){
            // Check if we can return to start
            for(int v : adj[u]){
                if(v == start){
                    return true;
                }
            }
            visited[u] = false;
            path.pop_back();
            return false;
        }
        
        for(int v : adj[u]){
            if(!visited[v]){
                if(dfs(v, start)){
                    return true;
                }
            }
        }
        
        visited[u] = false;
        path.pop_back();
        return false;
    };
    
    // Try starting from vertex 0
    if(dfs(0, 0)){
        cout << "Yes\n";
        cout << n << "\n";
        for(int idx : path){
            cout << vertices[idx].original << "\n";
        }
    } else {
        cout << "No\n";
    }
    
    return 0;
}
