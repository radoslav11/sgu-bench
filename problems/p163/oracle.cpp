#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    int p;
    cin >> p;

    vector<int> x(N);
    for(int i = 0; i < N; i++) {
        cin >> x[i];
    }

    long long ans = 0;
    for(int i = 0; i < N; i++) {
        int v = x[i];
        int vp;
        if(p == 1) {
            vp = v;
        } else if(p == 2) {
            vp = v * v;
        } else {
            vp = v * v * v;
        }
        if(vp > 0) {
            ans += vp;
        }
    }

    cout << ans << "\n";
    return 0;
}
