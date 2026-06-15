long long countLE(vector<double>&ang,double x){
  int n=ang.size();
  // doubled
  long long cnt=0;
  int j=0; // we use pointer
  // for each i, count k with ang[k]-ang[i] in (0,x], k>i (use doubled)
  // two pointer
  int hi=0;
  for(int i=0;i<n;i++){
     if(hi<i+1) hi=i+1;
     while(hi< i+n && (val(hi)-ang[i])<=x) hi++;
     cnt += hi - (i+1);
  }
  return cnt;
}
