#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Record {
    char type;
    string name;
    int k;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    vector<Record> log(N);
    set<string> U;
    for (int i = 0; i < N; ++i) {
        cin >> log[i].type;
        if (log[i].type == '=') {
            cin >> log[i].k;
        } else {
            cin >> log[i].name;
            U.insert(log[i].name);
        }
    }

    vector<string> F;
    for (char c1 = 'a'; c1 <= 'z' && F.size() < 300; ++c1) {
        string s(1, c1);
        if (U.count(s) == 0) F.push_back(s);
        for (char c2 = 'a'; c2 <= 'z' && F.size() < 300; ++c2) {
            s = string(1, c1) + c2;
            if (U.count(s) == 0) F.push_back(s);
        }
    }

    vector<string> all_names;
    vector<bool> is_fresh_flag;
    for (const string& name : U) {
        all_names.push_back(name);
        is_fresh_flag.push_back(false);
    }
    for (const string& name : F) {
        all_names.push_back(name);
        is_fresh_flag.push_back(true);
    }

    int V = all_names.size();
    unordered_map<string, int> name_to_id;
    for (int v = 0; v < V; ++v) {
        name_to_id[all_names[v]] = v;
    }

    vector<bool> in_S(V, false);
    vector<string> out;

    auto get_cost = [&](int j, int initial_s, int start_idx) {
        int cost = 0;
        int s = initial_s;
        for (int k = start_idx; k < N; ++k) {
            if (log[k].name == all_names[j]) {
                int op = (log[k].type == '+') ? 1 : -1;
                if (op == 1) {
                    if (s == 1) cost++;
                    else s = 1;
                } else {
                    if (s == 1) s = 0;
                    else cost++;
                }
            }
        }
        return cost;
    };

    for (int i = 0; i < N; ++i) {
        if (log[i].type == '+') {
            int j = name_to_id[log[i].name];
            if (in_S[j]) {
                out.push_back("- " + log[i].name);
            }
            out.push_back("+ " + log[i].name);
            in_S[j] = true;
        } else if (log[i].type == '-') {
            int j = name_to_id[log[i].name];
            if (!in_S[j]) {
                out.push_back("+ " + log[i].name);
            }
            out.push_back("- " + log[i].name);
            in_S[j] = false;
        } else if (log[i].type == '=') {
            int c = 0;
            for (int v = 0; v < V; ++v) {
                if (in_S[v]) c++;
            }

            if (c < log[i].k) {
                int need = log[i].k - c;
                struct NameInfo {
                    int id;
                    int value;
                    bool is_fresh;
                };
                vector<NameInfo> candidates;
                for (int v = 0; v < V; ++v) {
                    if (!in_S[v]) {
                        int cost0 = get_cost(v, 0, i + 1);
                        int cost1 = get_cost(v, 1, i + 1);
                        candidates.push_back({v, cost0 - cost1, is_fresh_flag[v]});
                    }
                }
                sort(candidates.begin(), candidates.end(), [](const NameInfo& a, const NameInfo& b) {
                    if (a.value != b.value) return a.value > b.value;
                    return a.is_fresh < b.is_fresh;
                });
                for (int t = 0; t < need; ++t) {
                    out.push_back("+ " + all_names[candidates[t].id]);
                    in_S[candidates[t].id] = true;
                }
            } else if (c > log[i].k) {
                int need = c - log[i].k;
                struct NameInfo {
                    int id;
                    int value;
                    bool is_fresh;
                };
                vector<NameInfo> candidates;
                for (int v = 0; v < V; ++v) {
                    if (in_S[v]) {
                        int cost0 = get_cost(v, 0, i + 1);
                        int cost1 = get_cost(v, 1, i + 1);
                        candidates.push_back({v, cost0 - cost1, is_fresh_flag[v]});
                    }
                }
                sort(candidates.begin(), candidates.end(), [](const NameInfo& a, const NameInfo& b) {
                    if (a.value != b.value) return a.value < b.value;
                    return a.is_fresh > b.is_fresh;
                });
                for (int t = 0; t < need; ++t) {
                    out.push_back("- " + all_names[candidates[t].id]);
                    in_S[candidates[t].id] = false;
                }
            }
            out.push_back("= " + to_string(log[i].k));
        }
    }

    cout << out.size() << "\n";
    for (const string& s : out) {
        cout << s << "\n";
    }

    return 0;
}
