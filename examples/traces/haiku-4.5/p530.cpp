#include<bits/stdc++.h>
using namespace std;

int n, m;
vector<int> adj[100005];
set<int> geniuses = {1, 2, 3};

// Returns 1 if company 1 can force a win, 2 if company 2 can force a win
int solve(set<int> hired1, set<int> hired2, int turn) {
    // Check win conditions
    int g1 = 0, g2 = 0;
    for(int g : geniuses) {
        if(hired1.count(g)) g1++;
        if(hired2.count(g)) g2++;
    }
    
    if(g1 >= 2) return 1;
    if(g2 >= 2) return 2;
    
    set<int> available;
    for(int i = 1; i <= n; i++) {
        if(!hired1.count(i) && !hired2.count(i)) {
            available.insert(i);
        }
    }
    
    if(available.empty()) {
        // Game over, shouldn't reach here if problem is well-defined
        return g1 >= g2 ? 1 : 2;
    }
    
    if(turn == 1) {
        // Company 1's turn
        set<int> canHire;
        
        if(hired1.empty()) {
            // First hire - can pick anyone
            canHire = available;
        } else {
            // Must have a friend already in company 1
            for(int p : available) {
                for(int friend_id : adj[p]) {
                    if(hired1.count(friend_id)) {
                        canHire.insert(p);
                        break;
                    }
                }
            }
        }
        
        if(canHire.empty()) {
            // Company 1 can't hire, pass to company 2
            return solve(hired1, hired2, 2);
        }
        
        // Try to find a move that leads to company 1 winning
        int best = 2; // Assume worst case
        for(int p : canHire) {
            set<int> new_hired1 = hired1;
            new_hired1.insert(p);
            int result = solve(new_hired1, hired2, 2);
            if(result == 1) {
                return 1; // Found winning move
            }
            best = result; // Otherwise take best available
        }
        return best;
    } else {
        // Company 2's turn
        set<int> canHire;
        
        if(hired2.empty()) {
            // First hire - can pick anyone
            canHire = available;
        } else {
            // Must have a friend already in company 2
            for(int p : available) {
                for(int friend_id : adj[p]) {
                    if(hired2.count(friend_id)) {
                        canHire.insert(p);
                        break;
                    }
                }
            }
        }
        
        if(canHire.empty()) {
            // Company 2 can't hire, pass to company 1
            return solve(hired1, hired2, 1);
        }
        
        // Try to find a move that leads to company 2 winning
        int best = 1; // Assume worst case
        for(int p : canHire) {
            set<int> new_hired2 = hired2;
            new_hired2.insert(p);
            int result = solve(hired1, new_hired2, 1);
            if(result == 2) {
                return 2; // Found winning move
            }
            best = result;
        }
        return best;
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n >> m;
    for(int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    
    set<int> hired1, hired2;
    int result = solve(hired1, hired2, 1);
    cout << result << endl;
    
    return 0;
}
