#include <iostream>
#include <string>
#include <cctype>
using namespace std;

string divide_by_2(string s) {
    string res;
    int carry = 0;
    for (int i = 0; i < s.length(); i++) {
        int digit = s[i] - '0';
        int current = carry * 10 + digit;
        int q = current / 2;
        carry = current % 2;
        if (!res.empty() || q > 0) {
            res += ('0' + q);
        }
    }
    if (res.empty()) {
        return "0";
    }
    return res;
}

string subtract_x(string s, int x) {
    string res = s;
    int borrow = x;
    for (int i = res.length() - 1; i >= 0; i--) {
        int digit = res[i] - '0';
        if (digit >= borrow) {
            res[i] = '0' + (digit - borrow);
            borrow = 0;
            break;
        } else {
            res[i] = '0' + (10 + digit - borrow);
            borrow = 1;
        }
    }
    // Remove leading zeros
    int start = 0;
    while (start < res.length() - 1 && res[start] == '0') {
        start++;
    }
    return res.substr(start);
}

int main() {
    string N;
    cin >> N;
    
    char last_digit = N.back();
    bool is_odd = (last_digit == '1' || last_digit == '3' || last_digit == '5' || 
                   last_digit == '7' || last_digit == '9');
    
    if (is_odd) {
        string N_minus_1 = N;
        N_minus_1[N.length() - 1]--;
        string result = divide_by_2(N_minus_1);
        cout << result << endl;
    } else {
        string M = divide_by_2(N);
        int mod4;
        if (N.length() == 1) {
            mod4 = (N[0] - '0') % 4;
        } else {
            string last_two = N.substr(N.length() - 2, 2);
            mod4 = stoi(last_two) % 4;
        }
        string result;
        if (mod4 == 0) {
            result = subtract_x(M, 1);
        } else {
            result = subtract_x(M, 2);
        }
        cout << result << endl;
    }
    return 0;
}
