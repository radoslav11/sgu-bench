#include<bits/stdc++.h>
using namespace std;

long long countThreeBits(long long start, long long end) {
    long long count = 0;
    for (int i = 0; i < 33; i++) {
        for (int j = i+1; j < 33; j++) {
            for (int k = j+1; k < 33; k++) {
                long long num = (1LL << i) | (1LL << j) | (1LL << k);
                if (num >= start && num <= end) count++;
            }
        }
    }
    return count;
}

int main(){
    int n;
    cin >> n;
    while(n--) {
        long long m;
        cin >> m;
        int cnt = 0;
        for (long long k = 1; k <= 33000000 && cnt <= 1; k++) {
            if (countThreeBits(k+1, 2*k) == m) cnt++;
        }
        cout << (cnt == 1 ? "YES" : "NO") << "\n";
    }
    return 0;
}
