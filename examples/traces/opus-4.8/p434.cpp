vector<int> sumArr(1<<m);
sumArr[0]=0;
for(int mask=1; mask<(1<<m); mask++){
  int low = __builtin_ctz(mask);
  sumArr[mask]=sumArr[mask ^ (1<<low)] + v[low];
}
vector<signed char> dp(1<<m, -1);
dp[0]=0;
for(int mask=1; mask<(1<<m); mask++){
  int i = __builtin_ctz(mask);
  int rest = mask ^ (1<<mask? ...)
  int best=-1;
  int ibit = 1<<i;
  int restMask = mask ^ ibit;
  // enumerate submasks of restMask, T = sub | ibit
  for(int sub=restMask; ; sub=(sub-1)&restMask){
     int T = sub | ibit;
     if(sumArr[T]==0){
        int r = mask ^ T;
        if(dp[r]>=0 && dp[r]+1>best) best=dp[r]+1;
     }
     if(sub==0) break;
  }
  dp[mask]=best;
}
