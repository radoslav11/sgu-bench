#include<bits/stdc++.h>
using namespace std;

int n;
vector<char> triangles;
vector<vector<int>> adj;
vector<bool> covered;
int answer = 0;

void solve(int idx) {
    while(idx < triangles.size() && (covered[idx] || triangles[idx] == '*')) {
        idx++;
    }
    
    if(idx == triangles.size()) {
        answer++;
        return;
    }
    
    for(int neighbor : adj[idx]) {
        if(!covered[neighbor] && triangles[neighbor] == '.') {
            covered[idx] = true;
            covered[neighbor] = true;
            solve(idx + 1);
            covered[idx] = false;
            covered[neighbor] = false;
        }
    }
}

int main(){
    cin >> n;
    
    for(int layer = 0; layer < 2*n; layer++) {
        int size = min(8*n-4, 4 + 8*layer);
        string s;
        cin >> s;
        for(char c : s) triangles.push_back(c);
    }
    
    adj.resize(triangles.size());
    for(int i = 0; i < triangles.size(); i++) {
        if(i > 0) adj[i].push_back(i-1);
        if(i < triangles.size()-1) adj[i].push_back(i+1);
    }
    
    covered.resize(triangles.size(), false);
    solve(0);
    cout << answer << endl;
    
    return 0;
}
