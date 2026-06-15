#include <iostream>
#include <vector>
#include <string>
using namespace std;

bool matches(const string& pattern, const string& s) {
    int m = pattern.size();
    int n = s.size();
    vector<vector<bool>> dp(m+1, vector<bool>(n+1, false));
    dp[0][0] = true;
    
    for (int i = 0; i < m; i++) {
        for (int j = 0; j <= n; j++) {
            if (!dp[i][j]) continue;
            if (pattern[i] == '*') {
                dp[i+1][j] = true;
                if (j < n) {
                    dp[i][j+1] = true;
                    dp[i+1][j+1] = true;
                }
            } else if (pattern[i] == '?') {
                if (j < n) {
                    dp[i+1][j+1] = true;
                }
            } else {
                if (j < n && pattern[i] == s[j]) {
                    dp[i+1][j+1] = true;
                }
            }
        }
    }
    return dp[m][n];
}

bool dfs(int depth, int max_depth, string& current, vector<string>& match, vector<string>& avoid) {
    if (depth > 0) {
        for (string s : avoid) {
            if (matches(current, s)) {
                return false;
            }
        }
    }
    
    if (depth == max_depth) {
        for (string s : match) {
            if (!matches(current, s)) {
                return false;
            }
        }
        return true;
    }
    
    for (char c = 'a'; c <= 'z'; c++) {
        current.push_back(c);
        if (dfs(depth+1, max_depth, current, match, avoid)) {
            return true;
        }
        current.pop_back();
    }
    
    current.push_back('?');
    if (dfs(depth+1, max_depth, current, match, avoid)) {
        return true;
    }
    current.pop_back();
    
    current.push_back('*');
    if (dfs(depth+1, max_depth, current, match, avoid)) {
        return true;
    }
    current.pop_back();
    
    return false;
}

int main() {
    int n, m;
    cin >> n >> m;
    
    vector<string> match(n);
    for (int i = 0; i < n; i++) {
        cin >> match[i];
    }
    
    vector<string> avoid(m);
    for (int i = 0; i < m; i++) {
        cin >> avoid[i];
    }
    
    for (int len = 0; len <= 5; len++) {
        string current = "";
        if (dfs(0, len, current, match, avoid)) {
            cout << current << endl;
            return 0;
        }
    }
    
    cout << "OOPS" << endl;
    return 0;
}
