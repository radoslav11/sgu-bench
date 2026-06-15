#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<int> B(M + 1);
    for (int i = 1; i <= M; ++i) cin >> B[i];

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<pair<string, string>> accepted;

    for (int f = 0; f < N; ++f) {
        string faculty;
        getline(cin, faculty);

        int K;
        cin >> K;

        vector<int> subjects(K);
        for (int i = 0; i < K; ++i) cin >> subjects[i];

        int sumGrades = 0;

        for (int i = 0; i < K; ++i) {
            int X, Y, Z;
            cin >> X >> Y >> Z;

            int T = B[subjects[i]];
            int grade;

            if (T < X) grade = 2;
            else if (T < Y) grade = 3;
            else if (T < Z) grade = 4;
            else grade = 5;

            sumGrades += grade;
        }

        int S;
        cin >> S;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        for (int i = 0; i < S; ++i) {
            string specialty;
            getline(cin, specialty);

            int required;
            cin >> required;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (sumGrades >= required) {
                accepted.push_back({faculty, specialty});
            }
        }
    }

    if (accepted.empty()) {
        cout << "Army\n";
    } else {
        for (auto &p : accepted) {
            cout << p.first << ' ' << p.second << '\n';
        }
    }

    return 0;
}
