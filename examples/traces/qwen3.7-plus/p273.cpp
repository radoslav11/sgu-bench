#include <iostream>
#include <string>

using namespace std;

bool can_reduce[4][4][4];
bool dp[205][205][4];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int B, R, Y, W;
    if (!(cin >> B >> R >> Y >> W)) return 0;

    auto read_rules = [&](int count, int color) {
        for (int i = 0; i < count; ++i) {
            string s;
            cin >> s;
            int c1 = -1, c2 = -1;
            if (s[0] == 'b') c1 = 0;
            else if (s[0] == 'r') c1 = 1;
            else if (s[0] == 'y') c1 = 2;
            else if (s[0] == 'w') c1 = 3;
            
            if (s[1] == 'b') c2 = 0;
            else if (s[1] == 'r') c2 = 1;
            else if (s[1] == 'y') c2 = 2;
            else if (s[1] == 'w') c2 = 3;
            
            if (c1 != -1 && c2 != -1) {
                can_reduce[color][c1][c2] = true;
            }
        }
    };

    read_rules(B, 0);
    read_rules(R, 1);
    read_rules(Y, 2);
    read_rules(W, 3);

    string S;
    cin >> S;
    int N = S.length();

    for (int i = 0; i < N; ++i) {
        int c = -1;
        if (S[i] == 'b') c = 0;
        else if (S[i] == 'r') c = 1;
        else if (S[i] == 'y') c = 2;
        else if (S[i] == 'w') c = 3;
        if (c != -1) {
            dp[i][i][c] = true;
        }
    }

    for (int len = 2; len <= N; ++len) {
        for (int i = 0; i <= N - len; ++i) {
            int j = i + len - 1;
            for (int k = i; k < j; ++k) {
                for (int c1 = 0; c1 < 4; ++c1) {
                    if (!dp[i][k][c1]) continue;
                    for (int c2 = 0; c2 < 4; ++c2) {
                        if (!dp[k+1][j][c2]) continue;
                        
                        if (can_reduce[0][c1][c2]) dp[i][j][0] = true;
                        if (can_reduce[1][c1][c2]) dp[i][j][1] = true;
                        if (can_reduce[2][c1][c2]) dp[i][j][2] = true;
                        if (can_reduce[3][c1][c2]) dp[i][j][3] = true;
                    }
                }
            }
        }
    }

    string result = "";
    if (dp[0][N-1][0]) result += 'b';
    if (dp[0][N-1][1]) result += 'r';
    if (dp[0][N-1][2]) result += 'y';
    if (dp[0][N-1][3]) result += 'w';

    if (result.empty()) {
        cout << "Nobody\n";
    } else {
        cout << result << "\n";
    }

    return 0;
}
