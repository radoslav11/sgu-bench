#include<bits/stdc++.h>
using namespace std;

int n, m, k;
vector<long long> a;
vector<long long> products;

void generateCombinations(int start, int count, long long product) {
    if (count == m) {
        products.push_back(product);
        return;
    }
    
    for (int i = start; i < n; i++) {
        generateCombinations(i + 1, count + 1, product * a[i]);
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n >> m >> k;
    
    a.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    
    generateCombinations(0, 0, 1);
    
    sort(products.begin(), products.end(), greater<long long>());
    
    cout << products[k - 1] << endl;
    
    return 0;
}
