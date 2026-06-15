#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream& out, const pair<T1, T2>& x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream& operator>>(istream& in, pair<T1, T2>& x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream& operator>>(istream& in, vector<T>& a) {
    for(auto& x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

const int16_t MAX_HEIGHT = 101;
const int16_t MAX_PERIMETER = 5300;

int16_t n;
vector<int16_t> heights;

void read() {
    cin >> n;
    heights.resize(n);
    cin >> heights;
}

int16_t calculate_initial_perimeter(const vector<int16_t>& h) {
    int16_t perimeter = 2;
    for(int16_t i = 0; i < static_cast<int16_t>(h.size()) - 1; ++i) {
        perimeter += 2 + abs(h[i] - h[i + 1]);
    }

    perimeter += h[0] + h.back();
    return perimeter;
}

vector<int16_t> reconstruct_stolen_boards(
    const vector<vector<vector<int16_t>>>& dp, const vector<int16_t>& h,
    int16_t max_height_last_height, int16_t max_height_perimeter
) {
    vector<int16_t> stolen_boards;
    int16_t current_height = max_height_last_height;
    int16_t current_perimeter = max_height_perimeter;

    for(int16_t pos = static_cast<int16_t>(h.size()); pos > 0; --pos) {
        if(dp[pos][current_height][current_perimeter] ==
               dp[pos - 1][current_height][current_perimeter] + h[pos - 1] &&
           dp[pos - 1][current_height][current_perimeter] != -1) {
            stolen_boards.push_back(pos);
        } else {
            for(int16_t prev_height = 0; prev_height < MAX_HEIGHT;
                ++prev_height) {
                int16_t prev_perimeter =
                    current_perimeter - abs(prev_height - current_height) - 2;
                if(prev_perimeter >= 0 &&
                   dp[pos - 1][prev_height][prev_perimeter] ==
                       dp[pos][current_height][current_perimeter]) {
                    current_height = prev_height;
                    current_perimeter = prev_perimeter;
                    break;
                }
            }
        }
    }

    sort(stolen_boards.begin(), stolen_boards.end());
    return stolen_boards;
}

void solve() {
    // dp[pos][last_height][perimeter] = maximum total height of the boards
    // removed among the first pos boards, given that the rightmost kept board
    // has height last_height and the kept fence so far contributes perimeter
    // to the half-perimeter accounting (each kept board adds 2 for its top and
    // bottom plus the height difference to the previous kept board). For each
    // board we either steal it (add its height to the removed total, keeping
    // last_height and perimeter) or keep it (advance last_height and add the
    // perimeter contribution). The target is to maximise removed height while
    // the kept fence's perimeter stays at least half the original. We compute
    // the initial perimeter, set the target to ceil(initial / 2), pick the
    // best final state with perimeter + last_height >= target, then walk the
    // dp table backwards to recover which boards were stolen.

    vector<vector<vector<int16_t>>> dp(
        n + 1,
        vector<vector<int16_t>>(MAX_HEIGHT, vector<int16_t>(MAX_PERIMETER, -1))
    );
    dp[0][0][0] = 0;

    int16_t initial_perimeter = calculate_initial_perimeter(heights);
    int16_t target_perimeter = (initial_perimeter + 1) / 2;

    for(int16_t pos = 0; pos < n; ++pos) {
        for(int16_t last_height = 0; last_height < MAX_HEIGHT; ++last_height) {
            for(int16_t perimeter = 0; perimeter < MAX_PERIMETER; ++perimeter) {
                if(dp[pos][last_height][perimeter] == -1) {
                    continue;
                }

                int16_t new_height = heights[pos];
                dp[pos + 1][last_height][perimeter] = max<int16_t>(
                    dp[pos + 1][last_height][perimeter],
                    dp[pos][last_height][perimeter] + new_height
                );

                int16_t new_perimeter =
                    perimeter + abs(last_height - new_height) + 2;
                if(new_perimeter < MAX_PERIMETER) {
                    dp[pos + 1][new_height][new_perimeter] = max<int16_t>(
                        dp[pos + 1][new_height][new_perimeter],
                        dp[pos][last_height][perimeter]
                    );
                }
            }
        }
    }

    int16_t max_height = -1, max_height_perimeter = 0,
            max_height_last_height = 0;
    for(int16_t last_height = 0; last_height < MAX_HEIGHT; ++last_height) {
        for(int16_t perimeter = 0; perimeter < MAX_PERIMETER; ++perimeter) {
            if(perimeter + last_height >= target_perimeter &&
               dp[n][last_height][perimeter] > max_height) {
                max_height = dp[n][last_height][perimeter];
                max_height_perimeter = perimeter;
                max_height_last_height = last_height;
            }
        }
    }

    vector<int16_t> stolen_boards = reconstruct_stolen_boards(
        dp, heights, max_height_last_height, max_height_perimeter
    );

    cout << max_height << endl;
    cout << static_cast<int16_t>(stolen_boards.size()) << endl;
    for(int16_t index: stolen_boards) {
        cout << index << " ";
    }

    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
