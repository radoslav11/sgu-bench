#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

bool is_suffix(const string& suffix, const string& str) {
    if (suffix.length() > str.length()) return false;
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

bool is_crossed(const string& X, const string& center, const string& w) {
    if (center.empty()) {
        for (int lenU = 0; lenU <= (int)w.length(); ++lenU) {
            string U = w.substr(0, lenU);
            string rem = w.substr(lenU);
            string V = rem;
            reverse(V.begin(), V.end());
            if (is_suffix(V, U) && is_suffix(U, X)) {
                return true;
            }
        }
    } else {
        char c = center[0];
        if (w.length() == 1 && w[0] == c) return true;
        for (int lenU = 1; lenU <= (int)w.length() - 1; ++lenU) {
            if (w[lenU] == c) {
                string U = w.substr(0, lenU);
                string rem = w.substr(lenU + 1);
                string V = rem;
                reverse(V.begin(), V.end());
                if (is_suffix(V, U) && is_suffix(U, X)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool is_word_covered(const string& X, const string& center, const string& w) {
    if (X.find(w) != string::npos) return true;
    string w_rev = w;
    reverse(w_rev.begin(), w_rev.end());
    if (X.find(w_rev) != string::npos) return true;
    if (is_crossed(X, center, w)) return true;
    if (is_crossed(X, center, w_rev)) return true;
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    if (!(cin >> N)) return 0;
    
    vector<string> words(N);
    vector<string> rev_words(N);
    vector<string> W_all;
    
    for (int i = 0; i < N; ++i) {
        cin >> words[i];
        rev_words[i] = words[i];
        reverse(rev_words[i].begin(), rev_words[i].end());
        W_all.push_back(words[i]);
        W_all.push_back(rev_words[i]);
    }
    
    sort(W_all.begin(), W_all.end());
    W_all.erase(unique(W_all.begin(), W_all.end()), W_all.end());
    int K = W_all.size();

    vector<vector<string>> add_str(K, vector<string>(K));
    vector<vector<int>> cover(K, vector<int>(K, 0));

    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            int max_ov = 0;
            int len = min(W_all[i].length(), W_all[j].length());
            for (int l = 1; l <= len; ++l) {
                if (W_all[i].substr(W_all[i].length() - l) == W_all[j].substr(0, l)) {
                    max_ov = l;
                }
            }
            add_str[i][j] = W_all[j].substr(max_ov);
            string merged = W_all[i] + add_str[i][j];
            int m = 0;
            for (int idx = 0; idx < N; ++idx) {
                if (merged.find(words[idx]) != string::npos || merged.find(rev_words[idx]) != string::npos) {
                    m |= (1 << idx);
                }
            }
            cover[i][j] = m;
        }
    }

    const string INF(2000, 'z' + 1);
    vector<vector<string>> dp(1 << N, vector<string>(K, INF));

    for (int i = 0; i < K; ++i) {
        int m = 0;
        for (int idx = 0; idx < N; ++idx) {
            if (W_all[i].find(words[idx]) != string::npos || W_all[i].find(rev_words[idx]) != string::npos) {
                m |= (1 << idx);
            }
        }
        if (W_all[i].length() < dp[m][i].length()) {
            dp[m][i] = W_all[i];
        }
    }

    for (int mask = 0; mask < (1 << N); ++mask) {
        for (int i = 0; i < K; ++i) {
            if (dp[mask][i].length() >= 1000) continue;
            const string& X = dp[mask][i];
            for (int j = 0; j < K; ++j) {
                int nmask = mask | cover[i][j];
                string nx = X + add_str[i][j];
                if (nx.length() < dp[nmask][j].length()) {
                    dp[nmask][j] = nx;
                }
            }
        }
    }

    int best_len = 1e9;
    string best_P = "";

    vector<string> centers;
    centers.push_back("");
    for (char c = 'a'; c <= 'z'; ++c) {
        centers.push_back(string(1, c));
    }

    for (int mask = 0; mask < (1 << N); ++mask) {
        for (int i = 0; i < K; ++i) {
            if (dp[mask][i].length() >= 1000) continue;
            string X = dp[mask][i];
            
            for (const string& center : centers) {
                for (int len = 1; len <= (int)X.length(); ++len) {
                    if (len * 2 + center.length() >= best_len) continue;
                    string X_pre = X.substr(0, len);
                    bool ok = true;
                    for (int idx = 0; idx < N; ++idx) {
                        if (!is_word_covered(X_pre, center, words[idx])) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        string P = X_pre + center;
                        string X_rev = X_pre;
                        reverse(X_rev.begin(), X_rev.end());
                        P += X_rev;
                        if (P.length() < best_len) {
                            best_len = P.length();
                            best_P = P;
                        } else if (P.length() == best_len && (best_P == "" || P < best_P)) {
                            best_P = P;
                        }
                        break;
                    }
                }
            }
        }
    }

    cout << best_P << "\n";
    return 0;
}
