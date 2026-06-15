#include <bits/stdc++.h>
using namespace std;

struct Frame {
    char type;      // 'l' or 'd'
    int parity;     // only for dictionaries: number of contained objects modulo 2
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long m;
    string s;
    cin >> m >> s;

    int n = (int)s.size();
    vector<Frame> st;

    enum Mode {
        START,
        STR_LEN,
        STR_CONTENT,
        INT_START,
        INT_DIGITS
    };

    Mode mode = START;

    long long strLen = 0;
    long long strRem = 0;
    bool strLeadingZero = false;

    bool intLeadingZero = false;

    auto isDigit = [](char c) {
        return c >= '0' && c <= '9';
    };

    auto error = [&](int pos) {
        cout << "Error at position " << pos << "!\n";
        exit(0);
    };

    auto minimal_ok = [&](long long prefLen, long long extra) {
        return prefLen + extra + (long long)st.size() <= m;
    };

    bool completeTop = false;

    function<void()> completeObject = [&]() {
        if (st.empty()) {
            completeTop = true;
        } else {
            if (st.back().type == 'd') {
                st.back().parity ^= 1;
            }
        }
    };

    for (int i = 0; i < n; i++) {
        if (completeTop) {
            error(i);
        }

        if (i >= m) {
            error(i);
        }

        char c = s[i];

        if (mode == START) {
            if (c == 'l' || c == 'd') {
                st.push_back({c, 0});
                if ((long long)i + 1 + (long long)st.size() > m) {
                    error(i);
                }
            } else if (c == 'e') {
                if (st.empty()) {
                    error(i);
                }
                if (st.back().type == 'd' && st.back().parity != 0) {
                    error(i);
                }
                st.pop_back();
                completeObject();
            } else if (c == 'i') {
                if (!minimal_ok((long long)i + 1, 2)) {
                    error(i);
                }
                mode = INT_START;
            } else if (isDigit(c)) {
                strLen = c - '0';
                strLeadingZero = (c == '0');

                if (!minimal_ok((long long)i + 1, 1 + strLen)) {
                    error(i);
                }

                mode = STR_LEN;
            } else {
                error(i);
            }
        } else if (mode == STR_LEN) {
            if (strLeadingZero && isDigit(c)) {
                error(i);
            }

            if (isDigit(c)) {
                int digit = c - '0';
                if (strLen > m) {
                    strLen = m + 1;
                } else {
                    strLen = strLen * 10 + digit;
                    if (strLen > m) strLen = m + 1;
                }

                if (!minimal_ok((long long)i + 1, 1 + strLen)) {
                    error(i);
                }
            } else if (c == ':') {
                if (!minimal_ok((long long)i + 1, strLen)) {
                    error(i);
                }

                strRem = strLen;
                if (strRem == 0) {
                    mode = START;
                    completeObject();
                } else {
                    mode = STR_CONTENT;
                }
            } else {
                error(i);
            }
        } else if (mode == STR_CONTENT) {
            strRem--;
            if (strRem == 0) {
                mode = START;
                completeObject();
            }
        } else if (mode == INT_START) {
            if (!isDigit(c)) {
                error(i);
            }

            intLeadingZero = (c == '0');

            if (!minimal_ok((long long)i + 1, 1)) {
                error(i);
            }

            mode = INT_DIGITS;
        } else if (mode == INT_DIGITS) {
            if (intLeadingZero && c != 'e') {
                error(i);
            }

            if (isDigit(c)) {
                if (!minimal_ok((long long)i + 1, 1)) {
                    error(i);
                }
            } else if (c == 'e') {
                mode = START;
                completeObject();
            } else {
                error(i);
            }
        }
    }

    if (completeTop) {
        cout << "ok\n";
    } else {
        error(n);
    }

    return 0;
}
