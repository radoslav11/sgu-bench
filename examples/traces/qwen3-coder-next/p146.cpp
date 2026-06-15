#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstdio>
using namespace std;

int main() {
    double L;
    int N;
    cin >> L >> N;
    
    long long L_int = static_cast<long long>(round(L * 10000.0));
    long long total_int = 0;
    
    for (int i = 0; i < N; i++) {
        int Ti, Vi;
        cin >> Ti >> Vi;
        long long d = (long long)Ti * Vi;
        // Compute contribution modulo L_int
        total_int = (total_int + (d % L_int) * 10000LL) % L_int;
    }
    
    long long rem_int = total_int;
    long long ans_int = min(rem_int, L_int - rem_int);
    
    double ans = (double)ans_int / 10000.0;
    printf("%.4f\n", ans);
    
    return 0;
}
