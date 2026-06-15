#include <iostream>
using namespace std;

int main() {
    int K;
    cin >> K;

    long long a = 1, b = 1, sum = 0;

    for (int i = 1; i <= K; ++i) {
        sum += a;
        long long c = a + b;
        a = b;
        b = c;
    }

    cout << sum << '\n';
    return 0;
}
