#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    if (cin >> n) {
        long long total_score = 0;
        for (int k = 1; k <= n; ++k) {
            if (k == 1) {
                total_score += 1;
            } else {
                long long r = k / 2;
                long long m_plus_2 = k - r + 1;
                long long d = r + 1;
                total_score += d * m_plus_2;
            }
        }
        
        cout << fixed << setprecision(3) << (double)total_score << "\n";
    }
    
    return 0;
}
