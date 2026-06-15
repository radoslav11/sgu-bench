#include <bits/stdc++.h>
using namespace std;

map<int, int> baseDurations = {{1, 64}, {2, 32}, {4, 16}, {8, 8}, {16, 4}, {32, 2}, {64, 1}};

long long parseDuration(string s) {
    int base = stoi(s);
    long long duration = baseDurations[base];
    long long dotValue = duration / 2;
    
    for (size_t i = to_string(base).length(); i < s.length(); i++) {
        if (s[i] == '.') {
            if (dotValue < 1) break;
            duration += dotValue;
            dotValue /= 2;
        }
    }
    return duration;
}

long long parseInput(string input) {
    long long total = 0;
    size_t pos = 0;
    while (pos < input.length()) {
        if (input[pos] == 'R') {
            pos++;
            string durStr = "";
            while (pos < input.length() && input[pos] != 'R') {
                durStr += input[pos];
                pos++;
            }
            total += parseDuration(durStr);
        }
    }
    return total;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string input;
    cin >> input;
    
    long long total = parseInput(input);
    
    map<long long, string> dp;
    dp[0] = "";
    
    priority_queue<pair<pair<int,string>, long long>, 
                   vector<pair<pair<int,string>, long long>>,
                   greater<pair<pair<int,string>, long long>>> pq;
    pq.push({{0, ""}, 0});
    
    vector<int> bases = {1, 2, 4, 8, 16, 32, 64};
    
    while (!pq.empty()) {
        auto [p, curr] = pq.top();
        pq.pop();
        auto [len, expr] = p;
        
        if (curr == total) continue;
        if (dp.count(curr) && (dp[curr].length() < expr.length() || 
            (dp[curr].length() == expr.length() && dp[curr] < expr))) {
            continue;
        }
        
        for (int base : bases) {
            long long baseDur = baseDurations[base];
            if (curr + baseDur > total) continue;
            
            long long duration = baseDur;
            long long dotValue = baseDur / 2;
            int dotCount = 0;
            
            while (curr + duration <= total && dotValue >= 1) {
                string cmd = "R" + to_string(base);
                for (int d = 0; d < dotCount; d++) cmd += ".";
                string newExpr = expr + cmd;
                long long newCurr = curr + duration;
                int newLen = newExpr.length();
                
                if (!dp.count(newCurr) || dp[newCurr].length() > newLen ||
                    (dp[newCurr].length() == newLen && dp[newCurr] > newExpr)) {
                    dp[newCurr] = newExpr;
                    pq.push({{newLen, newExpr}, newCurr});
                }
                
                duration += dotValue;
                dotValue /= 2;
                dotCount++;
            }
        }
    }
    
    cout << dp[total] << endl;
    
    return 0;
}
