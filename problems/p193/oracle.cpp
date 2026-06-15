// Second oracle for p193, independent of the reference's gcd loop.
// Closed form: for odd N the answer is floor(N/2); for N % 4 == 0 it is
// N/2 - 1; for N % 4 == 2 it is N/2 - 2. N has up to ~2001 decimal
// digits, handled as a string.
#include <iostream>
#include <string>

using namespace std;

string half(const string& s) {
    string res;
    int carry = 0;
    for(char c : s) {
        int d = carry * 10 + (c - '0');
        res.push_back('0' + d / 2);
        carry = d % 2;
    }
    size_t pos = res.find_first_not_of('0');
    return pos == string::npos ? "0" : res.substr(pos);
}

string sub_small(string s, int k) {
    int i = (int)s.size() - 1;
    int borrow = k;
    while(borrow > 0 && i >= 0) {
        int d = (s[i] - '0') - borrow % 10;
        borrow /= 10;
        if(d < 0) {
            d += 10;
            borrow += 1;
        }
        s[i--] = '0' + d;
    }
    size_t pos = s.find_first_not_of('0');
    return pos == string::npos ? "0" : s.substr(pos);
}

int main() {
    string s;
    cin >> s;
    int last = s.back() - '0';
    if(last % 2 == 1) {
        cout << half(s) << endl;
        return 0;
    }
    int mod4 = (s.size() >= 2 ? (s[s.size() - 2] - '0') * 10 + last : last) % 4;
    cout << sub_small(half(s), mod4 == 0 ? 1 : 2) << endl;
    return 0;
}
