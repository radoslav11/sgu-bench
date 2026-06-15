#include <bits/stdc++.h>
using namespace std;

struct Node {
    int pushes, moves, bx, by, sx, sy;
    bool operator<(const Node& other) const {
        if (pushes != other.pushes) {
            return pushes > other.pushes;
        }
        return moves > other.moves;
    }
};

int main() {
    vector<string> grid;
    string line;
    while (getline(cin, line)) {
        if (!line.empty()) {
            grid.push_back(line);
        }
    }
    
    if (grid.empty()) {
        cout << "Impossible." << endl;
        return 0;
    }
    
    int H = grid.size();
    int W = grid[0].size();
    
    int sokoban_r = -1, sokoban_c = -1;
    int box_r = -1, box_c = -1;
    int dest_r = -1, dest_c = -1;
    
    for (int r = 0; r < H; r++) {
        for (int c = 0; c < W; c++) {
            if (grid[r][c] == '@') {
                sokoban_r = r;
                sokoban_c = c;
            } else if (grid[r][c] == '$') {
                box_r = r;
                box_c = c;
            } else if (grid[r][c] == '.') {
                dest_r = r;
                dest_c = c;
            }
        }
    }
    
    if (box_r == dest_r && box_c == dest_c) {
        cout << "" << endl;
        return 0;
    }
    
    vector<pair<int, int>> dirs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    vector<char> move_chars = {'u', 'd', 'l', 'r'};
    
    auto state_hash = [](const tuple<int, int, int, int>& s) {
        return (size_t)(get<0>(s) << 21) ^ (size_t)(get<1>(s) << 14) ^ 
               (size_t)(get<2>(s) << 7) ^ (size_t)get<3>(s);
    };
    
    using State = tuple<int, int, int, int>;
    unordered_map<State, pair<int, int>, decltype(state_hash)> dist(100000, state_hash);
    unordered_map<State, pair<State, char>, decltype(state_hash)> parent(100000, state_hash);
    
    auto start_state = make_tuple(box_r, box_c, sokoban_r, sokoban_c);
    dist[start_state] = {0, 0};
    
    priority_queue<Node> pq;
    pq.push({0, 0, box_r, box_c, sokoban_r, sokoban_c});
    
    State final_state = make_tuple(-1, -1, -1, -1);
    bool found = false;
    
    while (!pq.empty()) {
        Node cur = pq.top();
        pq.pop();
        
        auto cur_state = make_tuple(cur.bx, cur.by, cur.sx, cur.sy);
        auto& cur_dist = dist[cur_state];
        
        if (cur.pushes > cur_dist.first || (cur.pushes == cur_dist.first && cur.moves > cur_dist.second)) {
            continue;
        }
        
        if (cur.bx == dest_r && cur.by == dest_c) {
            final_state = cur_state;
            found = true;
            break;
        }
        
        for (int i = 0; i < 4; i++) {
            int dx = dirs[i].first;
            int dy = dirs[i].second;
            char move_char = move_chars[i];
            
            int nsx = cur.sx + dx;
            int nsy = cur.sy + dy;
            
            if (nsx < 0 || nsx >= H || nsy < 0 || nsy >= W || grid[nsx][nsy] == '#') {
                continue;
            }
            
            State new_state;
            int npushes = cur.pushes;
            int nmoves = cur.moves + 1;
            char move_used = move_char;
            
            if (nsx == cur.bx && nsy == cur.by) {
                int nnbx = cur.bx + dx;
                int nnby = cur.by + dy;
                
                if (nnbx < 0 || nnbx >= H || nnby < 0 || nnby >= W || grid[nnbx][nnby] == '#') {
                    continue;
                }
                
                npushes = cur.pushes + 1;
                move_used = toupper(move_char);
                new_state = make_tuple(nnbx, nnby, cur.bx, cur.by);
            } else {
                new_state = make_tuple(cur.bx, cur.by, nsx, nsy);
            }
            
            if (dist.find(new_state) == dist.end() ||
                npushes < dist[new_state].first ||
                (npushes == dist[new_state].first && nmoves < dist[new_state].second)) {
                
                dist[new_state] = {npushes, nmoves};
                parent[new_state] = {cur_state, move_used};
                pq.push({npushes, nmoves, get<0>(new_state), get<1>(new_state), get<2>(new_state), get<3>(new_state)});
            }
        }
    }
    
    if (!found) {
        cout << "Impossible." << endl;
        return 0;
    }
    
    string path;
    State cur = final_state;
    while (parent.find(cur) != parent.end()) {
        auto [prev_state, move_char] = parent[cur];
        path += move_char;
        cur = prev_state;
    }
    
    reverse(path.begin(), path.end());
    cout << path << endl;
    
    return 0;
}
