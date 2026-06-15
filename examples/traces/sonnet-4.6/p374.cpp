#include <bits/stdc++.h>
using namespace std;

int main() {
    long long a, b, k;
    cin >> a >> b >> k;
    long long base = a + b;
    long long result = 1;
    for (int i = 0; i < k; i++) {
        result *= base;
    }
    cout << result << endl;
    return 0;
}
