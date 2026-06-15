#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cctype>

using namespace std;

const int MAX_COUNTS[9] = {2, 3, 5, 7, 11, 13, 17, 19, 23};

int get_sig(char c) {
    if (c == '_') return 1;
    if (c == '.') return 2;
    if (c == ',') return 3;
    if (c == '-') return 4;
    if (c == '~') return 5;
    if (c == '=') return 6;
    if (c == '\'') return 7;
    if (c == '^') return 8;
    if (c == '"') return 9;
    return 0;
}

char get_char(int sig) {
    if (sig == 1) return '_';
    if (sig == 2) return '.';
    if (sig == 3) return ',';
    if (sig == 4) return '-';
    if (sig == 5) return '~';
    if (sig == 6) return '=';
    if (sig == 7) return '\'';
    if (sig == 8) return '^';
    if (sig == 9) return '"';
    return ' ';
}

bool is_diputs_char(char c) {
    return get_sig(c) != 0 || c == 'O';
}

long long diputs_to_val(const string& S) {
    if (S == "O") return 0;
    long long val = 0;
    long long multipliers[9];
    multipliers[0] = 1;
    for (int i = 1; i < 9; i++) {
        multipliers[i] = multipliers[i-1] * (MAX_COUNTS[i-1] + 1);
    }
    int counts[9] = {0};
    for (char c : S) {
        int sig = get_sig(c);
        counts[sig - 1]++;
    }
    for (int i = 0; i < 9; i++) {
        val += (long long)counts[i] * multipliers[i];
    }
    return val;
}

string val_to_diputs(long long val) {
    if (val == 0) return "O";
    string res = "";
    long long multipliers[9];
    multipliers[0] = 1;
    for (int i = 1; i < 9; i++) {
        multipliers[i] = multipliers[i-1] * (MAX_COUNTS[i-1] + 1);
    }
    
    for (int i = 8; i >= 0; i--) {
        long long count = val / multipliers[i];
        val %= multipliers[i];
        for (int j = 0; j < count; j++) {
            res += get_char(i + 1);
        }
    }
    return res;
}

struct Item {
    bool is_num;
    bool is_dec;
    long long val;
    int start;
    int len;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    string s((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());
    int n = s.length();
    vector<Item> items;
    items.reserve(n / 2 + 1); 
    
    int i = 0;
    while (i < n) {
        if (isdigit((unsigned char)s[i])) {
            int len = 1;
            long long current_val = s[i] - '0';
            if (s[i] == '0') {
                len = 1;
            } else {
                while (i + len < n && isdigit((unsigned char)s[i + len])) {
                    long long next_val = current_val * 10 + (s[i + len] - '0');
                    if (next_val <= 836075519LL) {
                        current_val = next_val;
                        len++;
                    } else {
                        break;
                    }
                }
            }
            items.push_back({true, true, current_val, i, len});
            i += len;
        } else if (is_diputs_char(s[i])) {
            int len = 1;
            if (s[i] == 'O') {
                len = 1;
            } else {
                int counts[9] = {0};
                int last_sig = get_sig(s[i]);
                counts[last_sig - 1]++;
                while (i + len < n) {
                    char next_c = s[i + len];
                    int next_sig = get_sig(next_c);
                    if (next_sig == 0) break;
                    if (next_sig > last_sig) break;
                    if (counts[next_sig - 1] + 1 > MAX_COUNTS[next_sig - 1]) break;
                    
                    counts[next_sig - 1]++;
                    last_sig = next_sig;
                    len++;
                }
            }
            long long val = diputs_to_val(s.substr(i, len));
            items.push_back({true, false, val, i, len});
            i += len;
        } else {
            int len = 1;
            while (i + len < n && !isdigit((unsigned char)s[i + len]) && !is_diputs_char(s[i + len])) {
                len++;
            }
            items.push_back({false, false, 0, i, len});
            i += len;
        }
    }
    
    vector<long long> vals;
    vals.reserve(items.size());
    for (const auto& item : items) {
        if (item.is_num) {
            vals.push_back(item.val);
        }
    }
    
    if (vals.size() % 2 == 1) {
        sort(vals.begin(), vals.end());
    }
    
    int v_idx = 0;
    for (const auto& item : items) {
        if (item.is_num) {
            long long new_val = vals[v_idx++];
            if (item.is_dec) {
                cout << val_to_diputs(new_val);
            } else {
                cout << new_val;
            }
        } else {
            cout.write(s.c_str() + item.start, item.len);
        }
    }
    
    return 0;
}
