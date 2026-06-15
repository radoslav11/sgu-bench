#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int N, M, Y;
    cin >> N >> M >> Y;
    
    vector<int> X(N);
    for (int i = 0; i < N; i++) {
        cin >> X[i];
    }
    
    vector<int> K(N, 0);
    long long sumK = 0;
    
    // Compute initial K[i] = floor(X[i] * M / Y)
    for (int i = 0; i < N; i++) {
        K[i] = (X[i] * M) / Y;
        sumK += K[i];
    }
    
    // Calculate remaining coins to distribute
    int remaining = M - sumK;
    
    // Create indices sorted by residual (X[i] * M - K[i] * Y) in descending order
    vector<int> indices(N);
    for (int i = 0; i < N; i++) {
        indices[i] = i;
    }
    
    // Sort by residual in descending order
    sort(indices.begin(), indices.end(), [&](int i, int j) {
        long long residual_i = 1LL * X[i] * M - 1LL * K[i] * Y;
        long long residual_j = 1LL * X[j] * M - 1LL * K[j] * Y;
        if (residual_i != residual_j) {
            return residual_i > residual_j;
        }
        // For tie-breaking, we can use any consistent order
        return i < j;
    });
    
    // Distribute remaining coins
    for (int i = 0; i < remaining; i++) {
        K[indices[i]]++;
    }
    
    // Output the result
    for (int i = 0; i < N; i++) {
        if (i > 0) cout << " ";
        cout << K[i];
    }
    cout << endl;
    
    return 0;
}
