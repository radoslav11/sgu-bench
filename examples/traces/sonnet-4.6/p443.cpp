#include <bits/stdc++.h>
using namespace std;

int keyNumber(int n) {
    vector<int> factors;
    for (int i = 2; (long long)i * i <= n; i++) {
        if (n % i == 0) {
            factors.push_back(i);
            while (n % i == 0) n /= i;
        }
    }
    if (n > 1) factors.push_back(n);
    
    int largest = factors.back();
    int sumOthers = 0;
    for (int i = 0; i < (int)factors.size() - 1; i++) {
        sumOthers += factors[i];
    }
    return largest - sumOthers;
}

int main() {
    int a, b;
    cin >> a >> b;
    int ka = keyNumber(a);
    int kb = keyNumber(b);
    if (ka > kb) cout << "a" << endl;
    else cout << "b" << endl;
    return 0;
}
