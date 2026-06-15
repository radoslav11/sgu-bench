#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cctype>

using namespace std;

struct Op {
    enum Type { CMD, ARG, CALL } type;
    char cmd;
    char arg;
    struct Call {
        char func_name;
        vector<char> args;
    } call;
};

struct Function {
    char name;
    vector<char> params;
    vector<Op> ops;
};

struct Program {
    map<char, Function> funcs;
};

Program p;
int pos_global = 0;

Program parse(const string& s) {
    Program prog;
    while (pos_global < (int)s.length()) {
        char name = s[pos_global++];
        if (pos_global >= (int)s.length()) break;
        if (s[pos_global] == '(') pos_global++;
        vector<char> params;
        if (s[pos_global] != ')') {
            params.push_back(s[pos_global++]);
            if (s[pos_global] == ',') {
                pos_global++;
                params.push_back(s[pos_global++]);
            }
        }
        if (s[pos_global] == ')') pos_global++;
        if (s[pos_global] == '{') pos_global++;
        
        Function f;
        f.name = name;
        f.params = params;
        while (pos_global < (int)s.length() && s[pos_global] != '}') {
            Op op;
            if (s[pos_global] == 'L' || s[pos_global] == 'R' || s[pos_global] == 'C') {
                op.type = Op::CMD;
                op.cmd = s[pos_global++];
            } else if (islower((unsigned char)s[pos_global])) {
                op.type = Op::CALL;
                op.call.func_name = s[pos_global++];
                if (s[pos_global] == '(') pos_global++;
                if (s[pos_global] != ')') {
                    op.call.args.push_back(s[pos_global++]);
                    if (s[pos_global] == ',') {
                        pos_global++;
                        op.call.args.push_back(s[pos_global++]);
                    }
                }
                if (s[pos_global] == ')') pos_global++;
            } else if (isupper((unsigned char)s[pos_global])) {
                op.type = Op::ARG;
                op.arg = s[pos_global++];
            } else {
                pos_global++;
            }
            f.ops.push_back(op);
        }
        if (s[pos_global] == '}') pos_global++;
        prog.funcs[name] = f;
    }
    return prog;
}

int r, c;
int dir; // 0: N, 1: W, 2: S, 3: E
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, -1, 0, 1};
vector<pair<int, int>> ans;
bool visited[55][55];
string grid[55];
int N, M;

void execute_cmd(char cmd) {
    if (cmd == 'L') {
        dir = (dir + 1) % 4;
    } else if (cmd == 'R') {
        dir = (dir + 3) % 4;
    } else if (cmd == 'C') {
        int nr = r + dr[dir];
        int nc = c + dc[dir];
        if (nr >= 0 && nr < N && nc >= 0 && nc < M) {
            r = nr;
            c = nc;
            if (grid[r][c] == '*' && !visited[r][c]) {
                visited[r][c] = true;
                ans.push_back({r + 1, c + 1});
            }
        }
    }
}

void run(const string& func_name, map<char, char> env) {
    const auto& f = p.funcs[func_name[0]];
    for (const auto& op : f.ops) {
        if (op.type == Op::CMD) {
            execute_cmd(op.cmd);
        } else if (op.type == Op::ARG) {
            char val = env[op.arg];
            execute_cmd(val);
        } else if (op.type == Op::CALL) {
            map<char, char> new_env;
            for (size_t i = 0; i < op.call.args.size(); ++i) {
                char arg_val = op.call.args[i];
                if (arg_val == 'L' || arg_val == 'R' || arg_val == 'C') {
                    new_env[f.params[i]] = arg_val;
                } else {
                    new_env[f.params[i]] = env[arg_val];
                }
            }
            run(string(1, op.call.func_name), new_env);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> M)) return 0;

    int start_r = -1, start_c = -1;
    for (int i = 0; i < N; i++) {
        cin >> grid[i];
        for (int j = 0; j < M; j++) {
            if (grid[i][j] == 'R') {
                start_r = i;
                start_c = j;
            }
        }
    }

    string prog_str = "";
    char ch;
    while (cin >> ch) {
        prog_str += ch;
    }

    p = parse(prog_str);

    r = start_r;
    c = start_c;
    dir = 0; // 0: N, 1: W, 2: S, 3: E

    run("m", map<char, char>());

    cout << ans.size() << "\n";
    for (size_t i = 0; i < ans.size(); i++) {
        cout << ans[i].first << " " << ans[i].second << "\n";
    }

    return 0;
}
