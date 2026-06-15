#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (cin >> s) {
        string left_stk, right_stk;
        left_stk.reserve(s.length());
        right_stk.reserve(s.length());

        for (char c : s) {
            if (c >= 'a' && c <= 'z') {
                left_stk.push_back(c);
            } else if (c == 'L') {
                if (!left_stk.empty()) {
                    right_stk.push_back(left_stk.back());
                    left_stk.pop_back();
                }
            } else if (c == 'R') {
                if (!right_stk.empty()) {
                    left_stk.push_back(right_stk.back());
                    right_stk.pop_back();
                }
            }
        }

        reverse(right_stk.begin(), right_stk.end());
        cout << left_stk << right_stk << "\n";
    }

    return 0;
}
