#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct State {
    int br, bc;
    int sr, sc;
    int pushes;
    int moves;
    int id;
    
    bool operator>(const State& other) const {
        if (pushes != other.pushes) return pushes > other.pushes;
        return moves > other.moves;
    }
};

struct Transition {
    int parent_id;
    string path;
    char push_move;
};

static int dist_arr[105][105];
static int parent_r[105][105];
static int parent_c[105][105];
static char move_to[105][105];
static int q_r[10500], q_c[10500];
static int gen[105][105];
static int current_gen = 0;

int bfs_fast(int sr, int sc, int pr, int pc, int br, int bc, string& path, const vector<string>& grid, int R) {
    current_gen++;
    int head = 0, tail = 0;
    q_r[tail] = sr;
    q_c[tail] = sc;
    tail++;
    dist_arr[sr][sc] = 0;
    gen[sr][sc] = current_gen;
    
    int dr[] = {1, 0, 0, -1};
    int dc[] = {0, -1, 1, 0};
    char mc[] = {'d', 'l', 'r', 'u'};
    
    while(head < tail) {
        int r = q_r[head];
        int c = q_c[head];
        head++;
        
        if (r == pr && c == pc) {
            string p = "";
            int curr_r = pr, curr_c = pc;
            while (curr_r != sr || curr_c != sc) {
                p += move_to[curr_r][curr_c];
                int pr_r = parent_r[curr_r][curr_c];
                int pr_c = parent_c[curr_r][curr_c];
                curr_r = pr_r;
                curr_c = pr_c;
            }
            reverse(p.begin(), p.end());
            path = p;
            return dist_arr[pr][pc];
        }
        
        int d_rc = dist_arr[r][c] + 1;
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (nr >= 0 && nr < R && nc >= 0 && nc < (int)grid[nr].size()) {
                if (grid[nr][nc] != '#' && !(nr == br && nc == bc)) {
                    if (gen[nr][nc] != current_gen) {
                        gen[nr][nc] = current_gen;
                        dist_arr[nr][nc] = d_rc;
                        parent_r[nr][nc] = r;
                        parent_c[nr][nc] = c;
                        move_to[nr][nc] = mc[i];
                        q_r[tail] = nr;
                        q_c[tail] = nc;
                        tail++;
                    }
                }
            }
        }
    }
    return -1;
}

uint32_t get_key(int br, int bc, int sr, int sc) {
    return ((uint32_t)br << 24) | ((uint32_t)bc << 16) | ((uint32_t)sr << 8) | (sc);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    vector<string> grid;
    string line;
    int start_br = -1, start_bc = -1;
    int start_sr = -1, start_sc = -1;
    int dest_r = -1, dest_c = -1;
    
    while (getline(cin, line)) {
        if (line.empty()) continue;
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        grid.push_back(line);
    }
    
    int R = grid.size();
    bool found_box = false;
    
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < (int)grid[r].size(); ++c) {
            if (grid[r][c] == '@') {
                start_sr = r; start_sc = c;
            } else if (grid[r][c] == '.') {
                dest_r = r; dest_c = c;
            } else if (grid[r][c] != '#' && grid[r][c] != ' ' && grid[r][c] != '@' && grid[r][c] != '.') {
                start_br = r; start_bc = c;
                found_box = true;
            }
        }
    }
    
    if (!found_box) {
        for (int r = 0; r < R; ++r) {
            for (int c = 0; c < (int)grid[r].size(); ++c) {
                if (grid[r][c] == '*') {
                    start_br = r; start_bc = c;
                    found_box = true;
                }
            }
        }
    }
    
    if (start_br == -1 || start_sr == -1 || dest_r == -1) {
        cout << "Impossible.\n";
        return 0;
    }
    
    int dr_box[] = {-1, 0, 1, 0};
    int dc_box[] = {0, 1, 0, -1};
    char push_char[] = {'U', 'R', 'D', 'L'};
    
    priority_queue<State, vector<State>, greater<State>> pq;
    unordered_map<uint32_t, pair<int, int>> best;
    best.reserve(100000);
    
    vector<State> states;
    vector<Transition> trans;
    
    State initial_state = {start_br, start_bc, start_sr, start_sc, 0, 0, 0};
    states.push_back(initial_state);
    trans.push_back({-1, "", ' '});
    pq.push(initial_state);
    best[get_key(start_br, start_bc, start_sr, start_sc)] = {0, 0};
    
    int final_id = -1;
    
    while (!pq.empty()) {
        State curr = pq.top();
        pq.pop();
        
        uint32_t key = get_key(curr.br, curr.bc, curr.sr, curr.sc);
        auto it = best.find(key);
        if (it != best.end()) {
            if (curr.pushes > it->second.first || (curr.pushes == it->second.first && curr.moves > it->second.second)) {
                continue;
            }
        }
        
        if (curr.br == dest_r && curr.bc == dest_c) {
            final_id = curr.id;
            break;
        }
        
        for (int i = 0; i < 4; ++i) {
            int nbr = curr.br + dr_box[i];
            int nbc = curr.bc + dc_box[i];
            
            if (nbr >= 0 && nbr < R && nbc >= 0 && nbc < (int)grid[nbr].size() && grid[nbr][nbc] != '#') {
                int pr = curr.br - dr_box[i];
                int pc = curr.bc - dc_box[i];
                
                if (pr >= 0 && pr < R && pc >= 0 && pc < (int)grid[pr].size() && grid[pr][pc] != '#') {
                    string path;
                    int dist = bfs_fast(curr.sr, curr.sc, pr, pc, curr.br, curr.bc, path, grid, R);
                    if (dist != -1) {
                        int n_pushes = curr.pushes + 1;
                        int n_moves = curr.moves + dist + 1;
                        
                        uint32_t n_key = get_key(nbr, nbc, curr.br, curr.bc);
                        auto it_n = best.find(n_key);
                        if (it_n == best.end() || n_pushes < it_n->second.first || 
                           (n_pushes == it_n->second.first && n_moves < it_n->second.second)) {
                            
                            best[n_key] = {n_pushes, n_moves};
                            State next_state = {nbr, nbc, curr.br, curr.bc, n_pushes, n_moves, (int)states.size()};
                            states.push_back(next_state);
                            trans.push_back({curr.id, path, push_char[i]});
                            pq.push(next_state);
                        }
                    }
                }
            }
        }
    }
    
    if (final_id != -1) {
        string ans = "";
        int curr = final_id;
        while (curr != 0) {
            ans = trans[curr].path + trans[curr].push_move + ans;
            curr = trans[curr].parent_id;
        }
        cout << ans << "\n";
    } else {
        cout << "Impossible.\n";
    }
    
    return 0;
}
