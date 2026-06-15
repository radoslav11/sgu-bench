#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int resolve(int c1, int c2) {
    if (c1 == -2) c1 = -1;
    if (c2 == -2) c2 = -1;
    if (c1 == 27 || c2 == 27) return 27;
    if (c1 >= 0 && c2 >= 0) {
        return (c1 == c2) ? c1 : 27;
    }
    if (c1 == -1 && c2 == -1) return 0;
    if (c1 == -1) return c2;
    if (c2 == -1) return c1;
    return 27;
}

vector<int> expand(const string& s) {
    vector<int> res;
    for (char c : s) {
        if (c >= 'a' && c <= 'z') {
            res.push_back(c - 'a');
        } else if (c == '?') {
            res.push_back(-1);
        } else if (c == '!') {
            res.push_back(-1);
            res.push_back(-1);
            res.push_back(-1);
        }
    }
    return res;
}

pair<bool, string> solve(const vector<int>& L, const vector<int>& R) {
    int NL = L.size();
    int NR = R.size();
    for (int M_len = 0; M_len <= NL + NR; ++M_len) {
        int N = NL + M_len + NR;
        vector<int> S(N, -2);
        
        for (int i = 0; i < NL; ++i) {
            S[i] = (i < S.size()) ? L[i] : 27;
        }
        for (int i = 0; i < NR; ++i) {
            if (N - 1 - i < S.size()) {
                S[N - 1 - i] = R[NR - 1 - i];
            }
        }
        
        bool possible = true;
        for (int i = 0; i <= (N - 1) / 2; ++i) {
            int j = N - 1 - i;
            int res = resolve(S[i], S[j]);
            if (res == 27) {
                possible = false;
                break;
            }
            S[i] = S[j] = res;
        }
        
        if (possible) {
            string ans = "";
            for (int i = 0; i < N; ++i) {
                ans += (char)('a' + S[i]);
            }
            return {true, ans};
        }
    }
    return {false, ""};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    string regex;
    if (!(cin >> regex)) return 0;
    
    vector<string> candidates;
    
    string c0 = "";
    for (char c : regex) {
        if (c != '*') c0 += c;
    }
    candidates.push_back(c0);
    
    for (size_t i = 0; i < regex.size(); ++i) {
        if (regex[i] == '*') {
            string c = "";
            for (size_t j = 0; j < regex.size(); ++j) {
                if (i == j) {
                    c += '*';
                } else if (regex[j] != '*') {
                    c += regex[j];
                }
            }
            candidates.push_back(c);
        }
    }
    
    string best = "";
    bool found = false;
    
    for (const string& cand : candidates) {
        int star_pos = -1;
        for (size_t i = 0; i < cand.size(); ++i) {
            if (cand[i] == '*') {
                star_pos = i;
                break;
            }
        }
        
        vector<int> L, R;
        if (star_pos == -1) {
            L = expand(cand);
            R = vector<int>();
        } else {
            L = expand(cand.substr(0, star_pos));
            R = expand(cand.substr(star_pos + 1));
        }
        
        pair<bool, string> res = solve(L, R);
        if (res.first) {
            if (!found || res.second.length() < best.length() || 
                (res.second.length() == best.length() && res.second < best)) {
                best = res.second;
                found = true;
            }
        }
    }
    
    if (found) {
        cout << "YES\n" << best << "\n";
    } else {
        cout << "NO\n";
    }
    
    return 0;
}
