#include <bits/stdc++.h>
using namespace std;

long long parseLength(const string& s) {
    long long intPart = 0, fracPart = 0;
    size_t pos = s.find('.');
    
    if (pos == string::npos) {
        intPart = stoll(s);
    } else {
        intPart = stoll(s.substr(0, pos));
        string frac = s.substr(pos + 1);
        while (frac.size() < 4) frac += '0';
        if (frac.size() > 4) frac = frac.substr(0, 4);
        fracPart = stoll(frac);
    }
    
    return intPart * 10000 + fracPart;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string Lstr;
    int N;
    cin >> Lstr >> N;

    long long L = parseLength(Lstr);
    long long pos = 0;

    for (int i = 0; i < N; ++i) {
        long long T, V;
        cin >> T >> V;

        long long add = ((T % L) * (V % L)) % L;
        add = (add * (10000 % L)) % L;

        pos = (pos + add) % L;
    }

    long long ans = min(pos, L - pos);

    cout << ans / 10000 << '.';
    cout << setw(4) << setfill('0') << ans % 10000 << '\n';

    return 0;
}
