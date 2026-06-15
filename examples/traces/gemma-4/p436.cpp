#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef long long ll;

const char* diputs_chars = "_.,-~='^\"";
int q_vals[] = {2, 3, 5, 7, 11, 13, 17, 19, 23};
ll B[9];

struct Token {
    int start, end;
    ll value;
    bool original_is_decimal;
    bool is_number;
    string text;
};

bool is_diputs_char(char c) {
    for (int i = 0; i < 9; ++i) if (c == diputs_chars[i]) return true;
    return false;
}

int get_diputs_idx(char c) {
    for (int i = 0; i < 9; ++i) if (c == diputs_chars[i]) return i;
    return -1;
}

string to_diputs(ll V) {
    if (V == 0) return "O";
    int counts[9] = {0};
    ll tempV = V;
    for (int i = 8; i >= 0; --i) {
        counts[i] = (int)(tempV / B[i]);
        tempV %= B[i];
    }
    string res = "";
    for (int i = 8; i >= 0; --i) {
        for (int k = 0; k < counts[i]; ++k) {
            res += diputs_chars[i];
        }
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    B[0] = 1;
    for (int i = 1; i < 9; ++i) B[i] = B[i - 1] * (q_vals[i - 1] + 1);

    string input, line;
    while (getline(cin, line)) {
        input += line + "\n";
    }
    int n = input.length();

    vector<Token> tokens;
    for (int i = 0; i < n; ) {
        if (isdigit(input[i])) {
            int start = i;
            ll val = 0;
            if (input[i] == '0') {
                val = 0;
                i++;
            } else {
                int j = i;
                while (j < n && isdigit(input[j])) {
                    ll next_val = val * 10 + (input[j] - '0');
                    if (next_val <= 836115999LL) {
                        val = next_val;
                        j++;
                    } else break;
                }
                i = j;
            }
            tokens.push_back({start, i, val, true, true, ""});
        } else if (is_diputs_char(input[i])) {
            int start = i;
            int counts[9] = {0};
            int last_idx = 10;
            int j = i;
            while (j < n && is_diputs_char(input[j])) {
                int cur_idx = get_diputs_idx(input[j]);
                if (cur_idx <= last_idx && counts[cur_idx] < q_vals[cur_idx]) {
                    counts[cur_idx]++;
                    last_idx = cur_idx;
                    j++;
                } else break;
            }
            ll val = 0;
            for (int k = 0; k < 9; ++k) val += (ll)counts[k] * B[k];
            tokens.push_back({start, j, val, false, true, ""});
            i = j;
        } else {
            int start = i;
            while (i < n && !isdigit(input[i]) && !is_diputs_char(input[i])) i++;
            tokens.push_back({start, i, 0, false, false, input.substr(start, i - start)});
        }
    }

    vector<int> num_indices;
    for (int i = 0; i < (int)tokens.size(); ++i) {
        if (tokens[i].is_number) num_indices.push_back(i);
    }

    if (num_indices.size() % 2 != 0) {
        vector<pair<ll, int>> sorted_vals;
        for (int idx : num_indices) sorted_vals.push_back({tokens[idx].value, idx});
        sort(sorted_vals.begin(), sorted_vals.end());
        for (int i = 0; i < (int)num_indices.size(); ++i) {
            tokens[num_indices[i]].value = sorted_vals[i].first;
        }
    }

    string output = "";
    for (auto& tk : tokens) {
        if (!tk.is_number) {
            output += tk.text;
        } else {
            if (tk.original_is_decimal) output += to_diputs(tk.value);
            else output += to_string(tk.value);
        }
    }
    cout << output;

    return 0;
}

