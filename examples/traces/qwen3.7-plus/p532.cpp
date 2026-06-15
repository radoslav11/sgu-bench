#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct HSeg {
    long long y;
    long long x_min;
    long long x_max;
};

struct VSeg {
    long long x;
    long long y_min;
    long long y_max;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    if (!(cin >> n)) return 0;
    
    vector<HSeg> H;
    vector<VSeg> V;
    
    for (int i = 0; i < n; ++i) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        if (y1 == y2) {
            H.push_back({y1, min(x1, x2), max(x1, x2)});
        } else {
            V.push_back({x1, min(y1, y2), max(y1, y2)});
        }
    }
    
    sort(H.begin(), H.end(), [](const HSeg& a, const HSeg& b) {
        if (a.y != b.y) return a.y < b.y;
        return a.x_min < b.x_min;
    });
    
    sort(V.begin(), V.end(), [](const VSeg& a, const VSeg& b) {
        if (a.x != b.x) return a.x < b.x;
        return a.y_min < b.y_min;
    });
    
    long long ans = 0;
    int h_sz = H.size();
    int v_sz = V.size();
    
    for (int i = 0; i < h_sz; ++i) {
        for (int j = i + 1; j < h_sz; ++j) {
            if (H[i].y == H[j].y) continue;
            
            long long x1 = max(H[i].x_min, H[j].x_min);
            long long x2 = min(H[i].x_max, H[j].x_max);
            
            if (x1 >= x2) continue;
            
            long long y1 = H[i].y;
            long long y2 = H[j].y;
            
            int count = 0;
            for (int k = 0; k < v_sz; ++k) {
                if (V[k].x >= x1 && V[k].x <= x2 && V[k].y_min <= y1 && V[k].y_max >= y2) {
                    count++;
                }
            }
            
            ans += (long long)count * (count - 1) / 2;
        }
    }
    
    cout << ans << "\n";
    return 0;
}
