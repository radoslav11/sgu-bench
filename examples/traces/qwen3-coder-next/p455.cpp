#include <iostream>
#include <map>
using namespace std;

int main() {
    long long A, B, C;
    cin >> A >> B >> C;

    const int MAX = 2000000;
    map<long long, int> first_occ;
    long long x = 1;
    first_occ[x] = 0;

    int best_p = MAX + 10;
    int best_q = MAX + 10;

    for (int i = 1; i <= MAX; i++) {
        x = (A * x + B) % C;
        if (first_occ.find(x) != first_occ.end()) {
            int p = first_occ[x];
            if (p < best_p) {
                best_p = p;
                best_q = i;
            }
        } else {
            first_occ[x] = i;
        }
    }

    if (best_q <= MAX) {
        cout << best_q << endl;
    } else {
        cout << -1 << endl;
    }

    return 0;
}
