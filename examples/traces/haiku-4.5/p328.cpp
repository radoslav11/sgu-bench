#include<bits/stdc++.h>
using namespace std;

map<tuple<string, int, int>, int> memo;

// Compute Grundy number for a segment
// segment: substring of uncolored (0) and colored (1,2) vertices
// We'll actually work with just the uncolored part and track constraints
int grundy(string s, int left_constraint, int right_constraint) {
    // s contains only '0's (uncolored vertices in this segment)
    // left_constraint, right_constraint: 0=none, 1=black, 2=white
    
    auto key = make_tuple(s, left_constraint, right_constraint);
    if (memo.count(key)) return memo[key];
    
    int n = s.length();
    set<int> reachable;
    
    // Try coloring first uncolored vertex with color 1 or 2
    for (int color = 1; color <= 2; color++) {
        // Check if valid (not same as left constraint)
        if (left_constraint != 0 && left_constraint == color) continue;
        
        // Build new string with first vertex colored
        string next = s;
        next[0] = '0' + color;
        
        // Find segments recursively
        // This is getting complex, let me use a different approach
    }
    
    return memo[key] = 0;
}

int computeGrundy(string s, int left, int right) {
    int n = s.length();
    if (n == 0) return 0;
    
    auto key = make_tuple(s, left, right);
    if (memo.count(key)) return memo[key];
    
    set<int> reachable;
    
    // Try coloring position 0 with color 1 or 2
    for (int c = 1; c <= 2; c++) {
        if (left != 0 && left == c) continue; // Can't use same color as left neighbor
        
        // After coloring position 0 with color c, find next uncolored segment
        int i = 1;
        while (i < n && s[i] != '0') i++;
        
        if (i == n) {
            // No more uncolored vertices
            reachable.insert(0);
        } else {
            // Recursively compute Grundy for remaining segment
            string remaining = s.substr(i);
            int nextGrundy = computeGrundy(remaining, c, right);
            reachable.insert(nextGrundy);
        }
    }
    
    // Compute MEX (minimal excludant)
    int mex = 0;
    while (reachable.count(mex)) mex++;
    
    return memo[key] = mex;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    string s;
    cin >> n >> s;
    
    int xor_sum = 0;
    
    // Find segments of consecutive uncolored vertices
    for (int i = 0; i < n; i++) {
        if (s[i] == '0') {
            int j = i;
            while (j < n && s[j] == '0') j++;
            
            int left = (i > 0) ? (s[i-1] - '0') : 0;
            int right = (j < n) ? (s[j] - '0') : 0;
            
            string segment = s.substr(i, j - i);
            int g = computeGrundy(segment, left, right);
            xor_sum ^= g;
            
            i = j - 1;
        }
    }
    
    if (xor_sum != 0) {
        cout << "FIRST\n";
    } else {
        cout << "SECOND\n";
    }
    
    return 0;
}
