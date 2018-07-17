// Benchmark "top" written by ABC on Fri Feb  7 13:39:17 2014

module top ( 
    pi00, pi01, pi02, pi03, pi04, pi05, pi06, pi07, pi08, pi09, pi10, pi11,
    pi12, pi13, pi14, pi15, pi16, pi17, pi18, pi19, pi20, pi21, pi22, pi23,
    pi24, pi25, pi26, pi27, pi28, pi29, pi30, pi31, pi32,
    po00, po01, po02, po03, po04, po05, po06, po07, po08, po09, po10, po11,
    po12, po13, po14, po15, po16  );
  input  pi00, pi01, pi02, pi03, pi04, pi05, pi06, pi07, pi08, pi09,
    pi10, pi11, pi12, pi13, pi14, pi15, pi16, pi17, pi18, pi19, pi20, pi21,
    pi22, pi23, pi24, pi25, pi26, pi27, pi28, pi29, pi30, pi31, pi32;
  output po00, po01, po02, po03, po04, po05, po06, po07, po08, po09, po10,
    po11, po12, po13, po14, po15, po16;
  wire n50, n51, n52, n53, n54, n55, n56, n57, n58, n59, n60, n61, n62, n63,
    n64, n65, n66, n67, n68, n69, n70, n71, n72, n73, n74, n75, n76, n77,
    n78, n79, n80, n81, n82, n83, n84, n85, n86, n87, n88, n89, n90, n91,
    n92, n93, n94, n95, n96, n97, n98, n99, n100, n101, n102, n103, n104,
    n105, n106, n107, n108, n109, n110, n112, n114, n116, n118, n120, n122,
    n124, n126, n128, n130, n136, n137, n138, n139, n140, n141, n142, n144,
    n145, n146;
  xorx g00(.a(pi01), .b(pi00), .O(n50));
  andx g01(.a(pi32), .b(pi31), .O(n51));
  orx  g02(.a(pi32), .b(pi31), .O(n52));
  andx g03(.a(pi30), .b(pi29), .O(n53));
  orx  g04(.a(pi30), .b(pi29), .O(n54));
  andx g05(.a(pi28), .b(pi27), .O(n55));
  orx  g06(.a(pi28), .b(pi27), .O(n56));
  andx g07(.a(pi26), .b(pi25), .O(n57));
  orx  g08(.a(pi26), .b(pi25), .O(n58));
  andx g09(.a(pi24), .b(pi23), .O(n59));
  orx  g10(.a(pi24), .b(pi23), .O(n60));
  andx g11(.a(pi22), .b(pi21), .O(n61));
  orx  g12(.a(pi22), .b(pi21), .O(n62));
  andx g13(.a(pi20), .b(pi19), .O(n63));
  orx  g14(.a(pi20), .b(pi19), .O(n64));
  andx g15(.a(pi18), .b(pi17), .O(n65));
  orx  g16(.a(pi18), .b(pi17), .O(n66));
  andx g17(.a(pi16), .b(pi15), .O(n67));
  orx  g18(.a(pi16), .b(pi15), .O(n68));
  andx g19(.a(pi14), .b(pi13), .O(n69));
  orx  g20(.a(pi14), .b(pi13), .O(n70));
  andx g21(.a(pi12), .b(pi11), .O(n71));
  xorx g22(.a(pi12), .b(pi11), .O(n72));
  andx g23(.a(pi10), .b(pi09), .O(n73));
  andx g24(.a(pi08), .b(pi07), .O(n74));
  andx g25(.a(pi06), .b(pi05), .O(n75));
  orx  g26(.a(pi04), .b(pi02), .O(n76));
  andx g27(.a(pi04), .b(pi02), .O(n77));
  orx  g28(.a(n77), .b(pi03), .O(n78));
  andx g29(.a(n78), .b(n76), .O(n79));
  xorx g30(.a(pi06), .b(pi05), .O(n80));
  andx g31(.a(n80), .b(n79), .O(n81));
  orx  g32(.a(n81), .b(n75), .O(n82));
  xorx g33(.a(pi08), .b(pi07), .O(n83));
  andx g34(.a(n83), .b(n82), .O(n84));
  orx  g35(.a(n84), .b(n74), .O(n85));
  xorx g36(.a(pi10), .b(pi09), .O(n86));
  andx g37(.a(n86), .b(n85), .O(n87));
  orx  g38(.a(n87), .b(n73), .O(n88));
  andx g39(.a(n88), .b(n72), .O(n89));
  orx  g40(.a(n89), .b(n71), .O(n90));
  andx g41(.a(n90), .b(n70), .O(n91));
  orx  g42(.a(n91), .b(n69), .O(n92));
  andx g43(.a(n92), .b(n68), .O(n93));
  orx  g44(.a(n93), .b(n67), .O(n94));
  andx g45(.a(n94), .b(n66), .O(n95));
  orx  g46(.a(n95), .b(n65), .O(n96));
  andx g47(.a(n96), .b(n64), .O(n97));
  orx  g48(.a(n97), .b(n63), .O(n98));
  andx g49(.a(n98), .b(n62), .O(n99));
  orx  g50(.a(n99), .b(n61), .O(n100));
  andx g51(.a(n100), .b(n60), .O(n101));
  orx  g52(.a(n101), .b(n59), .O(n102));
  andx g53(.a(n102), .b(n58), .O(n103));
  orx  g54(.a(n103), .b(n57), .O(n104));
  andx g55(.a(n104), .b(n56), .O(n105));
  orx  g56(.a(n105), .b(n55), .O(n106));
  andx g57(.a(n106), .b(n54), .O(n107));
  orx  g58(.a(n107), .b(n53), .O(n108));
  andx g59(.a(n108), .b(n52), .O(n109));
  orx  g60(.a(n109), .b(n51), .O(n110));
  xorx g61(.a(n110), .b(n50), .O(po00));
  xorx g62(.a(pi32), .b(pi31), .O(n112));
  xorx g63(.a(n112), .b(n108), .O(po01));
  xorx g64(.a(pi30), .b(pi29), .O(n114));
  xorx g65(.a(n114), .b(n106), .O(po02));
  xorx g66(.a(pi28), .b(pi27), .O(n116));
  xorx g67(.a(n116), .b(n104), .O(po03));
  xorx g68(.a(pi26), .b(pi25), .O(n118));
  xorx g69(.a(n118), .b(n102), .O(po04));
  xorx g70(.a(pi24), .b(pi23), .O(n120));
  xorx g71(.a(n120), .b(n100), .O(po05));
  xorx g72(.a(pi22), .b(pi21), .O(n122));
  xorx g73(.a(n122), .b(n98), .O(po06));
  xorx g74(.a(pi20), .b(pi19), .O(n124));
  xorx g75(.a(n124), .b(n96), .O(po07));
  xorx g76(.a(pi18), .b(pi17), .O(n126));
  xorx g77(.a(n126), .b(n94), .O(po08));
  xorx g78(.a(pi16), .b(pi15), .O(n128));
  xorx g79(.a(n128), .b(n92), .O(po09));
  xorx g80(.a(pi14), .b(pi13), .O(n130));
  xorx g81(.a(n130), .b(n90), .O(po10));
  xorx g82(.a(n88), .b(n72), .O(po11));
  xorx g83(.a(n86), .b(n85), .O(po12));
  xorx g84(.a(n83), .b(n82), .O(po13));
  xorx g85(.a(n80), .b(n79), .O(po14));
  invx g86(.a(n76), .O(n136));
  orx  g87(.a(n77), .b(n136), .O(n137));
  andx g88(.a(n137), .b(pi03), .O(n138));
  invx g89(.a(n79), .O(n139));
  invx g90(.a(pi03), .O(n140));
  andx g91(.a(n76), .b(n140), .O(n141));
  andx g92(.a(n141), .b(n139), .O(n142));
  orx  g93(.a(n142), .b(n138), .O(po15));
  andx g94(.a(pi01), .b(pi00), .O(n144));
  orx  g95(.a(pi01), .b(pi00), .O(n145));
  andx g96(.a(n110), .b(n145), .O(n146));
  orx  g97(.a(n146), .b(n144), .O(po16));
endmodule


