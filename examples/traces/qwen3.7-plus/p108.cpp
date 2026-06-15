#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>

using namespace std;

const int MAXN = 10000005;
bitset<MAXN> is_not_self;
int sum_digits[10000];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<pair<int, int>> s(k);
    for (int i = 0; i < k; ++i) {
        cin >> s[i].first;
        s[i].second = i;
    }
    sort(s.begin(), s.end());
    
    for (int i = 0; i < 10000; ++i) {
        int num = i;
        int sum = 0;
        while (num > 0) {
            sum += num % 10;
            num /= 10;
        }
        sum_digits[i] = sum;
    }

    int total_self = 0;
    vector<int> ans(k);
    int query_idx = 0;

    for (int i = 1; i <= n; ++i) {
        if (!is_not_self[i]) {
            total_self++;
            while (query_idx < k && s[query_idx].first == total_self) {
                ans[s[query_idx].second] = i;
                query_idx++;
            }
        }
        
        int d = i + sum_digits[i % 10000] + sum_digits[i / 10000];
        if (d <= n) {
            is_not_self[d] = true;
        }
    }

    cout << total_self << "\n";
    for (int i = 0; i < k; ++i) {
        if (i > 0) cout << " ";
        cout << ans[i];
    }
    cout << "\n";

    return 0;
}
