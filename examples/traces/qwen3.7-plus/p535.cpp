#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

pair<string, string> reduce(const string& S) {
    string req = "";
    string push = "";
    vector<char> current_req;
    
    for (char c : S) {
        if (islower(c)) {
            current_req.push_back(c);
        } else {
            char target = tolower(c);
            if (!current_req.empty() && current_req.back() == target) {
                current_req.pop_back();
            } else {
                if (!current_req.empty()) {
                    return {"!", ""};
                }
                req += target;
            }
        }
    }
    for (char c : current_req) {
        push += c;
    }
    return {req, push};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    string s;
    if (!(cin >> s)) return 0;
    
    int initial_lower = 0;
    for (char c : s) {
        if (islower(c)) {
            initial_lower++;
        }
    }
    
    vector<string> segments;
    string current = "";
    for (char c : s) {
        if (c == '*') {
            segments.push_back(current);
            current = "";
        } else {
            current += c;
        }
    }
    segments.push_back(current);
    
    map<string, int> dp;
    dp[""] = 0;
    
    for (const string& seg : segments) {
        auto [req, push] = reduce(seg);
        if (req == "!") {
            cout << -1 << "\n";
            return 0;
        }
        
        map<string, int> new_dp;
        for (const auto& pair : dp) {
            const string& C = pair.first;
            int cost = pair.second;
            
            size_t max_limit = min(req.length(), C.length());
            for (size_t j = 0; j <= C.length(); ++j) {
                size_t limit = min(req.length(), j);
                for (size_t m = 0; m <= limit; ++m) {
                    if (m > 0) {
                        bool ok = true;
                        for (size_t k = 0; k < m; ++k) {
                            if (C[j - m + k] != req[k]) {
                                ok = false;
                                break;
                            }
                        }
                        if (!ok) continue;
                    }
                    
                    string new_stack = C.substr(0, j - m) + push;
                    int new_cost = cost + (int)req.length() - (int)m;
                    
                    auto it = new_dp.find(new_stack);
                    if (it == new_dp.end() || new_cost < it->second) {
                        new_dp[new_stack] = new_cost;
                    }
                }
            }
        }
        dp = move(new_dp);
    }
    
    if (dp.find("") != dp.end()) {
        cout << initial_lower + dp[""] << "\n";
    } else {
        cout << -1 << "\n";
    }
    
    return 0;
}
