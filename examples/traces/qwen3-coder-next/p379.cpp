#include <iostream>
#include <vector>
using namespace std;

bool check(long long x, const vector<long long>& B, long long C, long long P, long long T) {
    if (x == 0) 
        return true;
    long long time_needed = 0;
    int N = B.size() - 1;
    for (int i = 1; i <= N; i++) {
        long long rem = x - B[i-1];
        if (rem <= 0) 
            break;
        long long terms_i = (rem + C - 1) / C;
        time_needed += 2LL * P * terms_i;
        if (time_needed > T) 
            break;
    }
    return time_needed <= T;
}

int main() {
    long long N, C, P, T_val;
    cin >> N >> C >> P >> T_val;
    vector<long long> A(N);
    for (int i = 0; i < N; i++) {
        cin >> A[i];
    }

    vector<long long> B(N+1);
    B[0] = 0;
    for (int i = 1; i <= N; i++) {
        B[i] = B[i-1] + A[i-1];
    }

    long long total_people = B[N];
    long long lo = 0, hi = total_people;
    long long ans = 0;

    while (lo <= hi) {
        long long mid = (lo + hi) / 2;
        if (check(mid, B, C, P, T_val)) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    cout << ans << endl;
    return 0;
}
