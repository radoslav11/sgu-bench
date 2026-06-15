#include<bits/stdc++.h>
using namespace std;

long long digitadition(long long n) {
    long long sum = n;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, K;
    cin >> N >> K;
    
    // Mark which numbers have generators
    vector<bool> hasGenerator(N + 1, false);
    
    // For each number i, mark d(i) as having a generator
    for (long long i = 1; i <= N; i++) {
        long long d = digitadition(i);
        if (d <= N) {
            hasGenerator[d] = true;
        }
    }
    
    // Collect all self-numbers
    vector<long long> selfNumbers;
    for (int i = 1; i <= N; i++) {
        if (!hasGenerator[i]) {
            selfNumbers.push_back(i);
        }
    }
    
    // Output count of self-numbers
    cout << selfNumbers.size() << "\n";
    
    // Output requested self-numbers
    for (int i = 0; i < K; i++) {
        int index;
        cin >> index;
        index--; // Convert to 0-based indexing
        if (i > 0) cout << " ";
        cout << selfNumbers[index];
    }
    cout << "\n";
    
    return 0;
}
