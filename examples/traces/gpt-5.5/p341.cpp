#include <bits/stdc++.h>
using namespace std;

enum Type {
    UNDEF, INP, DFF, NOTG, ANDG, NANDG, ORG, NORG
};

struct Node {
    Type type = UNDEF;
    vector<int> args;
    vector<int> out;
    int indeg = 0;
    int val = 0;
    int state = 0;
};

static string trim(const string& s) {
    size_t l = 0, r = s.size();
    while (l < r && isspace((unsigned char)s[l])) l++;
    while (r > l && isspace((unsigned char)s[r - 1])) r--;
    return s.substr(l, r - l);
}

static string removeSpaces(const string& s) {
    string t;
    for (char c : s)
        if (!isspace((unsigned char)c))
            t += c;
    return t;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<Node> nodes;
    unordered_map<string, int> id;
    id.reserve(10000);

    auto getId = [&](const string& name) -> int {
        auto it = id.find(name);
        if (it != id.end()) return it->second;
        int v = (int)nodes.size();
        id[name] = v;
        nodes.push_back(Node());
        return v;
    };

    vector<int> inputs, outputs, dffs;
    string line;

    while (getline(cin, line)) {
        string s = trim(line);
        if (s.empty()) continue;
        if (s[0] == '#') continue;
        if (s == "INPUT VALUES") break;

        string t = removeSpaces(s);

        if (t.rfind("INPUT(", 0) == 0) {
            string name = t.substr(6, t.size() - 7);
            int v = getId(name);
            nodes[v].type = INP;
            inputs.push_back(v);
        } else if (t.rfind("OUTPUT(", 0) == 0) {
            string name = t.substr(7, t.size() - 8);
            int v = getId(name);
            outputs.push_back(v);
        } else {
            size_t eq = t.find('=');
            size_t lp = t.find('(', eq + 1);
            size_t rp = t.rfind(')');

            string lhs = t.substr(0, eq);
            string op = t.substr(eq + 1, lp - eq - 1);
            string inside = t.substr(lp + 1, rp - lp - 1);

            int v = getId(lhs);

            if (op == "DFF") nodes[v].type = DFF;
            else if (op == "NOT") nodes[v].type = NOTG;
            else if (op == "AND") nodes[v].type = ANDG;
            else if (op == "NAND") nodes[v].type = NANDG;
            else if (op == "OR") nodes[v].type = ORG;
            else if (op == "NOR") nodes[v].type = NORG;

            nodes[v].args.clear();

            size_t start = 0;
            while (start <= inside.size()) {
                size_t comma = inside.find(',', start);
                string arg = (comma == string::npos)
                           ? inside.substr(start)
                           : inside.substr(start, comma - start);
                nodes[v].args.push_back(getId(arg));
                if (comma == string::npos) break;
                start = comma + 1;
            }

            if (nodes[v].type == DFF) {
                dffs.push_back(v);
            }
        }
    }

    int n = (int)nodes.size();

    for (int v = 0; v < n; v++) {
        if (nodes[v].type == DFF) continue;
        if (nodes[v].type == INP || nodes[v].type == UNDEF) continue;

        for (int u : nodes[v].args) {
            nodes[u].out.push_back(v);
            nodes[v].indeg++;
        }
    }

    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (nodes[i].indeg == 0) q.push(i);
    }

    vector<int> topo;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        topo.push_back(v);

        for (int to : nodes[v].out) {
            if (--nodes[to].indeg == 0) {
                q.push(to);
            }
        }
    }

    while (getline(cin, line)) {
        string s = trim(line);
        if (s.empty()) continue;

        int p = 0;
        for (int v : inputs) {
            nodes[v].val = s[p++] - '0';
        }

        for (int v : topo) {
            switch (nodes[v].type) {
                case INP:
                    break;

                case DFF:
                    nodes[v].val = nodes[v].state;
                    break;

                case NOTG:
                    nodes[v].val = !nodes[nodes[v].args[0]].val;
                    break;

                case ANDG: {
                    int res = 1;
                    for (int u : nodes[v].args) res &= nodes[u].val;
                    nodes[v].val = res;
                    break;
                }

                case NANDG: {
                    int res = 1;
                    for (int u : nodes[v].args) res &= nodes[u].val;
                    nodes[v].val = !res;
                    break;
                }

                case ORG: {
                    int res = 0;
                    for (int u : nodes[v].args) res |= nodes[u].val;
                    nodes[v].val = res;
                    break;
                }

                case NORG: {
                    int res = 0;
                    for (int u : nodes[v].args) res |= nodes[u].val;
                    nodes[v].val = !res;
                    break;
                }

                default:
                    break;
            }
        }

        for (int v : outputs) {
            cout << nodes[v].val;
        }
        cout << '\n';

        vector<int> nextState;
        nextState.reserve(dffs.size());
        for (int v : dffs) {
            nextState.push_back(nodes[nodes[v].args[0]].val);
        }
        for (size_t i = 0; i < dffs.size(); i++) {
            nodes[dffs[i]].state = nextState[i];
        }
    }

    return 0;
}
