#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <cctype>
#include <algorithm>

using namespace std;

int main() {
    int B, R, Y, W;
    cin >> B >> R >> Y >> W;
    
    // For each color, store which pairs can be replaced by that color
    vector<vector<bool>> rules(256, vector<bool>(256, false));
    
    // We'll use a mapping from char to char* for rules
    // rules[pair] = set of colors that can replace this pair
    // Actually, let's use a map: pair -> color set
    set<char> blueRules, redRules, yellowRules, whiteRules;
    
    // Read blue rules
    for (int i = 0; i < B; i++) {
        string rule;
        cin >> rule;
        blueRules.insert(rule);
    }
    
    // Read red rules
    for (int i = 0; i < R; i++) {
        string rule;
        cin >> rule;
        redRules.insert(rule);
    }
    
    // Read yellow rules
    for (int i = 0; i < Y; i++) {
        string rule;
        cin >> rule;
        yellowRules.insert(rule);
    }
    
    // Read white rules
    for (int i = 0; i < W; i++) {
        string rule;
        cin >> rule;
        whiteRules.insert(rule);
    }
    
    string s;
    cin >> s;
    
    int n = s.size();
    
    // dp[l][r] is a set of characters that substring s[l..r] can be reduced to
    vector<vector<set<char>>> dp(n, vector<set<char>>(n));
    
    // Base case: substrings of length 1
    for (int i = 0; i < n; i++) {
        dp[i][i].insert(s[i]);
    }
    
    // Fill DP table for increasing lengths
    for (int len = 2; len <= n; len++) {
        for (int l = 0; l <= n - len; l++) {
            int r = l + len - 1;
            
            // Try all possible splits
            for (int k = l; k < r; k++) {
                // For each character that left part [l..k] can become
                for (char leftChar : dp[l][k]) {
                    // For each character that right part [k+1..r] can become
                    for (char rightChar : dp[k+1][r]) {
                        string pair = string(1, leftChar) + string(1, rightChar);
                        
                        // Check if any rule can replace this pair
                        if (blueRules.count(pair)) {
                            dp[l][r].insert('b');
                        }
                        if (redRules.count(pair)) {
                            dp[l][r].insert('r');
                        }
                        if (yellowRules.count(pair)) {
                            dp[l][r].insert('y');
                        }
                        if (whiteRules.count(pair)) {
                            dp[l][r].insert('w');
                        }
                    }
                }
            }
        }
    }
    
    // Check which colors can be achieved for the whole string
    set<char> result;
    for (char c : {'b', 'r', 'y', 'w'}) {
        if (!dp[0][n-1].count(c)) {
            continue;
        }
        
        // Check if color c is possible (has at least one stone of that color in input)
        // Actually, the problem says players choose different colors, and each color
        // must be chosen by exactly one player. But for "chance to win", we just need
        // that color can be the final result, regardless of how many of that color initially.
        
        // However, the problem states: "the player who chose the color of the stone left is a winner"
        // So if the final stone is color c, then the player who chose color c wins.
        // We need colors for which there exists a sequence leading to that color.
        result.insert(c);
    }
    
    if (result.empty()) {
        cout << "Nobody" << endl;
    } else {
        // Output in any order, but typically sorted
        for (char c : result) {
            cout << c;
        }
        cout << endl;
    }
    
    return 0;
}
