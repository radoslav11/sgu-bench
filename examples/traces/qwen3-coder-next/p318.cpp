#include <iostream>
#include <vector>
#include <bitset>
#include <set>

using namespace std;

int main() {
    int N, M;
    cin >> N >> M;

    vector<bitset<100>> resourceMask(N + 1);

    for (int i = 0; i < M; i++) {
        int k;
        cin >> k;
        while (k--) {
            int r;
            cin >> r;
            resourceMask[r].set(i);
        }
    }

    set<bitset<100>> distinctGroups;
    for (int r = 1; r <= N; r++) {
        if (!resourceMask[r].none()) {
            distinctGroups.insert(resourceMask[r]);
        }
    }

    cout << distinctGroups.size() << endl;

    return 0;
}
