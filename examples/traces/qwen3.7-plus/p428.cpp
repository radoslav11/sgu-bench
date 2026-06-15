#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

string get_pattern(int x, int y, int z) {
    string s = to_string(x) + "+" + to_string(y) + "=" + to_string(z);
    string res = "";
    char current_char = 'A';
    char m[256] = {0};
    for (char c : s) {
        if (c >= '0' && c <= '9') {
            if (m[(unsigned char)c] == 0) {
                m[(unsigned char)c] = current_char++;
            }
            res += m[(unsigned char)c];
        } else {
            res += c;
        }
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    map<string, int> counts;

    // Length 3: 100 <= x <= 499, x <= y, x + y < 1000
    for (int x = 100; x <= 499; ++x) {
        for (int y = x; y <= 999 - x; ++y) {
            int z = x + y;
            counts[get_pattern(x, y, z)]++;
        }
    }

    // Length 4: 1000 <= x <= 4999, x <= y, x + y < 10000
    for (int x = 1000; x <= 4999; ++x) {
        for (int y = x; y <= 9999 - x; ++y) {
            int z = x + y;
            counts[get_pattern(x, y, z)]++;
        }
    }

    // Output 1000 proper rebuses (patterns with exactly 1 valid solution)
    int printed = 0;
    for (auto const& pair : counts) {
        if (pair.second == 1) {
            cout << pair.first << "\n";
            printed++;
            if (printed == 1000) {
                break;
            }
        }
    }

    return 0;
}
