#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<string> grid(N);
    for(int i = 0; i < N; i++){
        cin >> grid[i];
    }
    
    int pr, pc, br, bc;
    cin >> pr >> pc >> br >> bc;
    pr--; pc--; br--; bc--;
    
    int flow_dr[] = {0, 0, 0, 1, -1};
    int flow_dc[] = {0, -1, 1, 0, 0};
    
    map<tuple<int,int,int,int>, double> dist;
    priority_queue<tuple<double,int,int,int,int>, vector<tuple<double,int,int,int,int>>, greater<tuple<double,int,int,int,int>>> pq;
    
    pq.push({0.0, pr, pc, br, bc});
    dist[{pr, pc, br, bc}] = 0.0;
    
    double answer = 1e9;
    
    while(!pq.empty()){
        auto [time, pr, pc, br, bc] = pq.top();
        pq.pop();
        
        if(pr < 0 || pr >= N || pc < 0 || pc >= M) {
            answer = min(answer, time);
            continue;
        }
        
        if(pr == br && pc == bc) {
            answer = min(answer, time);
            continue;
        }
        
        auto key = make_tuple(pr, pc, br, bc);
        if(dist.count(key) && dist[key] < time) continue;
        
        for(int move = 0; move < 5; move++){
            int flow_id = grid[pr][pc] - '0';
            int pdr = flow_dr[flow_id] + flow_dr[move];
            int pdc = flow_dc[flow_id] + flow_dc[move];
            
            int bdr = flow_dr[grid[br][bc] - '0'];
            int bdc = flow_dc[grid[br][bc] - '0'];
            
            int m = max(abs(pdr), abs(pdc));
            if(m == 0) continue;
            
            double t = 1.0 / m;
            int npr = pr + (pdr > 0 ? 1 : (pdr < 0 ? -1 : 0));
            int npc = pc + (pdc > 0 ? 1 : (pdc < 0 ? -1 : 0));
            int nbr = br + bdr;
            int nbc = bc + bdc;
            
            if(nbr < 0) nbr = 0;
            if(nbr >= N) nbr = N-1;
            if(nbc < 0) nbc = 0;
            if(nbc >= M) nbc = M-1;
            
            auto nkey = make_tuple(npr, npc, nbr, nbc);
            if(!dist.count(nkey) || dist[nkey] > time + t){
                dist[nkey] = time + t;
                pq.push({time + t, npr, npc, nbr, nbc});
            }
        }
    }
    
    if(answer >= 1e9) cout << "SOS\n";
    else cout << fixed << setprecision(2) << answer << "\n";
    
    return 0;
}
