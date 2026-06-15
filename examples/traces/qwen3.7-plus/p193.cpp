#include <iostream>
#include <string>

using namespace std;

string divide_by_2(const string& s) {
    string res = "";
    res.reserve(s.length());
    int carry = 0;
    for (char c : s) {
        int val = carry * 10 + (c - '0');
        res += (char)('0' + (val / 2));
        carry = val % 2;
    }
    size_t start = res.find_first_not_of('0');
    if (start == string::npos) return "0";
    return res.substr(start);
}

string subtract(const string& s, int val) {
    string res = s;
    int borrow = val;
    for (int i = (int)res.length() - 1; i >= 0; --i) {
        int d = res[i] - '0' - borrow;
        if (d < 0) {
            res[i] = (char)((d + 10) + '0');
            borrow = 1;
        } else {
            res[i] = (char)(d + '0');
            borrow = 0;
            break;
        }
    }
    size_t start = res.find_first_not_of('0');
    if (start == string::npos) return "0";
    return res.substr(start);
}

int get_mod_4(const string& s) {
    if (s.length() == 1) {
        return (s[0] - '0') % 4;
    }
    int val = (s[s.length()-2] - '0') * 10 + (s[s.length()-1] - '0');
    return val % 4;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    string s;
    if (cin >> s) {
        size_t start = s.find_first_not_of('0');
        if (start == string::npos) {
            cout << 0 << "\n";
            return 0;
        }
        s = s.substr(start);
        
        string ans = divide_by_2(s);
        if ((s.back() - '0') % 2 != 0) {
            cout << ans << "\n";
        } else {
            if (get_mod_4(s) == 0) {
                cout << subtract(ans, 1) << "\n";
            } else {
                cout << subtract(ans, 2) << "\n";
            }
        }
    }
    return 0;
}
