
module zero ( 
    pi0, pi1,
    po0 );
  input  pi0, pi1;
  output po0;
  wire n7, n8, n9, n10, n11;
  andx g0(.a(pi1), .b(pi0), .O(n7));
  orx  g3(.a(n7), .b(pi0), .O(n8));
  invx g1(.a(n8), .O(n9));
  andx  g3(.a(n9), .b(pi0), .O(n11));
  andx  g3(.a(n11), .b(pi1), .O(po0));
endmodule


