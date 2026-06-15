#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>

using namespace std;

struct BigInt {
    vector<int> digits;
    BigInt() {}
    BigInt(long long v) {
        if (v == 0) digits.push_back(0);
        while (v > 0) {
            digits.push_back(v % 10);
            v /= 10;
        }
    }
    void add(const BigInt& other) {
        int n = max(digits.size(), other.digits.size());
        int carry = 0;
        for (int i = 0; i < n || carry; ++i) {
            if (i == (int)digits.size()) digits.push_back(0);
            int sum = digits[i] + carry + (i < (int)other.digits.size() ? other.digits[i] : 0);
            digits[i] = sum % 10;
            carry = sum / 10;
        }
    }
    void divide(int divisor) {
        int remainder = 0;
        for (int i = (int)digits.size() - 1; i >= 0; --i) {
            long long current = remainder * 10LL + digits[i];
            digits[i] = current / divisor;
            remainder = current % divisor;
        }
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }
    void print() {
        if (digits.empty()) {
            cout << 0 << "\n";
            return;
        }
        for (int i = (int)digits.size() - 1; i >= 0; --i) {
            cout << digits[i];
        }
        cout << "\n";
    }
};

struct TransformHash {
    size_t operator()(const vector<uint16_t>& v) const {
        size_t h = 0;
        for (uint16_t x : v) {
            h = h * 131 + x;
        }
        return h;
    }
};

struct TransformEq {
    bool operator()(const vector<uint16_t>& a, const vector<uint16_t>& b) const {
        return a == b;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    vector<uint16_t> mapping(N * M);
    unordered_set<vector<uint16_t>, TransformHash, TransformEq> unique_transforms;

    auto add_transforms = [&](bool set1, bool set2) {
        if (set1) {
            for (int w = 1; w >= -1; w -= 2) {
                for (int u = 0; u < M; ++u) {
                    for (int a = 0; a < N; ++a) {
                        for (int b = 0; b < M; ++b) {
                            for (int r = 0; r < N; ++r) {
                                for (int c = 0; c < M; ++c) {
                                    int r_new = (w * r + u * c + a) % N;
                                    if (r_new < 0) r_new += N;
                                    int c_new = (w * c + b) % M;
                                    if (c_new < 0) c_new += M;
                                    mapping[r * M + c] = r_new * M + c_new;
                                }
                            }
                            unique_transforms.insert(mapping);
                        }
                    }
                }
            }
        }
        if (set2) {
            for (int w = 1; w >= -1; w -= 2) {
                for (int u = 0; u < N; ++u) {
                    for (int a = 0; a < N; ++a) {
                        for (int b = 0; b < M; ++b) {
                            for (int r = 0; r < N; ++r) {
                                for (int c = 0; c < M; ++c) {
                                    int r_new = (w * r + a) % N;
                                    if (r_new < 0) r_new += N;
                                    int c_new = (u * r + w * c + b) % M;
                                    if (c_new < 0) c_new += M;
                                    mapping[r * M + c] = r_new * M + c_new;
                                }
                            }
                            unique_transforms.insert(mapping);
                        }
                    }
                }
            }
        }
    };

    if (N > M) {
        add_transforms(true, false);
    } else if (N < M) {
        add_transforms(false, true);
    } else {
        add_transforms(true, true);
    }

    vector<BigInt> pow2(N * M + 1);
    pow2[0] = BigInt(1);
    for (int i = 1; i <= N * M; ++i) {
        pow2[i] = pow2[i - 1];
        int carry = 0;
        for (size_t j = 0; j < pow2[i].digits.size() || carry; ++j) {
            if (j == pow2[i].digits.size()) pow2[i].digits.push_back(0);
            int sum = pow2[i].digits[j] * 2 + carry;
            pow2[i].digits[j] = sum % 10;
            carry = sum / 10;
        }
    }

    BigInt total_fixed(0);
    vector<bool> visited(N * M, false);

    for (const auto& m : unique_transforms) {
        fill(visited.begin(), visited.end(), false);
        int cycles = 0;
        for (int i = 0; i < N * M; ++i) {
            if (!visited[i]) {
                cycles++;
                int curr = i;
                while (!visited[curr]) {
                    visited[curr] = true;
                    curr = m[curr];
                }
            }
        }
        total_fixed.add(pow2[cycles]);
    }

    total_fixed.divide(unique_transforms.size());
    total_fixed.print();

    return 0;
}
