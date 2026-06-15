#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    long long N;
    cin >> N;

    if (N % 2 == 0) {
        cout << "No solution" << endl;
        return 0;
    }

    if (N == 1) {
        cout << 0 << '\n';
        cout << '\n';
        return 0;
    }

    vector<int> operations;
    long long cur = N;

    while (cur != 1) {
        if (cur % 4 == 1) {
            operations.push_back(1);
            cur = (cur + 1) / 2;
        } else {
            operations.push_back(2);
            cur = (cur - 1) / 2;
        }
    }

    reverse(operations.begin(), operations.end());

    cout << operations.size() << '\n';
    for (int i = 0; i < operations.size(); i++) {
        if (i > 0) 
            cout << " ";
        cout << operations[i];
    }
    cout << '\n';

    return 0;
}
