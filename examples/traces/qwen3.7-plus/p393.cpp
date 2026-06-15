#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Letter {
    int id;
    int freq;
    int conflict_degree;
    bool operator<(const Letter& other) const {
        if (freq != other.freq) return freq > other.freq;
        return conflict_degree > other.conflict_degree;
    }
};

int N, M;
vector<pair<int, int>> words;
bool conflict[15][15];
vector<int> button_of;

bool solve(int idx, int current_K, int max_K, const vector<Letter>& sorted_letters) {
    if (idx == N) {
        return true;
    }
    
    int letter = sorted_letters[idx].id;
    
    for (int b = 1; b <= current_K + 1; ++b) {
        if (b > max_K) continue;
        
        // Check conflict with already assigned letters
        bool has_conflict = false;
        for (int i = 0; i < idx; ++i) {
            int other_letter = sorted_letters[i].id;
            if (button_of[other_letter] == b) {
                if (conflict[letter][other_letter]) {
                    has_conflict = true;
                    break;
                }
            }
        }
        if (has_conflict) continue;
        
        button_of[letter] = b;
        
        // Check if this assignment causes any vocabulary collision
        bool ok = true;
        bool seen[15][15] = {false};
        for (const auto& w : words) {
            int u = w.first;
            int v = w.second;
            if (button_of[u] != 0 && button_of[v] != 0) {
                int b1 = button_of[u];
                int b2 = button_of[v];
                if (seen[b1][b2]) {
                    ok = false;
                    break;
                }
                seen[b1][b2] = true;
            }
        }
        
        if (ok) {
            int next_K = max(current_K, b);
            if (solve(idx + 1, next_K, max_K, sorted_letters)) {
                return true;
            }
        }
        
        button_of[letter] = 0; // backtrack
    }
    
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    if (!(cin >> N >> M)) return 0;
    
    words.resize(M);
    vector<int> freq(N, 0);
    
    for (int i = 0; i < M; ++i) {
        string s;
        cin >> s;
        words[i] = {s[0] - 'a', s[1] - 'a'};
        freq[s[0] - 'a']++;
        freq[s[1] - 'a']++;
    }
    
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            conflict[i][j] = false;
        }
    }
    
    for (int i = 0; i < M; ++i) {
        for (int j = i + 1; j < M; ++j) {
            int u1 = words[i].first, v1 = words[i].second;
            int u2 = words[j].first, v2 = words[j].second;
            
            if (u1 == u2 && v1 != v2) {
                conflict[v1][v2] = conflict[v2][v1] = true;
            }
            if (v1 == v2 && u1 != u2) {
                conflict[u1][u2] = conflict[u2][u1] = true;
            }
            if (u1 == v1 && u2 == v2) {
                conflict[u1][u2] = conflict[u2][u1] = true;
            }
            if (u1 == v2 && v1 == u2) {
                conflict[u1][v1] = conflict[v1][u1] = true;
            }
        }
    }
    
    vector<Letter> sorted_letters(N);
    for (int i = 0; i < N; ++i) {
        sorted_letters[i].id = i;
        sorted_letters[i].freq = freq[i];
        int deg = 0;
        for (int j = 0; j < N; ++j) {
            if (conflict[i][j]) deg++;
        }
        sorted_letters[i].conflict_degree = deg;
    }
    
    sort(sorted_letters.begin(), sorted_letters.end());
    
    button_of.assign(N, 0);
    
    for (int K = 1; K <= N; ++K) {
        if (solve(0, 0, K, sorted_letters)) {
            cout << K << "\n";
            vector<string> buttons(K);
            for (int i = 0; i < N; ++i) {
                int b = button_of[i];
                if (b >= 1 && b <= K) {
                    buttons[b - 1] += (char)('a' + i);
                }
            }
            for (int i = 0; i < K; ++i) {
                cout << buttons[i] << "\n";
            }
            return 0;
        }
    }
    
    return 0;
}
