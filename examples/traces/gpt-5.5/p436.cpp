#include <bits/stdc++.h>
using namespace std;

struct Segment {
    bool isNum;
    char ch;
    long long val;
    bool outDiputs; // true: output in Diputs, false: output in decimal
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string input((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());

    vector<char> sym = {'_', '.', ',', '-', '~', '=', '\'', '^', '"'}; // least to most
    vector<int> mx = {2, 3, 5, 7, 11, 13, 17, 19, 23};

    unordered_map<char, int> id;
    for (int i = 0; i < 9; ++i) id[sym[i]] = i;

    vector<long long> baseMul(9, 1);
    for (int i = 1; i < 9; ++i) baseMul[i] = baseMul[i - 1] * (mx[i - 1] + 1LL);

    long long MAXV = 1;
    for (int x : mx) MAXV *= (x + 1LL);
    MAXV--;

    auto diputsToValue = [&](const vector<int>& cnt) -> long long {
        long long res = 0;
        for (int i = 0; i < 9; ++i) res += cnt[i] * baseMul[i];
        return res;
    };

    auto valueToDiputs = [&](long long v) -> string {
        if (v == 0) return "O";
        vector<int> cnt(9);
        for (int i = 0; i < 9; ++i) {
            cnt[i] = v % (mx[i] + 1LL);
            v /= (mx[i] + 1LL);
        }
        string res;
        for (int i = 8; i >= 0; --i) {
            res.append(cnt[i], sym[i]);
        }
        return res;
    };

    vector<Segment> segs;
    vector<long long> nums;

    for (size_t i = 0; i < input.size();) {
        char c = input[i];

        if (isdigit((unsigned char)c)) {
            long long val = 0;

            if (c == '0') {
                val = 0;
                ++i;
            } else {
                while (i < input.size() && isdigit((unsigned char)input[i])) {
                    int d = input[i] - '0';
                    if (val > (MAXV - d) / 10) break;
                    val = val * 10 + d;
                    ++i;
                }
            }

            segs.push_back({true, 0, val, true});
            nums.push_back(val);
        } else if (c == 'O') {
            ++i;
            segs.push_back({true, 0, 0, false});
            nums.push_back(0);
        } else if (id.count(c)) {
            vector<int> cnt(9, 0);
            int last = 8;

            while (i < input.size() && id.count(input[i])) {
                int k = id[input[i]];
                if (k > last) break;
                if (cnt[k] == mx[k]) break;

                cnt[k]++;
                last = k;
                ++i;
            }

            long long val = diputsToValue(cnt);
            segs.push_back({true, 0, val, false});
            nums.push_back(val);
        } else {
            segs.push_back({false, c, 0, false});
            ++i;
        }
    }

    if (nums.size() % 2 == 1) {
        sort(nums.begin(), nums.end());
    }

    size_t ptr = 0;
    string output;

    for (auto &s : segs) {
        if (!s.isNum) {
            output.push_back(s.ch);
        } else {
            long long v;
            if (nums.size() % 2 == 1) v = nums[ptr++];
            else v = s.val;

            if (s.outDiputs) output += valueToDiputs(v);
            else output += to_string(v);
        }
    }

    cout << output;
    return 0;
}
