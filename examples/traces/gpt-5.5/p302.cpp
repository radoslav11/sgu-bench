#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    getline(cin, s);

    vector<int> st; // 1 = UP, 2 = DOWN

    for (size_t i = 0; i < s.size(); ) {
        if (s[i] == '<') {
            size_t j = s.find('>', i);
            string tag = s.substr(i + 1, j - i - 1);

            if (!tag.empty() && tag[0] == '/') {
                if (!st.empty()) st.pop_back();
            } else {
                if (tag == "UP") st.push_back(1);
                else if (tag == "DOWN") st.push_back(2);
            }

            i = j + 1;
        } else {
            char c = s[i];

            if (!st.empty()) {
                if (st.back() == 1 && c >= 'a' && c <= 'z') {
                    c = char(c - 'a' + 'A');
                } else if (st.back() == 2 && c >= 'A' && c <= 'Z') {
                    c = char(c - 'A' + 'a');
                }
            }

            cout << c;
            ++i;
        }
    }

    return 0;
}
