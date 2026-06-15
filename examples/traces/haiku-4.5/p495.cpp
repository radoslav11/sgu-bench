#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long N, M;
    cin >> N >> M;
    
    double prob_never_picked = pow((double)(N - 1) / N, M);
    double expected_prizes = N * (1.0 - prob_never_picked);
    
    cout << fixed << setprecision(15) << expected_prizes << endl;
    
    return 0;
}
