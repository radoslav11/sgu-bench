#include <bits/stdc++.h>
using namespace std;

struct Node {
    string key;
    int parent;
    char token;
    int depth;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> s(n + m);
    for (int i = 0; i < n + m; i++) cin >> s[i];

    int k = n + m;

    auto getMask = [&](const string& key, int idx) -> unsigned short {
        unsigned char lo = key[2 * idx];
        unsigned char hi = key[2 * idx + 1];
        return (unsigned short)(lo | (hi << 8));
    };

    auto setMask = [&](string& key, int idx, unsigned short mask) {
        key[2 * idx] = char(mask & 255);
        key[2 * idx + 1] = char(mask >> 8);
    };

    auto applyToken = [&](const string& key, char tok) -> string {
        string nxt(2 * k, '\0');

        for (int i = 0; i < k; i++) {
            unsigned short mask = getMask(key, i);
            unsigned short nm = 0;
            int len = (int)s[i].size();

            if (tok == '*') {
                int mn = -1;
                for (int p = 0; p <= len; p++) {
                    if (mask & (1u << p)) {
                        mn = p;
                        break;
                    }
                }
                if (mn != -1) {
                    for (int p = mn; p <= len; p++) {
                        nm |= (1u << p);
                    }
                }
            } else if (tok == '?') {
                for (int p = 0; p < len; p++) {
                    if (mask & (1u << p)) {
                        nm |= (1u << (p + 1));
                    }
                }
            } else {
                for (int p = 0; p < len; p++) {
                    if ((mask & (1u << p)) && s[i][p] == tok) {
                        nm |= (1u << (p + 1));
                    }
                }
            }

            setMask(nxt, i, nm);
        }

        return nxt;
    };

    auto alivePositive = [&](const string& key) -> bool {
        for (int i = 0; i < n; i++) {
            if (getMask(key, i) == 0) return false;
        }
        return true;
    };

    auto good = [&](const string& key) -> bool {
        for (int i = 0; i < n; i++) {
            int len = (int)s[i].size();
            if (!(getMask(key, i) & (1u << len))) return false;
        }
        for (int i = n; i < n + m; i++) {
            int len = (int)s[i].size();
            if (getMask(key, i) & (1u << len)) return false;
        }
        return true;
    };

    string start(2 * k, '\0');
    for (int i = 0; i < k; i++) {
        setMask(start, i, 1);
    }

    vector<Node> nodes;
    queue<int> q;
    unordered_map<string, int> seen;

    nodes.push_back({start, -1, 0, 0});
    seen[start] = 0;
    q.push(0);

    int answer = -1;

    if (good(start)) {
        answer = 0;
    }

    vector<char> tokens;
    tokens.push_back('*');
    tokens.push_back('?');
    for (char c = 'a'; c <= 'z'; c++) tokens.push_back(c);

    while (!q.empty() && answer == -1) {
        int id = q.front();
        q.pop();

        if (nodes[id].depth == 100) continue;

        for (char tok : tokens) {
            if (tok == '*' && nodes[id].token == '*') continue;

            string nxt = applyToken(nodes[id].key, tok);

            if (!alivePositive(nxt)) continue;
            if (seen.count(nxt)) continue;

            int nid = (int)nodes.size();
            seen[nxt] = nid;
            nodes.push_back({nxt, id, tok, nodes[id].depth + 1});

            if (good(nxt)) {
                answer = nid;
                break;
            }

            q.push(nid);
        }
    }

    if (answer == -1) {
        cout << "OOPS\n";
        return 0;
    }

    string res;
    while (answer != 0) {
        res.push_back(nodes[answer].token);
        answer = nodes[answer].parent;
    }
    reverse(res.begin(), res.end());

    if ((int)res.size() > 100) {
        cout << "OOPS\n";
    } else {
        cout << res << '\n';
    }

    return 0;
}
