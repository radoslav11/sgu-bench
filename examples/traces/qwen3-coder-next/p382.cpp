#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

__int128 gcd(__int128 a, __int128 b) {
    a = a < 0 ? -a : a;
    b = b < 0 ? -b : b;
    while (b != 0) {
        a %= b;
        swap(a, b);
    }
    return a;
}

string to_string(__int128 x) {
    if (x == 0) return "0";
    string s = "";
    bool neg = false;
    if (x < 0) {
        neg = true;
        x = -x;
    }
    while (x > 0) {
        s = char('0' + (x % 10)) + s;
        x /= 10;
    }
    if (neg) s = '-' + s;
    return s;
}

int main() {
    int N;
    cin >> N;

    vector<vector<long long>> C(51, vector<long long>(51, 0));
    for (int i = 0; i <= 50; i++) {
        C[i][0] = 1;
        for (int j = 1; j <= i; j++) {
            C[i][j] = C[i-1][j-1] + C[i-1][j];
        }
    }

    vector<pair<__int128, __int128>> I(N+1);
    I[0] = make_pair(1, 1);

    for (int i = 1; i <= N; i++) {
        __int128 sum_num = 1;
        __int128 sum_den = 1;

        for (int k = 0; k < i; k++) {
            __int128 term_num = (__int128)C[i][k] * I[k].first;
            __int128 term_den = I[k].second;

            sum_num = sum_num * term_den + term_num * sum_den;
            sum_den = sum_den * term_den;

            __int128 g = gcd(sum_num, sum_den);
            if (g > 1) {
                sum_num /= g;
                sum_den /= g;
            } else if (g == 0) {
                sum_num = 0;
                sum_den = 1;
            }
        }

        __int128 d_i = (__int128)3 * (1LL << i) - 2;
        __int128 num = sum_num;
        __int128 den = sum_den * d_i;

        __int128 g = gcd(num, den);
        if (g > 1) {
            num /= g;
            den /= g;
        } else if (g == 0) {
            num = 0;
            den = 1;
        }

        if (den < 0) {
            num = -num;
            den = -den;
        }

        I[i] = make_pair(num, den);
    }

    string num_str = to_string(I[N].first);
    string den_str = to_string(I[N].second);
    cout << num_str << "/" << den_str << endl;

    return 0;
}
