#include<bits/stdc++.h>
using namespace std;

vector<int> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71};

long long computeN(vector<int>& factors) {
    // factors are (a₁+1), (a₂+1), ... 
    // we need to assign them to primes optimally
    // to minimize N, assign larger exponents to smaller primes
    
    vector<int> exponents;
    for(int f : factors) {
        exponents.push_back(f - 1);
    }
    sort(exponents.rbegin(), exponents.rend()); // sort in descending order
    
    long long result = 1;
    for(int i = 0; i < exponents.size(); i++) {
        for(int j = 0; j < exponents[i]; j++) {
            result *= primes[i];
            if(result > 1e15) return LLONG_MAX; // overflow protection
        }
    }
    return result;
}

void generateFactorizations(int k, int minFactor, vector<int>& current, vector<vector<int>>& allFactorizations) {
    if(k == 1) {
        allFactorizations.push_back(current);
        return;
    }
    
    for(int i = minFactor; i <= k; i++) {
        if(k % i == 0) {
            current.push_back(i);
            generateFactorizations(k / i, i, current, allFactorizations);
            current.pop_back();
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int K;
    cin >> K;
    
    vector<vector<int>> factorizations;
    vector<int> current;
    generateFactorizations(K, 2, current, factorizations);
    
    long long minN = LLONG_MAX;
    
    for(auto& factors : factorizations) {
        long long N = computeN(factors);
        minN = min(minN, N);
    }
    
    if(minN == LLONG_MAX) {
        cout << 0 << endl;
    } else {
        cout << minN << endl;
    }
    
    return 0;
}
