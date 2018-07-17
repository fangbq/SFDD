// Benchmark "C17.iscas" written by ABC on Fri Feb  7 13:35:25 2014

module C17.iscas ( 
    pi0, pi1, pi2, pi3, pi4,
    po0, po1  );
  input  pi0, pi1, pi2, pi3, pi4;
  output po0, po1;
  wire n7, n8, n9, n10, n11, n13;
  andx g0(.a(pi2), .b(pi0), .O(n7));
  invx g1(.a(pi2), .O(n8));
  invx g2(.a(pi3), .O(n9));
  orx  g3(.a(n9), .b(n8), .O(n10));
  andx g4(.a(n10), .b(pi1), .O(n11));
  orx  g5(.a(n11), .b(n7), .O(po0));
  andx g6(.a(n10), .b(pi4), .O(n13));
  orx  g7(.a(n13), .b(n11), .O(po1));
endmodule


