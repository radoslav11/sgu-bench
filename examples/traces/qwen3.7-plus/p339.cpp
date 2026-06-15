#include <iostream>
#include <vector>

using namespace std;

struct Segment {
    int l, r;
};

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    vector<Segment> segs;
    segs.reserve(1005);
    
    char type;
    int l, r;
    
    while (cin >> type >> l >> r) {
        if (type == '+') {
            int count = 0;
            for (size_t i = 0; i < segs.size(); ++i) {
                if (segs[i].l >= l && segs[i].r <= r) {
                    count++;
                }
            }
            cout << count << "\n";
            segs.push_back({l, r});
        } else if (type == '-') {
            for (size_t i = 0; i < segs.size(); ++i) {
                if (segs[i].l == l && segs[i].r == r) {
                    segs[i] = segs.back();
                    segs.pop_back();
                    break;
                }
            }
        }
    }
    
    return 0;
}
