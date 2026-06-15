#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cctype>

using namespace std;

/**
 * Problem: 360. B++ Interpreter
 * Solution: Simulation of the robot's movement on an NxM grid based on 
 * a B++ program. The robot follows commands L, R, C and is controlled 
 * by functions with 0-2 arguments.
 * 
 * Complexity:
 *   Parsing: O(length of program)
 *   Simulation: O(number of commands executed, max 1000)
 *   The constraints N, M <= 50 and max commands = 1000 are well within time limits.
 */

int N, M;
vector<string> grid;
int currR, currC, currDir; // 0: UP, 1: RIGHT, 2: DOWN, 3: LEFT

int dr[] = {-1, 0, 1, 0};
int dc[] = {0, 1, 0, -1};

struct Arg {
    bool is_cmd;
    char val; // 'L', 'R', 'C' or uppercase argument name
};

struct Operator {
    int type; // 0: CMD (L/R/C), 1: CALL, 2: ARG (uppercase letter)
    char cmd;
    char func_name;
    vector<Arg> call_args;
    char arg_name;
};

struct Function {
    char name;
    vector<char> arg_names;
    vector<Operator> operators;
};

map<char, Function> functions;
string S;
int pos = 0;

// Parse argument names in a function definition (e.g., X, Y)
void parseArgs(vector<char>& arg_names) {
    while (pos < (int)S.size() && isupper(S[pos])) {
        arg_names.push_back(S[pos++]);
        if (pos < (int)S.size() && S[pos] == ',') {
            pos++;
        } else {
            break;
        }
    }
}

// Parse arguments in a function call (e.g., L, C or X, Y)
void parseCallArgs(vector<Arg>& args) {
    while (pos < (int)S.size() && S[pos] != ')') {
        if (S[pos] == 'L' || S[pos] == 'R' || S[pos] == 'C') {
            args.push_back({true, S[pos++]});
        } else if (isupper(S[pos])) {
            args.push_back({false, S[pos++]});
        }
        if (pos < (int)S.size() && S[pos] == ',') {
            pos++;
        }
    }
}

// Parse operators in a function body (L, R, C, func(args), or Arg)
void parseOperators(vector<Operator>& operators) {
    while (pos < (int)S.size() && S[pos] != '}') {
        if (S[pos] == 'L' || S[pos] == 'R' || S[pos] == 'C') {
            operators.push_back({0, S[pos++], ' ', {}, ' '});
        } else if (islower(S[pos])) {
            char f_name = S[pos++];
            pos++; // skip '('
            vector<Arg> args;
            parseCallArgs(args);
            pos++; // skip ')'
            operators.push_back({1, ' ', f_name, args, ' '});
        } else if (isupper(S[pos])) {
            operators.push_back({2, ' ', ' ', {}, S[pos++]});
        } else {
            pos++;
        }
    }
}

// Parse function definitions in the program
void parseFunctions() {
    while (pos < (int)S.size()) {
        if (islower(S[pos])) {
            Function f;
            f.name = S[pos++];
            pos++; // skip '('
            parseArgs(f.arg_names);
            pos++; // skip ')'
            pos++; // skip '{'
            parseOperators(f.operators);
            pos++; // skip '}'
            functions[f.name] = f;
        } else {
            pos++;
        }
    }
}

set<pair<int, int>> visited_stars;
vector<pair<int, int>> visited_order;

// Simulate robot command execution
void apply_command(char cmd) {
    if (cmd == 'L') {
        currDir = (currDir + 3) % 4;
    } else if (cmd == 'R') {
        currDir = (currDir + 1) % 4;
    } else if (cmd == 'C') {
        int nr = currR + dr[currDir];
        int nc = currC + dc[currDir];
        if (nr >= 0 && nr < N && nc >= 0 && nc < M) {
            currR = nr;
            currC = nc;
            if (grid[currR][currC] == '*') {
                if (visited_stars.find({currR + 1, currC + 1}) == visited_stars.end()) {
                    visited_stars.insert({currR + 1, currC + 1});
                    visited_order.push_back({currR + 1, currC + 1});
                }
            }
        }
    }
}

// Recursively execute functions
void execute(char func_name, vector<char> arg_values) {
    if (functions.find(func_name) == functions.end()) return;
    Function& f = functions[func_name];
    map<char, char> current_args;
    for (int i = 0; i < (int)f.arg_names.size(); ++i) {
        current_args[f.arg_names[i]] = arg_values[i];
    }

    for (auto& op : f.operators) {
        if (op.type == 0) {
            apply_command(op.cmd);
        } else if (op.type == 1) {
            vector<char> next_arg_values;
            for (auto& arg : op.call_args) {
                if (arg.is_cmd) {
                    next_arg_values.push_back(arg.val);
                } else {
                    next_arg_values.push_back(current_args[arg.val]);
                }
            }
            execute(op.func_name, next_arg_values);
        } else if (op.type == 2) {
            apply_command(current_args[op.arg_name]);
        }
    }
}

int main() {
    // Optimized I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read map dimensions
    if (!(cin >> N >> M)) return 0;

    // Read grid
    grid.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> grid[i];
        for (int j = 0; j < M; ++j) {
            if (grid[i][j] == 'R') {
                currR = i;
                currC = j;
                currDir = 0; // Initially facing upper border
            }
        }
    }

    // Read program, ignoring all whitespace
    string line;
    getline(cin, line); // consume newline
    char ch;
    S.reserve(10000);
    while (cin.get(ch)) {
        if (!isspace(ch)) S += ch;
    }

    // Parse and execute
    parseFunctions();
    execute('m', {});

    // Output the visited marked cells
    cout << visited_order.size() << "\n";
    for (auto& p : visited_order) {
        cout << p.first << " " << p.second << "\n";
    }

    return 0;
}

