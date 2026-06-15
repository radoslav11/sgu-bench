#include <iostream>

using namespace std;

const int MAXN = 500005;
long long q1[MAXN];
long long q2[MAXN];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    if (!(cin >> n)) return 0;
    
    for (int i = 0; i < n; ++i) {
        cin >> q1[i];
    }
    
    int head1 = 0, tail1 = n;
    int head2 = 0, tail2 = 0;
    
    long long total_sum = 0;
    for (int i = 0; i < n - 1; ++i) {
        long long x, y;
        
        if (head2 == tail2 || (head1 < tail1 && q1[head1] <= q2[head2])) {
            x = q1[head1++];
        } else {
            x = q2[head2++];
        }
        
        if (head2 == tail2 || (head1 < tail1 && q1[head1] <= q2[head2])) {
            y = q1[head1++];
        } else {
            y = q2[head2++];
        }
        
        long long z = x + y;
        total_sum += z;
        q2[tail2++] = z;
    }
    
    cout << total_sum << "\n";
    
    return 0;
}
