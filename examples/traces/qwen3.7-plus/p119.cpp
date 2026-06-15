#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int gcd(int a, int b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, a0, b0;
    if (!(cin >> n >> a0 >> b0)) return 0;
    
    a0 %= n;
    b0 %= n;
    
    int g = gcd(a0, gcd(b0, n));
    int num_pairs = n / g;
    
    vector<pair<int, int>> pairs;
    pairs.reserve(num_pairs);
    
    for (int k = 0; k < num_pairs; ++k) {
        int a = (k * a0) % n;
        int b = (k * b0) % n;
        pairs.emplace_back(a, b);
    }
    
    sort(pairs.begin(), pairs.end());
    
    cout << num_pairs << "\n";
    for (const auto& p : pairs) {
        cout << p.first << " " << p.second << "\n";
    }
    
    return 0;
}
