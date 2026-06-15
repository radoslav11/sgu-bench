#include<bits/stdc++.h>
using namespace std;

map<string, string> definitions;

string normalize(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

bool hasCycle(string from, string to, set<string>& visiting) {
    if(visiting.count(to)) return true;
    if(!isdigit(to[0])) {
        string normalized = normalize(to);
        if(definitions.count(normalized)) {
            visiting.insert(to);
            bool res = hasCycle(from, definitions[normalized], visiting);
            visiting.erase(to);
            return res;
        }
    }
    return false;
}

string substitute(string s);

long long parseExpression(string s);

long long parseNumber(string& s, int& pos) {
    while(pos < s.length() && isspace(s[pos])) pos++;
    
    if(pos < s.length() && (s[pos] == '+' || s[pos] == '-')) {
        char sign = s[pos];
        pos++;
        long long val = parseNumber(s, pos);
        return sign == '-' ? -val : val;
    }
    
    if(s[pos] == '(') {
        pos++;
        long long val = parseExpression(s.substr(pos));
        while(pos < s.length() && s[pos] != ')') pos++;
        pos++;
        return val;
    }
    
    long long num = 0;
    while(pos < s.length() && isdigit(s[pos])) {
        num = num * 10 + (s[pos] - '0');
        pos++;
    }
    return num;
}

long long parseExpression(string s) {
    s = substitute(s);
    
    function<long long(int&, int)> parse = [&](int& pos, int prec) -> long long {
        while(pos < s.length() && isspace(s[pos])) pos++;
        
        long long left;
        if(pos < s.length() && (s[pos] == '+' || s[pos] == '-')) {
            char op = s[pos++];
            left = parse(pos, 100);
            if(op == '-') left = -left;
        } else if(s[pos] == '(') {
            pos++;
            left = parse(pos, 0);
            pos++;
        } else {
            while(pos < s.length() && isdigit(s[pos])) {
                left = left * 10 + (s[pos++] - '0');
            }
        }
        
        while(pos < s.length() && isspace(s[pos])) pos++;
        
        while(pos < s.length() && s[pos] != ')') {
            char op = s[pos];
            int opPrec = 0;
            if(op == '+' || op == '-') opPrec = 1;
            else if(op == '*' || op == '/' || op == '%') opPrec = 2;
            else if(op == '^') opPrec = 3;
            else break;
            
            if(opPrec < prec) break;
            pos++;
            
            while(pos < s.length() && isspace(s[pos])) pos++;
            long long right = parse(pos, (op == '^') ? opPrec : opPrec + 1);
            
            if(op == '+') left = left + right;
            else if(op == '-') left = left - right;
            else if(op == '*') left = left * right;
            else if(op == '/') {
                long long absL = abs(left), absR = abs(right);
                long long res = absL / absR;
                if((left < 0) ^ (right < 0)) res = -res;
                left = res;
            }
            else if(op == '%') {
                long long absL = abs(left), absR = abs(right);
                long long res = absL % absR;
                if(left < 0) res = -res;
                left = res;
            }
            else if(op == '^') left = pow(left, right);
            
            while(pos < s.length() && isspace(s[pos])) pos++;
        }
        return left;
    };
    
    int pos = 0;
    return parse(pos, 0);
}

string substitute(string s) {
    string result;
    for(int i = 0; i < s.length(); i++) {
        if(isalpha(s[i]) || isdigit(s[i])) {
            string token;
            while(i < s.length() && (isalnum(s[i]))) {
                token += s[i++];
            }
            i--;
            
            string key = normalize(token);
            if(definitions.count(key)) {
                result += definitions[key];
            } else {
                result += token;
            }
        } else {
            result += s[i];
        }
    }
    return result;
}

int main(){
    string line;
    while(getline(cin, line)) {
        if(line.substr(0, 5) == "print") {
            cout << parseExpression(line.substr(5)) << "\n";
        } else if(line.substr(0, 6) == "define") {
            istringstream iss(line.substr(6));
            string op1, op2;
            iss >> op1 >> op2;
            string key1 = normalize(op1);
            if(!definitions.count(key1)) {
                set<string> visiting;
                if(!hasCycle(op1, op2, visiting)) {
                    definitions[key1] = op2;
                }
            }
        }
    }
    return 0;
}
