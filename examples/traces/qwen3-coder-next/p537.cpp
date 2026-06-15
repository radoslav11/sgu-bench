#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

long long gcd(long long a, long long b) {
    if (b == 0) 
        return a;
    return gcd(b, a % b);
}

int main() {
    int n;
    cin >> n;
    for (int caseIdx = 1; caseIdx <= n; caseIdx++) {
        string s;
        cin >> s;
        int len = s.size();

        map<char, long long> coeffMap;
        long long p = 1;
        for (int i = len - 1; i >= 0; i--) {
            coeffMap[s[i]] += p;
            p *= 10;
        }

        long long g = 0;
        for (auto &kv : coeffMap) {
            g = gcd(g, kv.second);
        }

        vector<long long> divisors;
        if (g == 0) {
            // In case of empty string (though problem states length >= 1)
            divisors.push_back(0);
        } else {
            for (long long i = 1; i * i <= g; i++) {
                if (g % i == 0) {
                    divisors.push_back(i);
                    if (i != g / i) {
                        divisors.push_back(g / i);
                    }
                }
            }
            sort(divisors.begin(), divisors.end());
        }

        cout << "Case " << caseIdx << ":";
        for (long long d : divisors) {
            cout << " " << d;
        }
        cout << endl;
    }
    return 0;
}
