#include <bits/stdc++.h>
using namespace std;

int main() {
    long long N, M;
    cin >> N >> M;
    
    double prob_not_chosen = pow((double)(N - 1) / N, M);
    double expected = N * (1.0 - prob_not_chosen);
    
    cout << fixed << setprecision(9) << expected << endl;
    
    return 0;
}
