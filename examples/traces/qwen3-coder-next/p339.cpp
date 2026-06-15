#include <iostream>
#include <vector>
#include <cctype>
#include <string>
using namespace std;

int main() {
    vector<pair<int, int>> segments;
    char op;
    int L, R;

    while (cin >> op >> L >> R) {
        if (op == '+') {
            int count = 0;
            for (const auto& seg : segments) {
                if (L <= seg.first && seg.second <= R) {
                    count++;
                }
            }
            cout << count << '\n';
            segments.push_back({L, R});
        } else if (op == '-') {
            for (auto it = segments.begin(); it != segments.end(); ++it) {
                if (it->first == L && it->second == R) {
                    segments.erase(it);
                    break;
                }
            }
        }
    }

    return 0;
}
