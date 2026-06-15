#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

struct State {
    int seg_idx;
    vector<char> st;
    int used_U;
    int add_out;
    int take_out;

    bool operator>(const State& other) const {
        if (add_out != other.add_out) return add_out > other.add_out;
        if (take_out != other.take_out) return take_out > other.take_out;
        if (seg_idx != other.seg_idx) return seg_idx > other.seg_idx;
        if (used_U != other.used_U) return used_U > other.used_U;
        return st > other.st;
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string input; if (!(cin >> input)) return 0;
    vector<string> segs; vector<int> star_indices;
    string cur = "";
    for (int i = 0; i < (int)input.size(); ++i) {
        if (input[i] == '*') { segs.push_back(cur); star_indices.push_back(i); cur = ""; }
        else cur += input[i];
    }
    segs.push_back(cur);
    vector<char> U; vector<int> U_indices; vector<char> st_for_U;
    for (int i = 0; i < (int)input.size(); ++i) {
        if (input[i] >= 'a' && input[i] <= 'z') st_for_U.push_back(input[i]);
        else if (input[i] >= 'A' && input[i] <= 'Z') {
            char color = input[i] + ('a' - 'A');
            bool found = false;
            for (int j = (int)st_for_U.size() - 1; j >= 0; --j) {
                if (st_for_U[j] == color) { st_for_U.erase(st_for_U.begin() + j); found = true; break; }
            }
            if (!found) { U.push_back(color); U_indices.push_back(i); }
        }
    }
    int n_add_in = 0, n_take_in = 0;
    for (char c : input) { if (c >= 'a' && c <= 'z') n_add_in++; else if (c >= 'A' && c <= 'Z') n_take_in++; }
    priority_queue<State, vector<State>, greater<State>> pq;
    map<pair<pair<int, int>, vector<char>>, int> min_add_out;
    vector<char> start_st; bool possible = true;
    for (char c : segs[0]) {
        if (c >= 'a' && c <= 'z') start_st.push_back(c);
        else {
            char color = c + ('a' - 'A');
            if (!start_st.empty() && start_st.back() == color) start_st.pop_back();
            else { possible = false; break; }
        }
    }
    if (possible) { pq.push({0, start_st, 0, 0, 0}); min_add_out[{{0, 0}, start_st}] = 0; }
    int min_total_add = -1; int n_segs = segs.size();
    while (!pq.empty()) {
        State cur_s = pq.top(); pq.pop();
        if (cur_s.add_out > min_add_out[{{cur_s.seg_idx, cur_s.used_U}, cur_s.st}]) continue;
        if (cur_s.seg_idx == n_segs - 1) {
            if (cur_s.st.empty() && n_add_in + cur_s.add_out == n_take_in + cur_s.take_out) {
                int total_add = n_add_in + cur_s.add_out;
                if (min_total_add == -1 || total_add < min_total_add) min_total_add = total_add;
            }
            continue;
        }
        int star_idx = star_indices[cur_s.seg_idx];
        for (int m = 0; m <= (int)cur_s.st.size(); ++m) {
            for (int n = 0; cur_s.used_U + n <= (int)U.size(); ++n) {
                bool ok = true;
                for (int i = 0; i < n; ++i) if (U_indices[cur_s.used_U + i] < star_idx) { ok = false; break; }
                if (!ok) break;
                vector<char> next_st;
                for (int i = 0; i < (int)cur_s.st.size() - m; ++i) next_st.push_back(cur_s.st[i]);
                for (int i = 0; i < n; ++i) next_st.push_back(U[cur_s.used_U + n - 1 - i]);
                vector<char> st_after_seg = next_st; bool seg_possible = true;
                for (char c : segs[cur_s.seg_idx + 1]) {
                    if (c >= 'a' && c <= 'z') st_after_seg.push_back(c);
                    else {
                        char color = c + ('a' - 'A');
                        if (!st_after_seg.empty() && st_after_seg.back() == color) st_after_seg.pop_back();
                        else { seg_possible = false; break; }
                    }
                }
                if (seg_possible) {
                    int next_add = cur_s.add_out + n, next_take = cur_s.take_out + m, next_used = cur_s.used_U + n;
                    if (min_add_out.find({{cur_s.seg_idx + 1, next_used}, st_after_seg}) == min_add_out.end() || next_add < min_add_out[{{cur_s.seg_idx + 1, next_used}, st_after_seg}]) {
                        min_add_out[{{cur_s.seg_idx + 1, next_used}, st_after_seg}] = next_add;
                        pq.push({cur_s.seg_idx + 1, st_after_seg, next_used, next_add, next_take});
                    }
                }
            }
        }
    }
    cout << min_total_add << endl;
    return 0;
}

