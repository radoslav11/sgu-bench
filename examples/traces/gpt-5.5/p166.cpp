#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string commands;
    getline(cin, commands);

    vector<string> lines(1, "");
    int row = 0, col = 0;

    for (char cmd : commands) {
        if (cmd == 'Q') break;

        string &cur = lines[row];

        if ((cmd >= 'a' && cmd <= 'z') || cmd == ' ') {
            if (col > (int)cur.size()) cur.resize(col, ' ');
            cur.insert(cur.begin() + col, cmd);
            col++;
        } else if (cmd == 'L') {
            if (col > 0) col--;
        } else if (cmd == 'R') {
            col++;
        } else if (cmd == 'U') {
            if (row > 0) row--;
        } else if (cmd == 'D') {
            if (row + 1 < (int)lines.size()) row++;
        } else if (cmd == 'N') {
            string tail;
            if (col < (int)cur.size()) {
                tail = cur.substr(col);
                cur.erase(col);
            }
            lines.insert(lines.begin() + row + 1, tail);
            row++;
            col = 0;
        } else if (cmd == 'E') {
            col = (int)cur.size();
        } else if (cmd == 'H') {
            col = 0;
        } else if (cmd == 'X') {
            if (col < (int)cur.size()) {
                cur.erase(cur.begin() + col);
            } else {
                if (row + 1 < (int)lines.size()) {
                    if (col > (int)cur.size()) cur.resize(col, ' ');
                    cur += lines[row + 1];
                    lines.erase(lines.begin() + row + 1);
                }
            }
        } else if (cmd == 'B') {
            if (col > (int)cur.size()) {
                col--;
            } else if (col > 0) {
                cur.erase(cur.begin() + col - 1);
                col--;
            } else {
                if (row > 0) {
                    int prevLen = (int)lines[row - 1].size();
                    lines[row - 1] += cur;
                    lines.erase(lines.begin() + row);
                    row--;
                    col = prevLen;
                }
            }
        }
    }

    for (int i = 0; i < (int)lines.size(); i++) {
        if (i) cout << '\n';
        cout << lines[i];
    }

    return 0;
}
