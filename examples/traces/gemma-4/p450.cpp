#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <iomanip>
#include <algorithm>

using namespace std;

// Use a large value for infinity that doesn't overflow during min/max operations.
const long long INF = 2000000000000000000LL; 

struct Group {
    long long t, p, w, e;
};

struct Event {
    long long time;
    int type; // 0 for Finish Eating, 1 for Arrival
    int group_id;

    // Priority queue is a max-heap by default, so we use greater for a min-heap.
    // Events at the same time should process Finish first, then Arrival.
    bool operator>(const Event& other) const {
        if (time != other.time) return time > other.time;
        return type > other.type;
    }
};

struct SeatOption {
    long long min_sl_sr;
    long long max_sl_sr;
    int counter_idx;
    int leftmost_seat;

    // Ordering criteria for selecting seats
    bool operator<(const SeatOption& other) const {
        if (min_sl_sr != other.min_sl_sr) return min_sl_sr < other.min_sl_sr;
        if (max_sl_sr != other.max_sl_sr) return max_sl_sr < other.max_sl_sr;
        if (counter_idx != other.counter_idx) return counter_idx > other.counter_idx;
        return leftmost_seat > other.leftmost_seat;
    }
};

int N, M;
long long T_closing;
int C[101];
Group groups[10001];
bool occupied[101][101];
int group_seated_in[10001];
int group_seated_start[10001];

int main() {
    // Speed up standard I/O.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> M >> T_closing)) return 0;
    for (int i = 0; i < N; ++i) cin >> C[i];
    for (int i = 0; i < M; ++i) {
        cin >> groups[i].t >> groups[i].p >> groups[i].w >> groups[i].e;
    }

    priority_queue<Event, vector<Event>, greater<Event>> events;
    for (int i = 0; i < M; ++i) {
        events.push({groups[i].t, 1, i});
    }

    deque<int> waiting_queue;
    double total_satisfaction = 0;
    long long total_customers = 0;
    for (int i = 0; i < M; ++i) total_customers += groups[i].p;

    while (!events.empty()) {
        long long t = events.top().time;

        // 1. Handle all groups finishing eating at current time t.
        // This releases seats before any new groups try to occupy them.
        while (!events.empty() && events.top().time == t && events.top().type == 0) {
            int gid = events.top().group_id;
            int ci = group_seated_in[gid];
            int start = group_seated_start[gid];
            for (int j = 0; j < (int)groups[gid].p; ++j) {
                occupied[ci][start + j] = false;
            }
            events.pop();
        }

        // 2. Handle all groups arriving at current time t.
        while (!events.empty() && events.top().time == t && events.top().type == 1) {
            int gid = events.top().group_id;
            waiting_queue.push_back(gid);
            events.pop();
        }

        // 3. Process the waiting queue.
        while (!waiting_queue.empty()) {
            int gid = waiting_queue.front();
            // Check if the group has timed out (exceeded wait time or closing time reached).
            if (t > groups[gid].t + groups[gid].w || t >= T_closing) {
                total_satisfaction -= (double)groups[gid].p;
                waiting_queue.pop_front();
                continue;
            }

            // Search for the best block of seats among all counters.
            SeatOption best_option = {-1, -1, 1000, 1000};
            bool found = false;

            for (int i = 0; i < N; ++i) {
                for (int j = 0; j <= C[i] - (int)groups[gid].p; ++j) {
                    bool possible = true;
                    for (int k = 0; k < (int)groups[gid].p; ++k) {
                        if (occupied[i][j + k]) {
                            possible = false;
                            break;
                        }
                    }
                    if (possible) {
                        long long sl = INF, sr = INF;
                        // Calculate empty seats to the left.
                        for (int k = j - 1; k >= 0; --k) {
                            if (occupied[i][k]) {
                                sl = (long long)j - k - 1;
                                break;
                            }
                        }
                        // Calculate empty seats to the right.
                        for (int k = j + (int)groups[gid].p; k < C[i]; ++k) {
                            if (occupied[i][k]) {
                                sr = (long long)k - (j + (int)groups[gid].p - 1) - 1;
                                break;
                            }
                        }
                        SeatOption current = {min(sl, sr), max(sl, sr), i, j};
                        if (!found || best_option < current) {
                            best_option = current;
                            found = true;
                        }
                    }
                }
            }

            if (found) {
                // Seat the group.
                group_seated_in[gid] = best_option.counter_idx;
                group_seated_start[gid] = best_option.leftmost_seat;
                for (int k = 0; k < (int)groups[gid].p; ++k) {
                    occupied[best_option.counter_idx][best_option.leftmost_seat + k] = true;
                }
                double satisfaction_per_person = (double)(groups[gid].w - (t - groups[gid].t)) / (double)groups[gid].w;
                total_satisfaction += (double)groups[gid].p * satisfaction_per_person;
                // Schedule the time when the group finishes eating.
                events.push({t + groups[gid].e, 0, gid});
                waiting_queue.pop_front();
            } else {
                // If the first group in the queue cannot be seated, no other waiting group can.
                break;
            }
        }
    }

    // Any remaining groups in the waiting queue have given up.
    while (!waiting_queue.empty()) {
        int gid = waiting_queue.front();
        total_satisfaction -= (double)groups[gid].p;
        waiting_queue.pop_front();
    }

    // Output the final average satisfaction.
    cout << fixed << setprecision(16) << total_satisfaction / total_customers << endl;

    return 0;
}

