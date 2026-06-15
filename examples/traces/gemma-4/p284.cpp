#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct BigInt {
    vector<long long> digits;
    static const long long BASE = 1000000000000000000LL;

    BigInt(long long v = 0) {
        if (v == 0) digits.push_back(0);
        else {
            while (v > 0) {
                digits.push_back(v % BASE);
                v /= BASE;
            }
        }
    }

    void add(const BigInt& other) {
        size_t n = max(digits.size(), other.digits.size());
        long long carry = 0;
        for (size_t i = 0; i < n || carry; ++i) {
            if (i == digits.size()) digits.push_back(0);
            long long cur = digits[i] + carry + (i < other.digits.size() ? other.digits[i] : 0);
            if (cur >= BASE) {
                digits[i] = cur - BASE;
                carry = 1;
            } else {
                digits[i] = cur;
                carry = 0;
            }
        }
    }

    void subtract(const BigInt& other) {
        long long carry = 0;
        for (size_t i = 0; i < other.digits.size() || carry; ++i) {
            long long sub = (i < other.digits.size() ? other.digits[i] : 0) + carry;
            if (digits[i] < sub) {
                digits[i] = digits[i] + BASE - sub;
                carry = 1;
            } else {
                digits[i] -= sub;
                carry = 0;
            }
        }
        while (digits.size() > 1 && digits.back() == 0) digits.pop_back();
    }

    void print() const {
        if (digits.empty()) { printf("0\n"); return; }
        printf("%lld", digits.back());
        for (int i = (int)digits.size() - 2; i >= 0; --i) {
            printf("%018lld", digits[i]);
        }
        printf("\n");
    }
};

struct Node {
    vector<int> children;
    long long len;
    string full, pref, suff;
    BigInt count;
};

int count_occurrences(const string& text, const string& pattern) {
    if (pattern.empty()) return 0;
    int cnt = 0;
    size_t pos = text.find(pattern, 0);
    while (pos != string::npos) {
        cnt++;
        pos = text.find(pattern, pos + 1);
    }
    return cnt;
}

int main() {
    int N;
    if (!(cin >> N)) return 0;
    string S;
    cin >> S;
    int S_len = S.length();
    vector<Node> nodes(N + 1);
    for (int i = 1; i <= N; ++i) {
        int L; cin >> L;
        for (int j = 0; j < L; ++j) {
            string s; cin >> s;
            if (s == "a") nodes[i].children.push_back(-1);
            else if (s == "b") nodes[i].children.push_back(-2);
            else nodes[i].children.push_back(stoi(s));
        }
    }

    for (int i = 1; i <= N; ++i) {
        long long total_len = 0;
        string V = "";
        BigInt sum_all_counts(0);
        BigInt total_count_T_prime(0);

        for (int child : nodes[i].children) {
            string c_full, c_pref, c_suff;
            long long c_len;
            BigInt c_count;

            if (child == -1) {
                c_full = "a"; c_len = 1; c_pref = "a"; c_suff = "a"; c_count = BigInt(S == "a");
            } else if (child == -2) {
                c_full = "b"; c_len = 1; c_pref = "b"; c_suff = "b"; c_count = BigInt(S == "b");
            } else {
                c_len = nodes[child].len;
                c_full = nodes[child].full;
                c_pref = nodes[child].pref;
                c_suff = nodes[child].suff;
                c_count = nodes[child].count;
            }

            total_len += c_len;
            if (total_len > S_len) total_len = S_len + 1;
            sum_all_counts.add(c_count);
            if (c_len < S_len) {
                V += c_full;
                total_count_T_prime.add(c_count);
            } else {
                string T_prime = c_pref + "#" + c_suff;
                V += T_prime;
                total_count_T_prime.add(BigInt(count_occurrences(T_prime, S)));
            }
        }
        nodes[i].len = total_len;
        nodes[i].count = sum_all_counts;
        nodes[i].count.add(BigInt(count_occurrences(V, S)));
        nodes[i].count.subtract(total_count_T_prime);

        string full_str = "";
        for (int child : nodes[i].children) {
            if (child == -1) full_str += "a";
            else if (child == -2) full_str += "b";
            else {
                if (nodes[child].len < S_len) full_str += nodes[child].full;
                else full_str += nodes[child].pref;
            }
            if (full_str.length() >= S_len) break;
        }
        if (nodes[i].len < S_len) nodes[i].full = full_str;
        else nodes[i].full = "";

        string pref_str = "";
        for (int child : nodes[i].children) {
            if (child == -1) pref_str += "a";
            else if (child == -2) pref_str += "b";
            else {
                if (nodes[child].len < S_len) pref_str += nodes[child].full;
                else pref_str += nodes[child].pref;
            }
            if (pref_str.length() >= S_len - 1) break;
        }
        nodes[i].pref = pref_str.substr(0, S_len - 1);

        string suff_str = "";
        for (int j = (int)nodes[i].children.size() - 1; j >= 0; --j) {
            int child = nodes[i].children[j];
            string child_suff;
            if (child == -1) child_suff = "a";
            else if (child == -2) child_suff = "b";
            else {
                if (nodes[child].len < S_len) child_suff = nodes[child].full;
                else child_suff = nodes[child].suff;
            }
            suff_str = child_suff + suff_str;
            if (suff_str.length() >= S_len - 1) break;
        }
        if (suff_str.length() >= S_len - 1) suff_str = suff_str.substr(suff_str.length() - (S_len - 1));
        nodes[i].suff = suff_str;
    }
    nodes[N].count.print();
    return 0;
}

