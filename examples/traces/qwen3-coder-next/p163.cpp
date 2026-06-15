#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;

    int k;
    cin >> k;

    vector<int> potentials(n);
    for (int i = 0; i < n; i++) {
        cin >> potentials[i];
    }

    long long result = 0;

    if (k == 2) {
        for (int x : potentials) {
            result += static_cast<long long>(x) * x;
        }
    } else {
        // k is 1 or 3 (both odd)
        for (int x : potentials) {
            if (x > 0) {
                long long term = 1;
                for (int i = 0; i < k; i++) {
                    term *= x;
                }
                result += term;
            }
        }
    }

    cout << result << endl;

    return 0;
}
