#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

map<string, string> vars;

void remove_spaces(string &s) {
    string res;
    res.reserve(s.length());
    for (char c : s) {
        if (!isspace((unsigned char)c)) {
            res += c;
        }
    }
    s = res;
}

struct SubstrCall {
    string var;
    int o;
    bool has_c;
    int c;
};

SubstrCall parse_substr(const string& s) {
    string temp = s;
    while (!temp.empty() && (temp.back() == ')' || temp.back() == ';')) {
        temp.pop_back();
    }
    size_t p1 = temp.find('(');
    string args = temp.substr(p1 + 1);
    
    SubstrCall res;
    res.has_c = false;
    
    vector<string> parts;
    size_t pos = 0;
    size_t prev = 0;
    while ((pos = args.find(',', prev)) != string::npos) {
        parts.push_back(args.substr(prev, pos - prev));
        prev = pos + 1;
    }
    parts.push_back(args.substr(prev));
    
    res.var = parts[0];
    res.o = stoi(parts[1]);
    if (parts.size() == 3) {
        res.has_c = true;
        res.c = stoi(parts[2]);
    }
    return res;
}

string eval_substr(const SubstrCall& call) {
    string s = vars.count(call.var) ? vars[call.var] : "";
    int len = s.length();
    int start = (call.o >= 0) ? call.o : len + call.o;
    int end;
    if (call.has_c) {
        if (call.c >= 0) {
            end = start + call.c;
        } else {
            end = len + call.c;
        }
    } else {
        end = len;
    }
    start = max(0, min(len, start));
    end = max(0, min(len, end));
    return s.substr(start, end - start);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M;
    if (!(cin >> N >> M)) return 0;
    
    string line;
    getline(cin, line);
    
    int init_count = 0;
    while (init_count < N && getline(cin, line)) {
        string temp = line;
        size_t first = temp.find_first_not_of(" \t\r\n");
        if (first == string::npos) continue;
        
        size_t eq = temp.find('=');
        if (eq == string::npos) continue;
        
        string name = temp.substr(0, eq);
        size_t start_name = name.find_first_not_of(" \t\r\n");
        size_t end_name = name.find_last_not_of(" \t\r\n");
        if (start_name != string::npos) {
            name = name.substr(start_name, end_name - start_name + 1);
        } else {
            name = "";
        }
        
        size_t q1 = temp.find('"', eq);
        size_t q2 = temp.find('"', q1 + 1);
        if (q1 != string::npos && q2 != string::npos) {
            string val = temp.substr(q1 + 1, q2 - q1 - 1);
            vars[name] = val;
            init_count++;
        }
    }
    
    int prog_count = 0;
    while (prog_count < M && getline(cin, line)) {
        string temp = line;
        size_t first = temp.find_first_not_of(" \t\r\n");
        if (first == string::npos) continue;
        
        remove_spaces(temp);
        prog_count++;
        line = temp;
        
        if (line.length() >= 6 && line.substr(0, 6) == "print(") {
            if (line.find("substr(") != string::npos) {
                size_t p1 = line.find("substr(");
                SubstrCall sc = parse_substr(line.substr(p1));
                cout << eval_substr(sc) << "\n";
            } else {
                size_t p1 = line.find('(');
                size_t p2 = line.find(')');
                string var = line.substr(p1 + 1, p2 - p1 - 1);
                cout << (vars.count(var) ? vars[var] : "") << "\n";
            }
        } else if (line.length() >= 7 && line.substr(0, 7) == "substr(") {
            size_t eq = line.find('=');
            string lhs = line.substr(0, eq);
            string rhs = line.substr(eq + 1);
            
            string val;
            if (rhs.find("substr(") != string::npos) {
                SubstrCall sc = parse_substr(rhs);
                val = eval_substr(sc);
            } else {
                string var = rhs.substr(0, rhs.length() - 1);
                val = vars.count(var) ? vars[var] : "";
            }
            
            SubstrCall lhs_call = parse_substr(lhs);
            string s = vars.count(lhs_call.var) ? vars[lhs_call.var] : "";
            int len = s.length();
            int start = (lhs_call.o >= 0) ? lhs_call.o : len + lhs_call.o;
            int end;
            if (lhs_call.has_c) {
                end = (lhs_call.c >= 0) ? start + lhs_call.c : len + lhs_call.c;
            } else {
                end = len;
            }
            start = max(0, min(len, start));
            end = max(0, min(len, end));
            vars[lhs_call.var] = s.substr(0, start) + val + s.substr(end);
        } else {
            size_t eq = line.find('=');
            string lhs = line.substr(0, eq);
            string rhs = line.substr(eq + 1);
            
            string val;
            if (rhs.find("substr(") != string::npos) {
                SubstrCall sc = parse_substr(rhs);
                val = eval_substr(sc);
            } else {
                string var = rhs.substr(0, rhs.length() - 1);
                val = vars.count(var) ? vars[var] : "";
            }
            vars[lhs] = val;
        }
    }
    
    return 0;
}
