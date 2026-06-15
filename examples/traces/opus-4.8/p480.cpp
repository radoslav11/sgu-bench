#include <bits/stdc++.h>
using namespace std;

static const long long BIG = (long long)4e18;
static const long long CAP = (long long)2e9;

int c, k;
vector<vector<int>> a; // a[i][j]

// returns deficit (0 if feasible) using base[j]+add[j]; if usedOut given, fill used counts
long long simulate(const vector<long long>& base, const int* add, vector<long long>* usedOut){
    long long slots = 8; // slots at level k-1
    for(int j = k-1; j >= 0; j--){
        long long av = base[j] + (add ? (long long)add[j] : 0LL);
        long long used = min(slots, av);
        if(usedOut) (*usedOut)[j] = used;
        if(j > 0){
            slots = (slots - used) * 8;
            if(slots > CAP){
                return BIG;
            }
        } else {
            return slots - used; // deficit at level 0
        }
    }
    return 0;
}

int main(){
    if(!(cin >> c >> k)) return 0;
    a.assign(c, vector<int>(k, 0));
    for(int i = 0; i < c; i++)
        for(int j = 0; j < k; j++)
            cin >> a[i][j];

    // full availability
    vector<long long> full(k, 0);
    for(int i = 0; i < c; i++)
        for(int j = 0; j < k; j++)
            full[j] += a[i][j];

    long long fullDef = simulate(full, nullptr, nullptr);
    if(fullDef != 0){
        cout << "NO SOLUTION\n";
        return 0;
    }

    // greedy select colors
    vector<long long> sel(k, 0);
    vector<bool> usedColor(c, false);
    vector<int> chosen;

    while(true){
        long long curDef = simulate(sel, nullptr, nullptr);
        if(curDef == 0) break;
        int best = -1;
        long long bestDef = BIG + 1;
        for(int i = 0; i < c; i++){
            if(usedColor[i]) continue;
            long long d = simulate(sel, a[i].data(), nullptr);
            if(d < bestDef){
                bestDef = d;
                best = i;
            }
        }
        if(best < 0) break; // shouldn't happen
        usedColor[best] = true;
        chosen.push_back(best);
        for(int j = 0; j < k; j++) sel[j] += a[best][j];
    }

    // construct: compute used counts per level
    vector<long long> usedLevel(k, 0);
    simulate(sel, nullptr, &usedLevel);

    // distribute among chosen colors
    // remaining counts available per chosen color
    vector<array<long long,3>> out; // color(1-indexed), size index, count

    for(int j = 0; j < k; j++){
        long long need = usedLevel[j];
        if(need <= 0) continue;
        for(int idx = 0; idx < (int)chosen.size() && need > 0; idx++){
            int ci = chosen[idx];
            long long t = min(need, (long long)a[ci][j]);
            if(t > 0){
                out.push_back({(long long)(ci+1), (long long)j, t});
                need -= t;
            }
        }
    }

    cout << out.size() << "\n";
    for(auto &r : out){
        cout << r[0] << " " << r[1] << " " << r[2] << "\n";
    }
    return 0;
}
