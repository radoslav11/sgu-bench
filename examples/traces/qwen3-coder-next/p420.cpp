#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <cctype>
#include <climits>
#include <iomanip>
#include <map>
#include <stack>
#include <queue>
#include <cstring>
using namespace std;

typedef long long ll;

vector<ll> generatePermutations2Digit(int a, int b) {
    vector<ll> res;
    res.push_back(10LL * a + b);
    res.push_back(10LL * b + a);
    sort(res.begin(), res.end());
    return res;
}

vector<ll> generatePermutations3DigitAA0(int a) {
    vector<ll> res;
    res.push_back(101LL * a); // a a 0 -> 100*a + 10*a + 0 = 110*a? Wait: digits are a,a,0
    // Actually: 100*a + 10*a + 0 = 110*a, and 100*a + 10*0 + a = 101*a
    res.push_back(110LL * a);
    sort(res.begin(), res.end());
    return res;
}

vector<ll> generatePermutations3DigitAB0(int a, int b) {
    vector<ll> res;
    // Valid permutations (no leading zero): a b 0, a 0 b, b a 0, b 0 a
    res.push_back(100LL * a + 10LL * b + 0);
    res.push_back(100LL * a + 10LL * 0 + b);
    res.push_back(100LL * b + 10LL * a + 0);
    res.push_back(100LL * b + 10LL * 0 + a);
    sort(res.begin(), res.end());
    return res;
}

vector<ll> generatePermutations3DigitAAB(int a, int b) {
    vector<ll> res;
    // Digits: a, a, b
    res.push_back(100LL * a + 10LL * a + b); // a a b
    res.push_back(100LL * a + 10LL * b + a); // a b a
    res.push_back(100LL * b + 10LL * a + a); // b a a
    sort(res.begin(), res.end());
    return res;
}

vector<ll> generatePermutations4DigitAAA0(int a) {
    vector<ll> res;
    // Digits: a, a, a, 0
    // Valid: 0 in position 2,3,4
    res.push_back(1000LL * a + 100LL * a + 10LL * a + 0); // a a a 0
    res.push_back(1000LL * a + 100LL * a + 10LL * 0 + a); // a a 0 a
    res.push_back(1000LL * a + 100LL * 0 + 10LL * a + a); // a 0 a a
    sort(res.begin(), res.end());
    return res;
}

int main() {
    ll l, r;
    cin >> l >> r;
    
    ll ans = 0;
    vector<vector<ll>> allLists;
    
    // 2-digit: {a, b} a < b, a,b in 1..9
    for (int a = 1; a <= 9; a++) {
        for (int b = a + 1; b <= 9; b++) {
            allLists.push_back(generatePermutations2Digit(a, b));
        }
    }
    
    // 3-digit: {a, a, 0} a in 1..9
    for (int a = 1; a <= 9; a++) {
        allLists.push_back(generatePermutations3DigitAA0(a));
    }
    
    // 3-digit: {a, b, 0} a < b, a,b in 1..9
    for (int a = 1; a <= 9; a++) {
        for (int b = a + 1; b <= 9; b++) {
            allLists.push_back(generatePermutations3DigitAB0(a, b));
        }
    }
    
    // 3-digit: {a, a, b} a in 1..9, b in 1..9, b != a
    for (int a = 1; a <= 9; a++) {
        for (int b = 1; b <= 9; b++) {
            if (b != a) {
                allLists.push_back(generatePermutations3DigitAAB(a, b));
            }
        }
    }
    
    // 4-digit: {a, a, a, 0} a in 1..9
    for (int a = 1; a <= 9; a++) {
        allLists.push_back(generatePermutations4DigitAAA0(a));
    }
    
    // For each list, count how many in [l, r]
    for (auto& list : allLists) {
        // Count numbers in list that are in [l, r]
        auto it1 = lower_bound(list.begin(), list.end(), l);
        auto it2 = upper_bound(list.begin(), list.end(), r);
        int count = distance(it1, it2);
        if (count == 2) {
            ans += 2;
        }
    }
    
    cout << ans << endl;
    
    return 0;
}
