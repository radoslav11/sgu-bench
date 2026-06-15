// global
int gL, gN, gD;
int gbound[12];
// allowed determined by mode and D
// mode 0 = A (no constraint), mode 1 = B (bad only)
int gmode;

bool isGood(int pos, int d, int D){
  if(pos < gN){
    int dd = d - D;
    if(dd<0||dd>9) return false;
    if(pos==0 && dd==0) return false;
    return true;
  } else {
    int dd = d + D;
    if(dd<0||dd>9) return false;
    return true;
  }
}

bool allowed(int pos,int d){
  if(pos==0 && d==0) return false;
  if(gmode==1){ if(isGood(pos,d,gD)) return false; }
  return true;
}

ll memo[12][200][2]; // pos, diffOffset, tight ; but tight true not memoized
bool vis[12][200][2];
int OFF; // = 9*gN
ll rec(int pos, int diff, int tight){
  if(pos==gL){ return diff==gD?1:0; }
  if(!tight && vis[pos][diff+OFF][tight]) return memo[pos][diff+OFF][tight];
  int hi = tight ? gbound[pos] : 9;
  ll res=0;
  for(int d=0; d<=hi; d++){
    if(!allowed(pos,d)) continue;
    int nd = pos<gN ? diff+d : diff-d;
    int nt = (tight && d==hi)?1:0;
    res += rec(pos+1, nd, nt);
  }
  if(!tight){ vis[pos][diff+OFF][tight]=true; memo[pos][diff+OFF][tight]=res; }
  return res;
}
