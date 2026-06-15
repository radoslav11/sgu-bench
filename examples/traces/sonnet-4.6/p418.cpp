while(true){
  string ch=eatR(); // 'x'
  eat(); // then
  eat(); // begin
  eat(); // skip
  eat(); // ;
  PL inner=parseSegs();
  eat(); // end
  ...
