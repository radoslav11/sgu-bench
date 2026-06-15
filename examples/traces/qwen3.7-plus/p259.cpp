#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Leaflet {
    int t;
    int l;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    if (!(cin >> n)) return 0;
    
    vector<Leaflet> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i].t;
    }
    for (int i = 0; i < n; ++i) {
        cin >> a[i].l;
    }
    
    sort(a.begin(), a.end(), [](const Leaflet& x, const Leaflet& y) {
        if (x.l != y.l) return x.l > y.l;
        return x.t > y.t;
    });
    
    int current_time = 0;
    int max_time = 0;
    for (int i = 0; i < n; ++i) {
        current_time += a[i].t;
        max_time = max(max_time, current_time + a[i].l);
    }
    
    cout << max_time << "\n";
    
    return 0;
}
