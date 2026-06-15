#include <iostream>
#include <string>
using namespace std;

int main() {
    string N;
    int K;
    cin >> N >> K;

    string res;
    for (char c : N) {
        while (K > 0 && !res.empty() && c > res.back()) {
            res.pop_back();
            K--;
        }
        res.push_back(c);
    }

    if (K > 0) {
        res.resize(res.size() - K);
    }

    cout << res << endl;
    return 0;
}
