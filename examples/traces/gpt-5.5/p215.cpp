#include <bits/stdc++.h>
using namespace std;

using ll = long long;

static bool isNumberToken(const string& s) {
    return !s.empty() && isdigit((unsigned char)s[0]);
}

static string canonicalToken(string s) {
    if (isNumberToken(s)) {
        int i = 0;
        while (i + 1 < (int)s.size() && s[i] == '0') i++;
        return s.substr(i);
    } else {
        for (char &c : s) c = (char)tolower((unsigned char)c);
        return s;
    }
}

struct Parser {
    string s;
    int pos = 0;
    unordered_map<string, string>& defs;

    Parser(const string& str, unordered_map<string, string>& d) : s(str), defs(d) {}

    void skipSpaces() {
        while (pos < (int)s.size() && isspace((unsigned char)s[pos])) pos++;
    }

    ll resolve(string token) {
        token = canonicalToken(token);

        while (defs.count(token)) {
            token = defs[token];
        }

        if (isNumberToken(token)) return stoll(token);
        return 0;
    }

    ll power(ll a, ll b) {
        ll res = 1;
        while (b > 0) {
            if (b & 1) res *= a;
            a *= a;
            b >>= 1;
        }
        return res;
    }

    ll parseExpression() {
        return parseAddSub();
    }

    ll parseAddSub() {
        ll val = parseMulDivMod();

        while (true) {
            skipSpaces();
            if (pos >= (int)s.size()) break;

            char op = s[pos];
            if (op != '+' && op != '-') break;
            pos++;

            ll rhs = parseMulDivMod();
            if (op == '+') val += rhs;
            else val -= rhs;
        }

        return val;
    }

    ll parseMulDivMod() {
        ll val = parsePower();

        while (true) {
            skipSpaces();
            if (pos >= (int)s.size()) break;

            char op = s[pos];
            if (op != '*' && op != '/' && op != '%') break;
            pos++;

            ll rhs = parsePower();

            if (op == '*') {
                val *= rhs;
            } else if (op == '/') {
                ll sign = ((val < 0) ^ (rhs < 0)) ? -1 : 1;
                val = sign * (llabs(val) / llabs(rhs));
            } else {
                ll sign = ((val < 0) ^ (rhs < 0)) ? -1 : 1;
                val = sign * (llabs(val) % llabs(rhs));
            }
        }

        return val;
    }

    ll parsePower() {
        ll val = parseUnary();

        skipSpaces();
        if (pos < (int)s.size() && s[pos] == '^') {
            pos++;
            ll exp = parsePower();
            val = power(val, exp);
        }

        return val;
    }

    ll parseUnary() {
        skipSpaces();

        if (pos < (int)s.size() && s[pos] == '+') {
            pos++;
            return parseUnary();
        }

        if (pos < (int)s.size() && s[pos] == '-') {
            pos++;
            return -parseUnary();
        }

        return parsePrimary();
    }

    ll parsePrimary() {
        skipSpaces();

        if (pos < (int)s.size() && s[pos] == '(') {
            pos++;
            ll val = parseExpression();
            skipSpaces();
            if (pos < (int)s.size() && s[pos] == ')') pos++;
            return val;
        }

        if (pos < (int)s.size() && isdigit((unsigned char)s[pos])) {
            string token;
            while (pos < (int)s.size() && isdigit((unsigned char)s[pos])) {
                token += s[pos++];
            }
            return resolve(token);
        }

        string token;
        while (pos < (int)s.size() && isalnum((unsigned char)s[pos])) {
            token += s[pos++];
        }

        return resolve(token);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unordered_map<string, string> defs;
    string line;

    while (getline(cin, line)) {
        if (line.empty()) continue;

        if (line.rfind("print", 0) == 0) {
            string expr = line.substr(5);
            Parser parser(expr, defs);
            cout << parser.parseExpression() << '\n';
        } else if (line.rfind("define", 0) == 0) {
            string rest = line.substr(6);
            stringstream ss(rest);
            string a, b;
            ss >> a >> b;

            a = canonicalToken(a);
            b = canonicalToken(b);

            if (defs.count(a)) continue;

            bool circular = false;
            string cur = b;

            while (true) {
                if (cur == a) {
                    circular = true;
                    break;
                }

                auto it = defs.find(cur);
                if (it == defs.end()) break;
                cur = it->second;
            }

            if (!circular) {
                defs[a] = b;
            }
        }
    }

    return 0;
}
