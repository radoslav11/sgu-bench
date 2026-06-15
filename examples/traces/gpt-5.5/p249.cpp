#include <bits/stdc++.h>
using namespace std;

class FastOutput {
    static const int BUFSIZE = 1 << 20;
    char buf[BUFSIZE];
    int pos = 0;

public:
    ~FastOutput() {
        flush();
    }

    void flush() {
        if (pos) {
            fwrite(buf, 1, pos, stdout);
            pos = 0;
        }
    }

    void putChar(char c) {
        if (pos == BUFSIZE) flush();
        buf[pos++] = c;
    }

    void writeUInt(unsigned int x) {
        char s[12];
        int n = 0;
        if (x == 0) {
            putChar('0');
            return;
        }
        while (x > 0) {
            s[n++] = char('0' + x % 10);
            x /= 10;
        }
        while (n--) putChar(s[n]);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    int rows = 1 << N;
    int cols = 1 << M;

    FastOutput out;

    for (int i = 0; i < rows; ++i) {
        unsigned int rowGray = i ^ (i >> 1);

        for (int j = 0; j < cols; ++j) {
            unsigned int colGray = j ^ (j >> 1);
            unsigned int value = rowGray | (colGray << N);

            if (j) out.putChar(' ');
            out.writeUInt(value);
        }
        out.putChar('\n');
    }

    return 0;
}
