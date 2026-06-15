#include <bits/stdc++.h>
using namespace std;

struct Tok {
    string s;
    int type; // 0 ident/word, 1 literal, 2 symbol
};

string low(string s) {
    for (char &c : s) c = (char)tolower((unsigned char)c);
    return s;
}

vector<Tok> toks;
int p;

struct Elem {
    bool term;
    string val;
};

using Prod = vector<Elem>;
using Alts = vector<Prod>;

Alts concatAlts(const Alts &a, const Alts &b) {
    Alts r;
    for (auto &x : a) {
        for (auto &y : b) {
            Prod z = x;
            z.insert(z.end(), y.begin(), y.end());
            r.push_back(z);
        }
    }
    return r;
}

bool isWord(const string &w) {
    return p < (int)toks.size() && toks[p].type == 0 && toks[p].s == w;
}

void expectWord(const string &w) {
    if (!isWord(w)) exit(0);
    p++;
}

void expectSym(const string &w) {
    if (p >= (int)toks.size() || toks[p].s != w) exit(0);
    p++;
}

Alts parseSeq();

Alts parseIf() {
    Alts res;
    bool hasErrorElse = false;

    while (true) {
        expectWord("if");
        expectWord("peek");
        expectSym("=");

        if (p >= (int)toks.size() || toks[p].type != 1) exit(0);
        string ch = toks[p].s;
        p++;

        expectWord("then");
        expectWord("begin");
        expectWord("skip");
        expectSym(";");

        Alts body = parseSeq();

        expectWord("end");

        for (auto prod : body) {
            Prod cur;
            cur.push_back({true, ch});
            cur.insert(cur.end(), prod.begin(), prod.end());
            res.push_back(cur);
        }

        if (isWord("else")) {
            p++;
            if (isWord("if")) {
                continue;
            } else if (isWord("error")) {
                p++;
                expectSym(";");
                hasErrorElse = true;
                break;
            } else {
                exit(0);
            }
        } else {
            break;
        }
    }

    if (!hasErrorElse) {
        res.push_back(Prod());
    }

    return res;
}

Alts parseSegment() {
    if (isWord("if")) {
        return parseIf();
    } else {
        if (p >= (int)toks.size() || toks[p].type != 0) exit(0);
        string name = toks[p].s;
        p++;
        expectSym(";");

        Prod pr;
        pr.push_back({false, name});
        return Alts{pr};
    }
}

Alts parseSeq() {
    Alts cur(1);
    cur[0] = Prod();

    while (p < (int)toks.size() && !isWord("end")) {
        Alts seg = parseSegment();
        cur = concatAlts(cur, seg);
    }

    return cur;
}

string renderProd(const Prod &pr) {
    string res;
    string lit;

    auto flush = [&]() {
        if (!lit.empty()) {
            res += "'";
            res += lit;
            res += "'";
            lit.clear();
        }
    };

    for (auto &e : pr) {
        if (e.term) {
            lit += e.val;
        } else {
            flush();
            res += "<" + e.val + ">";
        }
    }
    flush();
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string input((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());

    for (int i = 0; i < (int)input.size(); ) {
        unsigned char c = input[i];

        if (isspace(c)) {
            i++;
        } else if (isalpha(c)) {
            string w;
            while (i < (int)input.size() && isalpha((unsigned char)input[i])) {
                w += (char)tolower((unsigned char)input[i]);
                i++;
            }
            toks.push_back({w, 0});
        } else if (input[i] == '\'') {
            i++;
            string lit;
            while (i < (int)input.size() && input[i] != '\'') {
                lit += input[i++];
            }
            if (i < (int)input.size() && input[i] == '\'') i++;
            toks.push_back({lit, 1});
        } else {
            toks.push_back({string(1, input[i]), 2});
            i++;
        }
    }

    set<string> routines;

    for (int i = 0; i + 3 < (int)toks.size(); i++) {
        if (toks[i].type == 0 && toks[i].s == "var") break;

        if (toks[i].type == 0 && toks[i].s == "procedure" &&
            toks[i + 1].type == 0 &&
            toks[i + 2].s == ";" &&
            toks[i + 3].type == 0 && toks[i + 3].s == "forward") {
            string name = toks[i + 1].s;
            if (name != "skip" && name != "error")
                routines.insert(name);
        }
    }

    map<string, vector<string>> grammar;

    for (int i = 0; i + 2 < (int)toks.size(); i++) {
        if (toks[i].type == 0 && toks[i].s == "var") break;

        if (toks[i].type == 0 && toks[i].s == "procedure" &&
            toks[i + 1].type == 0 &&
            routines.count(toks[i + 1].s) &&
            toks[i + 2].s == ";") {

            if (i + 3 < (int)toks.size() && toks[i + 3].type == 0 && toks[i + 3].s == "forward")
                continue;

            string name = toks[i + 1].s;
            p = i + 3;

            expectWord("begin");
            Alts alts = parseSeq();
            expectWord("end");
            expectSym(";");

            vector<string> out;
            for (auto &pr : alts) out.push_back(renderProd(pr));

            sort(out.begin(), out.end());
            grammar[name] = out;
        }
    }

    for (auto &kv : grammar) {
        cout << "<" << kv.first << ">::=";
        for (int i = 0; i < (int)kv.second.size(); i++) {
            if (i) cout << "|";
            cout << kv.second[i];
        }
        cout << "\n";
    }

    return 0;
}
