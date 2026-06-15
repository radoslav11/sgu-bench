#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<string> maze;
    string line;
    while(getline(cin, line)){
        maze.push_back(line);
    }
    
    int H = maze.size();
    int W = maze[0].size();
    
    int sr, sc, br, bc, dr, dc;
    
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            if(maze[i][j] == '@') sr = i, sc = j;
            if(maze[i][j] == '$') br = i, bc = j;
            if(maze[i][j] == '.') dr = i, dc = j;
        }
    }
    
    // State: (sr, sc, br, bc)
    // Cost: (pushes, moves)
    map<tuple<int,int,int,int>, pair<int,int>> visited;
    map<tuple<int,int,int,int>, tuple<int,int,int,int>> parent;
    map<tuple<int,int,int,int>, char> parent_move;
    
    priority_queue<pair<pair<int,int>, tuple<int,int,int,int>>, 
                   vector<pair<pair<int,int>, tuple<int,int,int,int>>>,
                   greater<pair<pair<int,int>, tuple<int,int,int,int>>>> pq;
    
    auto start = make_tuple(sr, sc, br, bc);
    pq.push({{0, 0}, start});
    visited[start] = {0, 0};
    
    int dr_dir[] = {-1, 1, 0, 0};
    int dc_dir[] = {0, 0, 1, -1};
    char dir_char[] = {'u', 'd', 'r', 'l'};
    
    auto goal = make_tuple(br, bc, dr, dc);  // box at destination
    
    while(!pq.empty()){
        auto [cost, state] = pq.top();
        pq.pop();
        
        auto [pushes, moves] = cost;
        auto [s_r, s_c, b_r, b_c] = state;
        
        if(state == goal) break;
        
        if(visited.count(state) && visited[state] < cost) continue;
        
        for(int d = 0; d < 4; d++){
            int ns_r = s_r + dr_dir[d];
            int ns_c = s_c + dc_dir[d];
            
            if(ns_r < 0 || ns_r >= H || ns_c < 0 || ns_c >= W) continue;
            if(maze[ns_r][ns_c] == '#') continue;
            
            int nb_r = b_r, nb_c = b_c;
            int new_pushes = pushes;
            
            // Check if we're moving into the box
            if(ns_r == b_r && ns_c == b_c){
                // Push the box
                nb_r = b_r + dr_dir[d];
                nb_c = b_c + dc_dir[d];
                
                if(nb_r < 0 || nb_r >= H || nb_c < 0 || nb_c >= W) continue;
                if(maze[nb_r][nb_c] == '#') continue;
                
                new_pushes = pushes + 1;
            }
            
            auto new_state = make_tuple(ns_r, ns_c, nb_r, nb_c);
            int new_moves = moves + 1;
            
            if(!visited.count(new_state) || visited[new_state] > make_pair(new_pushes, new_moves)){
                visited[new_state] = {new_pushes, new_moves};
                parent[new_state] = state;
                parent_move[new_state] = (new_pushes > pushes) ? toupper(dir_char[d]) : dir_char[d];
                pq.push({{new_pushes, new_moves}, new_state});
            }
        }
    }
    
    if(!visited.count(goal)){
        cout << "Impossible." << endl;
        return 0;
    }
    
    string result = "";
    auto curr = goal;
    while(curr != start){
        result = parent_move[curr] + result;
        curr = parent[curr];
    }
    
    cout << result << endl;
    
    return 0;
}
