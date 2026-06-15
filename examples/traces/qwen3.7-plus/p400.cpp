#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

struct TeamData {
    int rank;
    string team_name;
    long long solved;
    long long penalty;
    vector<string> results;
    vector<long long> U;
};

vector<string> split(const string& s) {
    vector<string> res;
    stringstream ss(s);
    string token;
    while (ss >> token) {
        res.push_back(token);
    }
    return res;
}

string join(const vector<string>& v, int start, int end) {
    string res = "";
    for (int i = start; i <= end; ++i) {
        if (i > start) res += " ";
        res += v[i];
    }
    return res;
}

void solve() {
    string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;
        vector<string> tokens = split(line);
        if (tokens.empty()) continue;
        
        int k_problems = 0;
        bool found_penalty = false;
        for (size_t i = 0; i < tokens.size(); ++i) {
            if (tokens[i] == "Penalty") {
                found_penalty = true;
                k_problems = tokens.size() - 1 - i;
                break;
            }
        }
        if (!found_penalty) continue;

        vector<TeamData> teams;
        while (getline(cin, line)) {
            vector<string> t_line = split(line);
            if (t_line.size() >= (size_t)(k_problems + 3)) {
                bool ok = true;
                int r = -1, p = -1, s = -1;
                try {
                    r = stoi(t_line[0]);
                    p = stoi(t_line[t_line.size() - k_problems - 1]);
                    s = stoi(t_line[t_line.size() - k_problems - 2]);
                } catch (...) {
                    ok = false;
                }
                if (ok) {
                    TeamData td;
                    td.rank = r;
                    td.team_name = join(t_line, 1, t_line.size() - k_problems - 3);
                    td.solved = s;
                    td.penalty = p;
                    td.results = vector<string>(t_line.end() - k_problems, t_line.end());
                    
                    for (string res : td.results) {
                        if (res == ".") {
                            td.U.push_back(241);
                        } else if (!res.empty() && res[0] == '-') {
                            int x = stoi(res.substr(1));
                            td.U.push_back(241 + 20 * x);
                        }
                    }
                    sort(td.U.begin(), td.U.end());
                    teams.push_back(td);
                    continue;
                }
            }
            
            if (!teams.empty() && t_line.size() == 1) {
                long long balloons = stoll(t_line[0]);
                string my_team;
                getline(cin, my_team);
                
                if (!my_team.empty()) {
                    my_team.erase(0, my_team.find_first_not_of(" \t\r\n"));
                    if (my_team.find_last_not_of(" \t\r\n") != string::npos) {
                        my_team.erase(my_team.find_last_not_of(" \t\r\n") + 1);
                    } else {
                        my_team.clear();
                    }
                }
                
                TeamData* my_team_data = nullptr;
                for (auto& t : teams) {
                    if (t.team_name == my_team) {
                        my_team_data = &t;
                        break;
                    }
                }
                if (!my_team_data) {
                    TeamData td;
                    td.team_name = my_team;
                    td.solved = 0;
                    td.penalty = 0;
                    for (int i = 0; i < k_problems; ++i) {
                        td.U.push_back(241);
                    }
                    teams.push_back(td);
                    my_team_data = &teams.back();
                }
                
                long long S_my = my_team_data->solved;
                long long P_my = my_team_data->penalty;
                vector<long long> U_my = my_team_data->U;
                
                long long min_rank = 1e18;
                long long max_rank = -1;
                
                for (long long c_my = 0; c_my <= min((long long)balloons, (long long)U_my.size()); ++c_my) {
                    long long rem_B = balloons - c_my;
                    long long max_other_U = 0;
                    for (const auto& t : teams) {
                        if (t.team_name == my_team) continue;
                        max_other_U += t.U.size();
                    }
                    if (rem_B > max_other_U) continue;
                    
                    long long S_target = S_my + c_my;
                    long long P_target_min = P_my;
                    for (int i = 0; i < c_my; ++i) P_target_min += U_my[i];
                    long long P_target_max = (c_my > 0) ? 1000000000000000LL : P_my;
                    
                    vector<long long> dp_min(rem_B + 1, 1e18);
                    dp_min[0] = 0;
                    for (const auto& t : teams) {
                        if (t.team_name == my_team) continue;
                        vector<long long> next_dp(rem_B + 1, 1e18);
                        for (int j = 0; j <= rem_B; ++j) {
                            if (dp_min[j] == 1e18) continue;
                            for (long long c = 0; c <= (long long)t.U.size(); ++c) {
                                if (j + c > rem_B) break;
                                long long cost = 0;
                                if (t.solved + c > S_target) {
                                    cost = 1;
                                } else if (t.solved + c == S_target) {
                                    if (c == 0) {
                                        if (t.penalty < P_target_min) cost = 1;
                                    } else {
                                        cost = 0;
                                    }
                                }
                                next_dp[j + c] = min(next_dp[j + c], dp_min[j] + cost);
                            }
                        }
                        dp_min = next_dp;
                    }
                    if (dp_min[rem_B] != 1e18) {
                        min_rank = min(min_rank, dp_min[rem_B] + 1);
                    }
                    
                    vector<long long> dp_max(rem_B + 1, -1);
                    dp_max[0] = 0;
                    for (const auto& t : teams) {
                        if (t.team_name == my_team) continue;
                        vector<long long> next_dp(rem_B + 1, -1);
                        for (int j = 0; j <= rem_B; ++j) {
                            if (dp_max[j] == -1) continue;
                            for (long long c = 0; c <= (long long)t.U.size(); ++c) {
                                if (j + c > rem_B) break;
                                long long cost = 0;
                                if (t.solved + c > S_target) {
                                    cost = 1;
                                } else if (t.solved + c == S_target) {
                                    long long min_p = t.penalty;
                                    for (int k = 0; k < c; ++k) min_p += t.U[k];
                                    if (min_p < P_target_max) {
                                        cost = 1;
                                    }
                                }
                                next_dp[j + c] = max(next_dp[j + c], dp_max[j] + cost);
                            }
                        }
                        dp_max = next_dp;
                    }
                    if (dp_max[rem_B] != -1) {
                        max_rank = max(max_rank, dp_max[rem_B] + 1);
                    }
                }
                
                if (min_rank == 1e18) min_rank = 1;
                if (max_rank == -1) max_rank = teams.size();
                
                cout << min_rank << " " << max_rank << "\n";
                break;
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
