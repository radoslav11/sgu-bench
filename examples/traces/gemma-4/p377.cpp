#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/**
 * Problem analysis:
 * The condition is that every 2x2 square in an N x M grid must contain exactly two students.
 * Let x_{i,j} be 1 if cell (i,j) has a student, 0 otherwise.
 * The condition is x_{i,j} + x_{i+1,j} + x_{i,j+1} + x_{i+1,j+1} = 2.
 * This problem has a known structural property. The solutions are:
 * 1. Each column is a sequence of (0,1,0,1,...) or (1,0,1,0,...).
 *    There are 2^M such ways.
 * 2. Each row is a sequence of (0,1,0,1,...) or (1,0,1,0,...).
 *    There are 2^N such ways.
 * The overlap between these two sets of solutions consists of the two checkerboard patterns.
 * So the total number of ways is 2^M + 2^N - 2.
 * 
 * For N, M up to 1000, 2^N + 2^M - 2 can be up to 2^1001 - 2, which has about 302 decimal digits.
 * We need to perform big integer addition and subtraction.
 */

// Represents a big integer as a vector of digits (least significant first)
typedef vector<int> BigInt;

// Multiplies a BigInt by 2
BigInt multiplyByTwo(BigInt a) {
    int carry = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        int val = a[i] * 2 + carry;
        a[i] = val % 10;
        carry = val / 10;
    }
    if (carry) {
        a.push_back(carry);
    }
    return a;
}

// Computes 2^n as a BigInt
BigInt powerOfTwo(int n) {
    BigInt res = {1};
    for (int i = 0; i < n; ++i) {
        res = multiplyByTwo(res);
    }
    return res;
}

// Adds two BigInts
BigInt add(BigInt a, BigInt b) {
    BigInt res;
    int carry = 0;
    size_t n = max(a.size(), b.size());
    for (size_t i = 0; i < n || carry; ++i) {
        int val = carry + (i < a.size() ? a[i] : 0) + (i < b.size() ? b[i] : 0);
        res.push_back(val % 10);
        carry = val / 10;
    }
    return res;
}

// Subtracts a small integer (in this case, 2) from a BigInt
// The problem guarantees N, M >= 2, so 2^N + 2^M - 2 > 0
BigInt subtractTwo(BigInt a) {
    int borrow = 2;
    for (size_t i = 0; i < a.size() && borrow > 0; ++i) {
        if (a[i] >= borrow) {
            a[i] -= borrow;
            borrow = 0;
        } else {
            a[i] = a[i] + 10 - borrow;
            borrow = 1;
        }
    }
    // Remove leading zeros
    while (a.size() > 1 && a.back() == 0) {
        a.pop_back();
    }
    return a;
}

int main() {
    // Speed up standard I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    // Calculate 2^N and 2^M
    BigInt powN = powerOfTwo(N);
    BigInt powM = powerOfTwo(M);

    // Result is 2^N + 2^M - 2
    BigInt res = add(powN, powM);
    res = subtractTwo(res);

    // Convert the BigInt to a string (reverse digits for human-readable format)
    string output = "";
    for (int i = (int)res.size() - 1; i >= 0; --i) {
        output += to_string(res[i]);
    }

    cout << output << endl;

    return 0;
}

