#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

bool reach[105][105];
vector<vector<int>> H;
vector<int> cur;

struct ASTNode {
    int type;
    int val;
    int left;
    int right;
};

vector<ASTNode> ast;
vector<string> tokens;
int token_idx;

int D_global = 0;
int zero_id = -1, one_id = -1;
vector<vector<int>> impl_id, and_id, or_id, equiv_id;

int make_node(int type, int val, int left, int right) {
    ast.push_back({type, val, left, right});
    return (int)ast.size() - 1;
}

vector<int> get_max(const vector<int>& M) {
    vector<int> res;
    for (size_t i = 0; i < M.size(); ++i) {
        bool is_max = true;
        for (size_t j = 0; j < M.size(); ++j) {
            if (i == j) continue;
            if (reach[M[i]][M[j]]) {
                is_max = false;
                break;
            }
        }
        if (is_max) res.push_back(M[i]);
    }
    sort(res.begin(), res.end());
    return res;
}

vector<int> op_impl(const vector<int>& a, const vector<int>& b) {
    vector<int> res;
    for (int x : b) {
        bool ok = true;
        for (int y : a) {
            if (reach[x][y]) {
                ok = false;
                break;
            }
        }
        if (ok) res.push_back(x);
    }
    sort(res.begin(), res.end());
    return res;
}

vector<int> op_and(const vector<int>& a, const vector<int>& b) {
    vector<int> u = a;
    u.insert(u.end(), b.begin(), b.end());
    sort(u.begin(), u.end());
    u.erase(unique(u.begin(), u.end()), u.end());
    return get_max(u);
}

vector<int> op_or(const vector<int>& a, const vector<int>& b) {
    vector<int> res;
    for (int x = 1; x <= 100; ++x) {
        bool ok = false;
        for (int y : a) {
            for (int z : b) {
                if (reach[x][y] && reach[x][z]) {
                    ok = true;
                    break;
                }
            }
            if (ok) break;
        }
        if (ok) res.push_back(x);
    }
    return get_max(res);
}

vector<int> op_equiv(const vector<int>& a, const vector<int>& b) {
    return op_and(op_impl(a, b), op_impl(b, a));
}

int get_id(const vector<int>& v) {
    for (size_t i = 0; i < H.size(); ++i) {
        if (H[i] == v) return (int)i;
    }
    return -1;
}

void find_antichains(int idx, int N) {
    if (idx > N) {
        H.push_back(cur);
        return;
    }
    find_antichains(idx + 1, N);
    
    bool can_add = true;
    for (int v : cur) {
        if (reach[idx][v] || reach[v][idx]) {
            can_add = false;
            break;
        }
    }
    if (can_add) {
        cur.push_back(idx);
        find_antichains(idx + 1, N);
        cur.pop_back();
    }
}

int eval(int node_idx, const vector<int>& assign) {
    const ASTNode& node = ast[node_idx];
    if (node.type == 0) return assign[node.val];
    if (node.type == 1) return node.val == 0 ? zero_id : one_id;
    if (node.type == 2) return impl_id[eval(node.left, assign)][zero_id];
    if (node.type == 3) return and_id[eval(node.left, assign)][eval(node.right, assign)];
    if (node.type == 4) return or_id[eval(node.left, assign)][eval(node.right, assign)];
    if (node.type == 5) return impl_id[eval(node.left, assign)][eval(node.right, assign)];
    if (node.type == 6) return equiv_id[eval(node.left, assign)][eval(node.right, assign)];
    return -1;
}

vector<string> tokenize(const string& s) {
    vector<string> res;
    int i = 0;
    while (i < (int)s.length()) {
        if (isspace(s[i])) {
            i++;
        } else if (i + 1 < (int)s.length() && s[i] == '=' && s[i+1] == '>') {
            res.push_back("=>");
            i += 2;
        } else {
            res.push_back(string(1, s[i]));
            i++;
        }
    }
    return res;
}

int parse_equivalence();
int parse_impl();
int parse_disj();
int parse_conj();
int parse_unary();
int parse_primary();

int parse_equivalence() {
    int res = parse_impl();
    vector<int> list;
    list.push_back(res);
    while (token_idx < (int)tokens.size() && tokens[token_idx] == "=") {
        token_idx++;
        list.push_back(parse_impl());
    }
    if (list.size() == 1) return list[0];
    int ans = make_node(6, -1, list[0], list[1]);
    for (size_t i = 1; i < list.size() - 1; ++i) {
        int eq_node = make_node(6, -1, list[i], list[i+1]);
        ans = make_node(3, -1, ans, eq_node);
    }
    return ans;
}

int parse_impl() {
    int res = parse_disj();
    if (token_idx < (int)tokens.size() && tokens[token_idx] == "=>") {
        token_idx++;
        int right = parse_impl();
        return make_node(5, -1, res, right);
    }
    return res;
}

int parse_disj() {
    int res = parse_conj();
    while (token_idx < (int)tokens.size() && tokens[token_idx] == "|") {
        token_idx++;
        int right = parse_conj();
        res = make_node(4, -1, res, right);
    }
    return res;
}

int parse_conj() {
    int res = parse_unary();
    while (token_idx < (int)tokens.size() && tokens[token_idx] == "&") {
        token_idx++;
        int right = parse_unary();
        res = make_node(3, -1, res, right);
    }
    return res;
}

int parse_unary() {
    if (token_idx < (int)tokens.size() && tokens[token_idx] == "~") {
        token_idx++;
        int child = parse_unary();
        return make_node(2, -1, child, -1);
    }
    return parse_primary();
}

int parse_primary() {
    string t = tokens[token_idx++];
    if (t == "(") {
        int res = parse_equivalence();
        token_idx++; 
        return res;
    } else if (t == "0") {
        return make_node(1, 0, -1, -1);
    } else if (t == "1") {
        return make_node(1, 1, -1, -1);
    } else {
        return make_node(0, t[0] - 'A', -1, -1);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    while (cin >> N >> M) {
        for (int i = 1; i <= N; ++i) {
            for (int j = 1; j <= N; ++j) {
                reach[i][j] = (i == j);
            }
        }
        for (int i = 0; i < M; ++i) {
            int u, v;
            cin >> u >> v;
            reach[u][v] = true;
        }
        for (int k = 1; k <= N; ++k) {
            for (int i = 1; i <= N; ++i) {
                for (int j = 1; j <= N; ++j) {
                    if (reach[i][k] && reach[k][j]) {
                        reach[i][j] = true;
                    }
                }
            }
        }
        
        H.clear();
        cur.clear();
        find_antichains(1, N);
        int D = (int)H.size();
        D_global = D;
        
        zero_id = -1;
        one_id = -1;
        vector<int> all_verts;
        for (int i = 1; i <= N; ++i) all_verts.push_back(i);
        vector<int> max_all = get_max(all_verts);
        for (int i = 0; i < D; ++i) {
            if (H[i] == max_all) zero_id = i;
            if (H[i].empty()) one_id = i;
        }
        
        impl_id.assign(D, vector<int>(D));
        and_id.assign(D, vector<int>(D));
        or_id.assign(D, vector<int>(D));
        equiv_id.assign(D, vector<int>(D));
        
        for (int i = 0; i < D; ++i) {
            for (int j = 0; j < D; ++j) {
                impl_id[i][j] = get_id(op_impl(H[i], H[j]));
                and_id[i][j] = get_id(op_and(H[i], H[j]));
                or_id[i][j] = get_id(op_or(H[i], H[j]));
                equiv_id[i][j] = get_id(op_equiv(H[i], H[j]));
            }
        }
        
        int K;
        if (!(cin >> K)) break;
        string line;
        getline(cin, line);
        for (int i = 0; i < K; ++i) {
            getline(cin, line);
            if (!line.empty() && line.back() == '\r') line.pop_back();
            
            tokens = tokenize(line);
            token_idx = 0;
            ast.clear();
            
            vector<int> vars;
            for (char c = 'A'; c <= 'Z'; ++c) {
                if (line.find(c) != string::npos) {
                    vars.push_back(c - 'A');
                }
            }
            
            int root = parse_equivalence();
            
            bool is_valid = true;
            vector<int> assign(26, 0);
            
            auto check_assign = [&](auto& self, int var_idx) -> void {
                if (!is_valid) return;
                if (var_idx == (int)vars.size()) {
                    int res = eval(root, assign);
                    if (res != one_id) {
                        is_valid = false;
                    }
                    return;
                }
                int var = vars[var_idx];
                for (int v = 0; v < D; ++v) {
                    assign[var] = v;
                    self(self, var_idx + 1);
                    if (!is_valid) return;
                }
            };
            
            check_assign(check_assign, 0);
            
            if (is_valid) {
                cout << "valid\n";
            } else {
                cout << "invalid\n";
            }
        }
    }
    return 0;
}
