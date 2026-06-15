#include <bits/stdc++.h>
using namespace std;

static vector<int> manacher_odd(const string &s) {
    int n = (int)s.size();
    vector<int> d(n);
    int l = 0, r = -1;
    for (int i = 0; i < n; i++) {
        int k = (i > r) ? 1 : min(d[l + r - i], r - i + 1);
        while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
        d[i] = k;
        if (i + k - 1 > r) {
            l = i - k + 1;
            r = i + k - 1;
        }
    }
    return d;
}

static vector<int> manacher_even(const string &s) {
    int n = (int)s.size();
    vector<int> d(n);
    int l = 0, r = -1;
    for (int i = 0; i < n; i++) {
        int k = (i > r) ? 0 : min(d[l + r - i + 1], r - i + 1);
        while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
        d[i] = k;
        if (i + k - 1 > r) {
            l = i - k;
            r = i + k - 1;
        }
    }
    return d;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int h, w;
    cin >> h >> w;

    vector<string> a(h);
    for (int i = 0; i < h; i++) cin >> a[i];

    vector<int> rowOdd(h * w), rowEven(h * w);
    vector<int> colOdd(h * w), colEven(h * w);

    for (int i = 0; i < h; i++) {
        auto d1 = manacher_odd(a[i]);
        auto d2 = manacher_even(a[i]);
        for (int j = 0; j < w; j++) {
            rowOdd[i * w + j] = d1[j];
            rowEven[i * w + j] = d2[j];
        }
    }

    for (int j = 0; j < w; j++) {
        string s;
        s.reserve(h);
        for (int i = 0; i < h; i++) s.push_back(a[i][j]);

        auto d1 = manacher_odd(s);
        auto d2 = manacher_even(s);

        for (int i = 0; i < h; i++) {
            colOdd[i * w + j] = d1[i];
            colEven[i * w + j] = d2[i];
        }
    }

    auto rowPal = [&](int r, int c, int len) -> bool {
        if (len & 1) {
            int rad = len / 2;
            int center = c + rad;
            return rowOdd[r * w + center] >= rad + 1;
        } else {
            int rad = len / 2;
            int center = c + rad;
            return rowEven[r * w + center] >= rad;
        }
    };

    auto colPal = [&](int r, int c, int len) -> bool {
        if (len & 1) {
            int rad = len / 2;
            int center = r + rad;
            return colOdd[center * w + c] >= rad + 1;
        } else {
            int rad = len / 2;
            int center = r + rad;
            return colEven[center * w + c] >= rad;
        }
    };

    auto check = [&](int len, int &ansR, int &ansC) -> bool {
        int hh = h - len + 1;
        int ww = w - len + 1;
        if (hh <= 0 || ww <= 0) return false;

        vector<unsigned char> goodRows(hh * ww, 0);

        for (int c = 0; c < ww; c++) {
            int sum = 0;
            for (int r = 0; r < len; r++) {
                if (rowPal(r, c, len)) sum++;
            }

            for (int r = 0; r < hh; r++) {
                if (sum == len) goodRows[r * ww + c] = 1;

                if (r + len < h) {
                    if (rowPal(r, c, len)) sum--;
                    if (rowPal(r + len, c, len)) sum++;
                }
            }
        }

        for (int r = 0; r < hh; r++) {
            int sum = 0;
            for (int c = 0; c < len; c++) {
                if (colPal(r, c, len)) sum++;
            }

            for (int c = 0; c < ww; c++) {
                if (sum == len && goodRows[r * ww + c]) {
                    ansR = r;
                    ansC = c;
                    return true;
                }

                if (c + len < w) {
                    if (colPal(r, c, len)) sum--;
                    if (colPal(r, c + len, len)) sum++;
                }
            }
        }

        return false;
    };

    int bestLen = 1, bestR = 0, bestC = 0;
    int n = min(h, w);

    {
        int lo = 0, hi = (n - 1) / 2;
        while (lo < hi) {
            int mid = (lo + hi + 1) / 2;
            int r, c;
            if (check(2 * mid + 1, r, c)) lo = mid;
            else hi = mid - 1;
        }

        int r, c;
        int len = 2 * lo + 1;
        if (check(len, r, c) && len > bestLen) {
            bestLen = len;
            bestR = r;
            bestC = c;
        }
    }

    {
        int lo = 0, hi = n / 2;
        while (lo < hi) {
            int mid = (lo + hi + 1) / 2;
            int r, c;
            if (check(2 * mid, r, c)) lo = mid;
            else hi = mid - 1;
        }

        if (lo > 0) {
            int r, c;
            int len = 2 * lo;
            if (check(len, r, c) && len > bestLen) {
                bestLen = len;
                bestR = r;
                bestC = c;
            }
        }
    }

    cout << bestR + 1 << ' ' << bestC + 1 << ' '
         << bestR + bestLen << ' ' << bestC + bestLen << '\n';

    return 0;
}
