#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int K, N;
    if (!(cin >> K >> N)) return 0;

    vector<pair<long long, int>> green_teas;
    vector<pair<long long, int>> black_teas;

    for (int i = 1; i <= N; ++i) {
        long long c;
        int s;
        cin >> c >> s;
        if (s == 0) {
            green_teas.push_back({c, i});
        } else {
            black_teas.push_back({c, i});
        }
    }

    sort(green_teas.begin(), green_teas.end());
    sort(black_teas.begin(), black_teas.end());

    int num_green = green_teas.size();
    int num_black = black_teas.size();

    long long min_cost = -1;
    int best_G = -1;

    for (int G = 0; G <= K; ++G) {
        int B = K - G;
        if (G > num_green) continue;
        if (B > num_black) continue;
        if (G > 2 * B + 2) continue;
        if (B > 2 * G + 2) continue;

        long long current_cost = 0;
        for (int i = 0; i < G; ++i) {
            current_cost += green_teas[i].first;
        }
        for (int i = 0; i < B; ++i) {
            current_cost += black_teas[i].first;
        }

        if (min_cost == -1 || current_cost < min_cost) {
            min_cost = current_cost;
            best_G = G;
        }
    }

    if (best_G == -1) {
        cout << "Impossible\n";
    } else {
        int G = best_G;
        int B = K - G;
        vector<int> seq;
        int g = G, b = B;
        int idx_g = 0, idx_b = 0;

        while (g > 0 || b > 0) {
            bool can_g = (seq.size() < 2 || !(seq[seq.size()-1] == 0 && seq[seq.size()-2] == 0));
            bool can_b = (seq.size() < 2 || !(seq[seq.size()-1] == 1 && seq[seq.size()-2] == 1));

            bool pick_g = false;
            if (can_g && g > 0) {
                if (!can_b || b == 0) {
                    pick_g = true;
                } else if (g >= b) {
                    pick_g = true;
                }
            }

            if (pick_g) {
                seq.push_back(0);
                g--;
            } else if (can_b && b > 0) {
                seq.push_back(1);
                b--;
            }
        }

        for (size_t i = 0; i < seq.size(); ++i) {
            if (seq[i] == 0) {
                cout << green_teas[idx_g++].second << (i == seq.size() - 1 ? "" : " ");
            } else {
                cout << black_teas[idx_b++].second << (i == seq.size() - 1 ? "" : " ");
            }
        }
        cout << "\n";
    }

    return 0;
}
