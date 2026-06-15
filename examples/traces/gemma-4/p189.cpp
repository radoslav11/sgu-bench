#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

/**
 * Problem: Perl-like Substr
 * Implementing a simplified Perl string manipulation language.
 * Supports substr(string, offset, [count]) for both reading and replacing substrings.
 */

// Trim leading and trailing whitespace from a string
string trim(const string& s) {
    size_t first = s.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = s.find_last_not_of(" \t\n\r");
    return s.substr(first, (last - first + 1));
}

struct Expression {
    bool is_substr;
    string name;
    int offset;
    int count;
    bool has_count;
};

// Parse an expression which is either a variable name or a substr call
Expression parse_expression(string expr) {
    Expression e;
    expr = trim(expr);
    if (expr.size() >= 6 && expr.substr(0, 6) == "substr") {
        e.is_substr = true;
        size_t start_paren = expr.find('(');
        size_t end_paren = expr.find_last_of(')');
        string inside = expr.substr(start_paren + 1, end_paren - start_paren - 1);
        size_t first_comma = inside.find(',');
        e.name = trim(inside.substr(0, first_comma));
        string rest = inside.substr(first_comma + 1);
        size_t second_comma = rest.find(',');
        if (second_comma != string::npos) {
            e.has_count = true;
            e.offset = stoi(trim(rest.substr(0, second_comma)));
            e.count = stoi(trim(rest.substr(second_comma + 1)));
        } else {
            e.has_count = false;
            e.offset = stoi(trim(rest));
        }
    } else {
        e.is_substr = false;
        e.name = expr;
    }
    return e;
}

// Get the substring from a variable based on parameters
string get_substr(string var_name, int offset, int count, bool has_count, map<string, string>& vars) {
    string s = vars[var_name];
    int len = (int)s.length();
    int start_index;
    if (offset >= 0) start_index = offset;
    else start_index = len + offset;

    int end_index;
    if (!has_count) end_index = len;
    else if (count >= 0) end_index = start_index + count;
    else end_index = len + count;

    // Ensure indices are within logical bounds (as per problem guarantee)
    if (start_index < 0) start_index = 0;
    if (end_index > len) end_index = len;
    if (start_index >= end_index) return "";

    return s.substr(start_index, end_index - start_index);
}

// Replace a substring in a variable with a new string
void replace_substr(string var_name, int offset, int count, bool has_count, string new_val, map<string, string>& vars) {
    string s = vars[var_name];
    int len = (int)s.length();
    int start_index;
    if (offset >= 0) start_index = offset;
    else start_index = len + offset;

    int end_index;
    if (!has_count) end_index = len;
    else if (count >= 0) end_index = start_index + count;
    else end_index = len + count;

    if (start_index < 0) start_index = 0;
    if (end_index > len) end_index = len;
    if (start_index > end_index) start_index = end_index;

    string replaced = s.substr(0, start_index) + new_val + s.substr(end_index);
    vars[var_name] = replaced;
}

// Initialize variables from the first N lines
void init_variable(string line, map<string, string>& vars) {
    size_t eq_pos = line.find('=');
    if (eq_pos == string::npos) return;
    string name = trim(line.substr(0, eq_pos));
    size_t first_quote = line.find('"');
    size_t last_quote = line.find_last_of('"');
    if (first_quote != string::npos && last_quote != string::npos && first_quote < last_quote) {
        string value = line.substr(first_quote + 1, last_quote - first_quote - 1);
        vars[name] = value;
    } else {
        vars[name] = "";
    }
}

// Execute one command line
void execute(string line, map<string, string>& vars) {
    line = trim(line);
    if (line.empty()) return;
    if (line.back() == ';') line.pop_back();
    line = trim(line);

    if (line.substr(0, 5) == "print") {
        size_t start_paren = line.find('(');
        size_t end_paren = line.find_last_of(')');
        if (start_paren != string::npos && end_paren != string::npos) {
            string expr_str = line.substr(start_paren + 1, end_paren - start_paren - 1);
            Expression e = parse_expression(expr_str);
            if (e.is_substr) {
                cout << get_substr(e.name, e.offset, e.count, e.has_count, vars) << endl;
            } else {
                cout << vars[e.name] << endl;
            }
        }
    } else {
        size_t eq_pos = line.find('=');
        if (eq_pos != string::npos) {
            string left_str = line.substr(0, eq_pos);
            string right_str = line.substr(eq_pos + 1);
            Expression e1 = parse_expression(left_str);
            Expression e2 = parse_expression(right_str);

            if (e1.is_substr) {
                string val;
                if (e2.is_substr) {
                    val = get_substr(e2.name, e2.offset, e2.count, e2.has_count, vars);
                } else {
                    val = vars[e2.name];
                }
                replace_substr(e1.name, e1.offset, e1.count, e1.has_count, val, vars);
            } else {
                if (e2.is_substr) {
                    vars[e1.name] = get_substr(e2.name, e2.offset, e2.count, e2.has_count, vars);
                } else {
                    vars[e1.name] = vars[e2.name];
                }
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    string dummy;
    getline(cin, dummy); // consume the remaining newline after M

    map<string, string> vars;

    for (int i = 0; i < N; ++i) {
        string line;
        if (getline(cin, line)) {
            init_variable(line, vars);
        }
    }

    for (int i = 0; i < M; ++i) {
        string line;
        if (getline(cin, line)) {
            execute(line, vars);
        }
    }

    return 0;
}

