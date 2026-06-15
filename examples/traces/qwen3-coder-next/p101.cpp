#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

struct Domino {
    int a, b;
    int index;
};

int n;
vector<Domino> dominoes;
vector<bool> used;
vector<pair<int, char>> result;

bool dfs(int pos, int lastRight) {
    if (pos == n) {
        return true;
    }
    
    for (int i = 0; i < n; i++) {
        if (used[i]) continue;
        
        // Try without rotation: domino[i].a matches lastRight
        if (dominoes[i].a == lastRight) {
            used[i] = true;
            result.push_back({dominoes[i].index, '+'});
            if (dfs(pos + 1, dominoes[i].b)) {
                return true;
            }
            result.pop_back();
            used[i] = false;
        }
        
        // Try with rotation: domino[i].b matches lastRight
        if (dominoes[i].b == lastRight) {
            used[i] = true;
            result.push_back({dominoes[i].index, '-'});
            if (dfs(pos + 1, dominoes[i].a)) {
                return true;
            }
            result.pop_back();
            used[i] = false;
        }
    }
    
    return false;
}

int main() {
    cin >> n;
    dominoes.resize(n);
    used.assign(n, false);
    
    for (int i = 0; i < n; i++) {
        cin >> dominoes[i].a >> dominoes[i].b;
        dominoes[i].index = i + 1;
    }
    
    // Try each domino as the starting piece
    for (int start = 0; start < n; start++) {
        // Try starting without rotation
        used.assign(n, false);
        used[start] = true;
        result.clear();
        result.push_back({dominoes[start].index, '+'});
        if (dfs(1, dominoes[start].b)) {
            for (auto& r : result) {
                cout << r.first << " " << r.second << endl;
            }
            return 0;
        }
        
        // Try starting with rotation
        used.assign(n, false);
        used[start] = true;
        result.clear();
        result.push_back({dominoes[start].index, '-'});
        if (dfs(1, dominoes[start].a)) {
            for (auto& r : result) {
                cout << r.first << " " << r.second << endl;
            }
            return 0;
        }
    }
    
    cout << "No solution" << endl;
    return 0;
}
