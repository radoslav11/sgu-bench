#include <iostream>
#include <string>
#include <stack>
using namespace std;

int main() {
    string start, s;
    getline(cin, start);
    getline(cin, s);

    stack<char> stk;
    for (char c : s) {
        if (!stk.empty() && stk.top() != c) {
            stk.pop();
        } else {
            stk.push(c);
        }
    }

    if (stk.size() == 1 && stk.top() == start[0]) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }

    return 0;
}
