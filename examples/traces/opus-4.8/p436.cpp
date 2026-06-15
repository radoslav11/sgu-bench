string toDiputs(long long val){
  if(val==0) return "O";
  string s;
  for(int p=8;p>=0;p--){
    long long cnt=val/w[p];
    val%=w[p];
    for(int k=0;k<cnt;k++) s+=chars[p];
  }
  return s;
}
string toDecimal(long long val){ return to_string(val); }
