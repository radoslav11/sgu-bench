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

struct Zone {
    int q, cap, id;
};

struct Student {
    int p, w, id;
};

int k;
int64_t total_n;
vector<Zone> zones;
vector<Student> students;

void read() {
    cin >> k;
    zones.resize(k);
    total_n = 0;
    for(int i = 0; i < k; i++) {
        cin >> zones[i].cap;
        total_n += zones[i].cap;
    }

    for(int i = 0; i < k; i++) {
        cin >> zones[i].q;
        zones[i].id = i;
    }

    students.resize(total_n);
    for(int i = 0; i < total_n; i++) {
        cin >> students[i].p;
    }

    for(int i = 0; i < total_n; i++) {
        cin >> students[i].w;
        students[i].id = i;
    }
}

void solve() {
    // A student qualifies for the All-Russian olympiad iff his level p exceeds
    // his zone's level q. To maximise total qualifying weight we greedily
    // process students by decreasing weight; sort zones by increasing level
    // and place each student into the highest-level zone that still has free
    // capacity and that he can beat (q < p). This keeps low-level zones free
    // for weaker students who can only qualify there. Any students left
    // unassigned are poured into whatever capacity remains, in zone order.

    sort(students.begin(), students.end(), [&](auto& a, auto& b) {
        return a.w > b.w;
    });

    sort(zones.begin(), zones.end(), [&](auto& a, auto& b) {
        return a.q < b.q;
    });

    vector<int> answer(total_n, -1);

    for(auto& st: students) {
        int best_zone = -1;
        for(int j = k - 1; j >= 0; j--) {
            if(zones[j].q < st.p && zones[j].cap > 0) {
                best_zone = j;
                break;
            }
        }

        if(best_zone != -1) {
            answer[st.id] = zones[best_zone].id + 1;
            zones[best_zone].cap--;
        }
    }

    int zi = 0;
    for(int i = 0; i < total_n; i++) {
        if(answer[i] == -1) {
            while(zi < k && zones[zi].cap == 0) {
                zi++;
            }

            answer[i] = zones[zi].id + 1;
            zones[zi].cap--;
        }
    }

    for(int i = 0; i < total_n; i++) {
        cout << answer[i] << (i + 1 < total_n ? ' ' : '\n');
    }
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
