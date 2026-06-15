struct Arc {
  bool isSegment;
  // segment: P0, P1
  // arc: center, R, startAngle s, sweep w (CCW)
  double ax,ay,bx,by; // endpoints always
  double cx_,cy_; // center
  double R;
  double s, w;
};
