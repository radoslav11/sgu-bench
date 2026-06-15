#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

/**
 * Problem analysis:
 * N participants, K riding stages.
 * Each participant swims once, then goes through K riding stages.
 * Swimming time: 2 * 10^7 - Wi
 * Riding time: 2 * 10^7 - Ri - Sj
 * 
 * At each stage k, N horses are available with ridingness Sj = f(k, j) = 3*Ak^2 + 5*Ak*Bj + 2*Bj^2.
 * Each horse can only be used once. Since there are N horses at the start of each of the K stages,
 * this implies a total of N*K horses are available, each used exactly once.
 * 
 * The sorting order for participants at the start of any stage (including after swimming)
 * is determined by (finish_time, -strength).
 * 
 * Key observation:
 * For any stage k, the ridingness of the j-th horse f(k, j) = 3*Ak^2 + 5*Ak*Bj + 2*Bj^2
 * increases as Bj increases (since Ak >= 1, Bj >= 1).
 * Thus, the best horse in any stage is always the one with the largest Bj.
 * The order of horses by ridingness (descending) is the same for all stages.
 */

struct Participant {
    int id;
    long long W, R, P;
    long long T;
};

// Sort participants by finish time ascending, then strength descending.
bool compareParticipants(const Participant& a, const Participant& b) {
    if (a.T != b.T) return a.T < b.T;
    return a.P > b.P;
}

int main() {
    // Optimize I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, K;
    if (!(cin >> N >> K)) return 0;

    vector<Participant> participants(N);
    for (int i = 0; i < N; ++i) {
        participants[i].id = i + 1;
        cin >> participants[i].W >> participants[i].R >> participants[i].P;
        // Swimming stage completion time
        participants[i].T = 20000000LL - participants[i].W;
    }

    vector<long long> A(K);
    for (int i = 0; i < K; ++i) {
        cin >> A[i];
    }

    vector<long long> B(N);
    for (int i = 0; i < N; ++i) {
        cin >> B[i];
    }

    // Pre-calculate the order of horses (by Bj descending)
    vector<int> horse_order(N);
    iota(horse_order.begin(), horse_order.end(), 0);
    sort(horse_order.begin(), horse_order.end(), [&](int i, int j) {
        return B[i] > B[j];
    });

    // Initial sorting of participants by their swimming finish time
    sort(participants.begin(), participants.end(), compareParticipants);

    // Simulation of K riding stages
    for (int k = 0; k < K; ++k) {
        long long Ak = A[k];
        long long Ak_sq_3 = 3LL * Ak * Ak;
        
        // In each stage, participants who reach the start of the stage first (based on sorted order)
        // take the best available horses (based on horse_order).
        for (int m = 0; m < N; ++m) {
            int participant_idx = m; // Using index in the already-sorted participants list
            int horse_idx = horse_order[m]; // Using index from horse_order
            
            long long Bj = B[horse_idx];
            long long Sj = Ak_sq_3 + 5LL * Ak * Bj + 2LL * Bj * Bj;
            
            participants[participant_idx].T += (20000000LL - participants[participant_idx].R - Sj);
        }
        
        // Re-sort participants for the next stage (or for the final finish order)
        sort(participants.begin(), participants.end(), compareParticipants);
    }

    // Output the final order of participants
    for (int i = 0; i < N; ++i) {
        cout << participants[i].id << (i == N - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}

