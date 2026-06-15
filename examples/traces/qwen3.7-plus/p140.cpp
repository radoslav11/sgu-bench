#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    long long p, b;
    if (!(cin >> n >> p >> b)) return 0;

    vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        a[i] %= p;
    }

    vector<long long> g(n + 1);
    g[0] = p;
    for (int i = 1; i <= n; ++i) {
        g[i] = std::gcd(g[i - 1], a[i - 1]);
    }

    if (b % g[n] != 0) {
        cout << "NO\n";
        return 0;
    }

    cout << "YES\n";
    vector<long long> x(n);
    long long r = b % p;

    for (int i = n; i >= 1; --i) {
        long long target = r % g[i - 1];
        for (long long xi = 0; xi < p; ++xi) {
            if ((a[i - 1] * xi) % g[i - 1] == target) {
                x[i - 1] = xi;
                r = (r - a[i - 1] * xi) % p;
                if (r < 0) {
                    r += p;
                }
                break;
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        cout << x[i] << (i == n - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}
