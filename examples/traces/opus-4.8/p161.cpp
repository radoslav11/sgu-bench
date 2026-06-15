void build(BS cur,int start){
  // record
  add(cur);
  for(int i=start;i<N;i++){
     // compatible: reach[i] & cur ==0 and reachInto[i] & cur==0
     if((reach[i]&cur).none() && (reachInto[i]&cur).none())
        build(cur | bit(i), i+1);
  }
}
