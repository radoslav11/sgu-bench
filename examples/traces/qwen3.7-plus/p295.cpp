#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

unsigned long long C[65][65];

void precompute() {
    for (int i = 0; i <= 64; ++i) {
        for (int j = 0; j <= 64; ++j) {
            if (j == 0) {
                C[i][j] = 1;
            } else if (j > i) {
                C[i][j] = 0;
            } else {
                C[i][j] = C[i-1][j-1] + C[i-1][j];
            }
        }
    }
}

bool is_replaceable(char ch) {
    string valid = "ABCEHKMOPTXaceopxy";
    return valid.find(ch) != string::npos;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    precompute();
    
    int q, c;
    if (!(cin >> q >> c)) return 0;
    
    string dummy;
    getline(cin, dummy);
    string s;
    getline(cin, s);
    if (!s.empty() && s.back() == '\r') {
        s.pop_back();
    }
    
    vector<string> words;
    string current_word = "";
    for (char ch : s) {
        if (ch == ' ') {
            if (!current_word.empty()) {
                words.push_back(current_word);
                current_word = "";
            }
        } else {
            current_word += ch;
        }
    }
    if (!current_word.empty()) {
        words.push_back(current_word);
    }
    
    int W = words.size();
    int L = 0;
    int M = 0;
    for (const string& w : words) {
        L += w.length();
        for (char ch : w) {
            if (is_replaceable(ch)) {
                M++;
            }
        }
    }
    
    int S_min = max(W - 1, q - L);
    int S_max = c - L;
    
    unsigned long long total_ways = 0;
    if (S_min <= S_max) {
        for (int S = S_min; S <= S_max; ++S) {
            total_ways += C[S + 1][W];
        }
    }
    
    unsigned long long ans = (1ULL << M) * total_ways - 1;
    cout << ans << "\n";
    
    return 0;
}
