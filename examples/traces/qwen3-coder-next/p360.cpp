#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cctype>
#include <algorithm>
#include <sstream>

using namespace std;

struct Robot {
    int r, c;
    int dir; // 0: up, 1: right, 2: down, 3: left
    Robot(int r, int c) : r(r), c(c), dir(0) {}
    
    void move(int n, int m) {
        int dr[] = {-1, 0, 1, 0};
        int dc[] = {0, 1, 0, -1};
        int nr = r + dr[dir] * n;
        int nc = c + dc[dir] * n;
        if (nr >= 0 && nr < n && nc >= 0 && nc < m) {
            r = nr;
            c = nc;
        }
    }
    
    void turnLeft() {
        dir = (dir + 3) % 4;
    }
    
    void turnRight() {
        dir = (dir + 1) % 4;
    }
};

struct Function {
    vector<char> args;
    vector<string> operators;
};

vector<string> tokenize(const string& s) {
    vector<string> tokens;
    string token;
    for (char c : s) {
        if (isspace(c)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int N, M;
    cin >> N >> M;
    cin.ignore();
    
    vector<string> grid(N);
    int start_r = -1, start_c = -1;
    
    for (int i = 0; i < N; i++) {
        getline(cin, grid[i]);
        for (int j = 0; j < M; j++) {
            if (grid[i][j] == 'R') {
                start_r = i;
                start_c = j;
                grid[i][j] = '~';
            }
        }
    }
    
    // Read the rest of the input (program)
    string program = "";
    string line;
    while (getline(cin, line)) {
        program += line + " ";
    }
    
    // Tokenize the program by removing whitespace and parsing functions
    // We'll parse manually by scanning the string
    
    // Map: function name -> Function struct
    map<char, Function> functions;
    
    // We need to parse the program string to extract functions
    // Remove all whitespace for easier parsing
    string clean_program = "";
    for (char c : program) {
        if (!isspace(c)) {
            clean_program += c;
        }
    }
    
    // Parse clean_program for function definitions
    size_t pos = 0;
    auto peek = [&](size_t offset = 0) -> char {
        return (pos + offset < clean_program.size()) ? clean_program[pos + offset] : '\0';
    };
    auto consume = [&](size_t cnt = 1) {
        pos += cnt;
    };
    
    while (pos < clean_program.size()) {
        // Expect: [func_name]([args]){[body]}
        if (!islower(clean_program[pos])) break;
        char func_name = clean_program[pos++];
        
        if (clean_program[pos] != '(') break;
        pos++; // consume '('
        
        Function func;
        
        // Parse arguments
        if (isupper(clean_program[pos]) && clean_program[pos] != 'L' && clean_program[pos] != 'R' && clean_program[pos] != 'C') {
            func.args.push_back(clean_program[pos++]);
            
            while (clean_program[pos] == ',') {
                pos++; // consume ','
                if (isupper(clean_program[pos]) && clean_program[pos] != 'L' && clean_program[pos] != 'R' && clean_program[pos] != 'C') {
                    func.args.push_back(clean_program[pos++]);
                }
            }
        }
        
        if (clean_program[pos] != ')') break;
        pos++; // consume ')'
        
        if (clean_program[pos] != '{') break;
        pos++; // consume '{'
        
        // Parse body until '}'
        int brace_count = 1;
        string body = "";
        while (pos < clean_program.size() && brace_count > 0) {
            if (clean_program[pos] == '{') brace_count++;
            else if (clean_program[pos] == '}') brace_count--;
            if (brace_count > 0) {
                body += clean_program[pos];
            }
            pos++;
        }
        
        // Now parse body into operators
        // Each operator is either:
        // - single char: L, R, C
        // - function call: [a-z]([args])
        // - argument: [A-Z] (but only if it's an argument of current function)
        
        // We'll extract operators one by one
        size_t body_pos = 0;
        while (body_pos < body.size()) {
            string op = "";
            
            // If it's a letter (could be function or argument)
            if (islower(body[body_pos])) {
                // Function call
                op += body[body_pos++];
                if (body_pos < body.size() && body[body_pos] == '(') {
                    op += body[body_pos++];
                    int par_count = 1;
                    while (body_pos < body.size() && par_count > 0) {
                        if (body[body_pos] == '(') par_count++;
                        else if (body[body_pos] == ')') par_count--;
                        op += body[body_pos++];
                    }
                }
            } else if (isupper(body[body_pos])) {
                // Could be an argument or L/R/C? But L/R/C are lowercase
                op += body[body_pos++];
            } else {
                // Should not happen, but skip
                body_pos++;
                continue;
            }
            
            func.operators.push_back(op);
        }
        
        functions[func_name] = func;
    }
    
    // Now simulate the robot execution
    Robot robot(start_r, start_c);
    map<pair<int, int>, bool> visited; // for marking cells visited (first time)
    vector<pair<int, int>> visit_order;
    
    // Helper: get current cell as (r, c) and mark if it's a '*' and not visited
    auto check_and_mark = [&]() {
        if (grid[robot.r][robot.c] == '*' && !visited.count({robot.r, robot.c})) {
            visited[{robot.r, robot.c}] = true;
            visit_order.push_back({robot.r, robot.c});
        }
    };
    
    // Now execute the main function m()
    // We'll use a stack-based approach for recursion (since we know it's finite and depth is limited)
    // But since max 1000 commands, recursion depth is small, we can use recursion
    
    // Map to store argument values for current function call
    map<char, string> arg_values; // arg name -> string (which is either "L", "R", or "C")
    
    // Recursive function to execute a function
    function<void(const Function&, map<char, string>&)> execute_func;
    
    execute_func = [&](const Function& func, map<char, string>& current_args) {
        for (const string& op : func.operators) {
            // Check if op is a function call or single command
            if (op.size() == 1) {
                char cmd = op[0];
                if (islower(cmd)) {
                    // This is a function call without arguments? But functions always have parens
                    // So this shouldn't happen in valid programs
                    continue;
                } else if (isupper(cmd)) {
                    // Argument reference
                    if (current_args.count(cmd)) {
                        string arg_cmd = current_args[cmd];
                        if (arg_cmd == "L") robot.turnLeft();
                        else if (arg_cmd == "R") robot.turnRight();
                        else if (arg_cmd == "C") robot.move(1, N, M);
                        check_and_mark();
                    }
                    // else: ignore (shouldn't happen)
                } else {
                    // Command: L, R, C
                    if (cmd == 'L') robot.turnLeft();
                    else if (cmd == 'R') robot.turnRight();
                    else if (cmd == 'C') {
                        robot.move(1, N, M);
                        check_and_mark();
                    }
                }
            } else {
                // Function call: starts with lowercase, then '(' and args
                if (islower(op[0])) {
                    char func_name = op[0];
                    // Find the function
                    if (functions.find(func_name) == functions.end()) continue;
                    const Function& callee = functions[func_name];
                    
                    // Parse arguments from op string: from '(' to ')'
                    size_t start = op.find('(');
                    size_t end = op.rfind(')');
                    string args_str = op.substr(start + 1, end - start - 1);
                    
                    // Split args_str by commas (if any)
                    vector<string> actual_args;
                    if (!args_str.empty()) {
                        stringstream ss(args_str);
                        string token;
                        while (getline(ss, token, ',')) {
                            actual_args.push_back(token);
                        }
                    }
                    
                    // Map actual args to formal args
                    map<char, string> new_args;
                    for (size_t i = 0; i < callee.args.size(); i++) {
                        if (i < actual_args.size()) {
                            new_args[callee.args[i]] = actual_args[i];
                        }
                    }
                    
                    // Recursively execute
                    execute_func(callee, new_args);
                }
            }
        }
    };
    
    // Start with function m
    if (functions.find('m') != functions.end()) {
        map<char, string> empty_args;
        execute_func(functions['m'], empty_args);
    }
    
    // Output
    cout << visit_order.size() << "\n";
    for (auto& p : visit_order) {
        // Convert to 1-indexed
        cout << p.first + 1 << " " << p.second + 1 << "\n";
    }
    
    return 0;
}
