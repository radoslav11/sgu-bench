#include<bits/stdc++.h>
using namespace std;

int n;

bool canWin(vector<int> state) {
    vector<int> goal(n + 1);
    for(int i = 0; i < n; i++) goal[i] = i + 1;
    goal[n] = 0;
    
    if(state == goal) return true;
    
    queue<vector<int>> q;
    set<vector<int>> visited;
    q.push(state);
    visited.insert(state);
    
    while(!q.empty()) {
        vector<int> curr = q.front();
        q.pop();
        
        if(curr == goal) return true;
        
        int emptyPos = -1;
        for(int i = 0; i < n; i++) {
            if(curr[i] == 0) {
                emptyPos = i;
                break;
            }
        }
        
        if(emptyPos > 0 && curr[emptyPos - 1] > 0 && curr[emptyPos - 1] < n) {
            for(int i = 0; i < n; i++) {
                if(curr[i] == curr[emptyPos - 1] + 1) {
                    vector<int> next = curr;
                    next[emptyPos] = curr[emptyPos - 1] + 1;
                    next[i] = 0;
                    if(!visited.count(next)) {
                        visited.insert(next);
                        q.push(next);
                    }
                    break;
                }
            }
        }
    }
    
    return false;
}

int main(){
    cin >> n;
    
    vector<int> perm;
    for(int i = 1; i <= n; i++) perm.push_back(i);
    perm.push_back(0);
    
    int count = 0;
    
    do {
        if(canWin(perm)) count++;
    } while(next_permutation(perm.begin(), perm.end()));
    
    cout << count << endl;
    
    return 0;
}
