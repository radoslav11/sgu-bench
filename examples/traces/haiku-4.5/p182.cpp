#include<bits/stdc++.h>
using namespace std;

string expr;
int pos;

struct Expr {
    char op;
    vector<Expr*> children;
    char var;
};

Expr* parseExpr();
Expr* parseOr();
Expr* parseAnd();
Expr* parseNeg();
Expr* parsePrimary();

Expr* parsePrimary() {
    if(expr[pos] == '(') {
        pos++;
        Expr* e = parseExpr();
        pos++;
        return e;
    }
    Expr* e = new Expr();
    e->op = 'v';
    e->var = expr[pos++];
    return e;
}

Expr* parseNeg() {
    if(expr[pos] == '!') {
        pos++;
        Expr* e = new Expr();
        e->op = '!';
        e->children.push_back(parseNeg());
        return e;
    }
    return parsePrimary();
}

Expr* parseAnd() {
    Expr* left = parseNeg();
    while(pos < expr.length() && expr[pos] == '&') {
        pos++;
        Expr* right = parseNeg();
        Expr* e = new Expr();
        e->op = '&';
        e->children.push_back(left);
        e->children.push_back(right);
        left = e;
    }
    return left;
}

Expr* parseOr() {
    Expr* left = parseAnd();
    while(pos < expr.length() && (expr[pos] == '|' || expr[pos] == '=' || expr[pos] == '<' || expr[pos] == '#')) {
        char op = expr[pos];
        if(op == '|') pos += 2;
        else if(op == '=') pos += 2;
        else if(op == '<') pos += 3;
        else pos++;
        
        Expr* right = parseAnd();
        Expr* e = new Expr();
        e->op = op;
        e->children.push_back(left);
        e->children.push_back(right);
        left = e;
    }
    return left;
}

Expr* parseExpr() {
    return parseOr();
}

string toString(Expr* e) {
    if(e->op == 'v') return string(1, e->var);
    if(e->op == '!') return "!" + toString(e->children[0]);
    
    string left = toString(e->children[0]);
    string right = toString(e->children[1]);
    
    if(e->op == '&') return left + "&" + right;
    if(e->op == '|') return left + "||" + right;
    if(e->op == '=') return left + "=>" + right;
    if(e->op == '<') return left + "<=>" + right;
    if(e->op == '#') return left + "#" + right;
    return "";
}

int main(){
    cin >> expr;
    pos = 0;
    Expr* tree = parseExpr();
    cout << toString(tree) << endl;
    return 0;
}
