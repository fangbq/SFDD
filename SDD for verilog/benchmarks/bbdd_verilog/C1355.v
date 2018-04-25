// Benchmark "C1355.iscas" written by ABC on Fri Feb  7 13:23:55 2014

module C1355.iscas ( 
    pi00, pi01, pi02, pi03, pi04, pi05, pi06, pi07, pi08, pi09, pi10, pi11,
    pi12, pi13, pi14, pi15, pi16, pi17, pi18, pi19, pi20, pi21, pi22, pi23,
    pi24, pi25, pi26, pi27, pi28, pi29, pi30, pi31, pi32, pi33, pi34, pi35,
    pi36, pi37, pi38, pi39, pi40,
    po00, po01, po02, po03, po04, po05, po06, po07, po08, po09, po10, po11,
    po12, po13, po14, po15, po16, po17, po18, po19, po20, po21, po22, po23,
    po24, po25, po26, po27, po28, po29, po30, po31  );
  input  pi00, pi01, pi02, pi03, pi04, pi05, pi06, pi07, pi08, pi09,
    pi10, pi11, pi12, pi13, pi14, pi15, pi16, pi17, pi18, pi19, pi20, pi21,
    pi22, pi23, pi24, pi25, pi26, pi27, pi28, pi29, pi30, pi31, pi32, pi33,
    pi34, pi35, pi36, pi37, pi38, pi39, pi40;
  output po00, po01, po02, po03, po04, po05, po06, po07, po08, po09, po10,
    po11, po12, po13, po14, po15, po16, po17, po18, po19, po20, po21, po22,
    po23, po24, po25, po26, po27, po28, po29, po30, po31;
  wire n73, n74, n75, n76, n77, n78, n79, n80, n81, n82, n83, n84, n85, n86,
    n87, n88, n89, n90, n91, n92, n93, n94, n95, n96, n97, n98, n99, n100,
    n101, n102, n103, n104, n105, n106, n107, n108, n109, n110, n111, n112,
    n113, n114, n115, n116, n117, n118, n119, n120, n121, n122, n123, n124,
    n125, n126, n127, n128, n129, n130, n131, n132, n133, n134, n135, n136,
    n137, n138, n139, n140, n141, n142, n143, n144, n145, n146, n147, n148,
    n149, n150, n151, n152, n153, n154, n155, n156, n157, n158, n159, n160,
    n161, n162, n163, n164, n165, n166, n167, n168, n169, n170, n171, n172,
    n173, n174, n175, n176, n177, n178, n179, n180, n181, n182, n183, n184,
    n185, n187, n189, n191, n193, n194, n195, n196, n197, n198, n199, n200,
    n201, n203, n205, n207, n209, n210, n211, n212, n213, n214, n215, n216,
    n217, n219, n221, n223, n225, n226, n227, n228, n230, n232, n234, n236,
    n237, n238, n239, n240, n241, n242, n243, n244, n245, n246, n247, n249,
    n251, n253, n255, n256, n257, n259, n261, n263, n265, n266, n267, n269,
    n271, n273, n275, n276, n277, n279, n281, n283;
  xorx g000(.a(pi08), .b(pi05), .O(n73));
  xorx g001(.a(pi26), .b(pi25), .O(n74));
  xorx g002(.a(n74), .b(n73), .O(n75));
  andx g003(.a(pi01), .b(pi03), .O(n76));
  xorx g004(.a(pi22), .b(pi20), .O(n77));
  xorx g005(.a(pi38), .b(pi36), .O(n78));
  xorx g006(.a(n78), .b(n77), .O(n79));
  xorx g007(.a(pi24), .b(pi17), .O(n80));
  xorx g008(.a(pi39), .b(pi34), .O(n81));
  xorx g009(.a(n81), .b(n80), .O(n82));
  xorx g010(.a(n82), .b(n79), .O(n83));
  xorx g011(.a(n83), .b(n76), .O(n84));
  xorx g012(.a(n84), .b(n75), .O(n85));
  xorx g013(.a(pi16), .b(pi15), .O(n86));
  xorx g014(.a(pi30), .b(pi29), .O(n87));
  xorx g015(.a(n87), .b(n86), .O(n88));
  andx g016(.a(pi01), .b(pi10), .O(n89));
  invx g017(.a(n89), .O(n90));
  xorx g018(.a(pi23), .b(pi18), .O(n91));
  xorx g019(.a(pi40), .b(pi33), .O(n92));
  xorx g020(.a(n92), .b(n91), .O(n93));
  xorx g021(.a(n93), .b(n82), .O(n94));
  xorx g022(.a(n94), .b(n90), .O(n95));
  xorx g023(.a(n95), .b(n88), .O(n96));
  xorx g024(.a(pi13), .b(pi14), .O(n97));
  xorx g025(.a(pi32), .b(pi31), .O(n98));
  xorx g026(.a(n98), .b(n97), .O(n99));
  andx g027(.a(pi01), .b(pi11), .O(n100));
  invx g028(.a(n100), .O(n101));
  xorx g029(.a(pi21), .b(pi19), .O(n102));
  xorx g030(.a(pi37), .b(pi35), .O(n103));
  xorx g031(.a(n103), .b(n102), .O(n104));
  xorx g032(.a(n104), .b(n79), .O(n105));
  xorx g033(.a(n105), .b(n101), .O(n106));
  xorx g034(.a(n106), .b(n99), .O(n107));
  xorx g035(.a(pi09), .b(pi06), .O(n108));
  xorx g036(.a(pi28), .b(pi27), .O(n109));
  xorx g037(.a(n109), .b(n108), .O(n110));
  andx g038(.a(pi01), .b(pi04), .O(n111));
  invx g039(.a(n111), .O(n112));
  xorx g040(.a(n104), .b(n93), .O(n113));
  xorx g041(.a(n113), .b(n112), .O(n114));
  xorx g042(.a(n114), .b(n110), .O(n115));
  andx g043(.a(n115), .b(n85), .O(n116));
  andx g044(.a(n116), .b(n107), .O(n117));
  andx g045(.a(n117), .b(n96), .O(n118));
  invx g046(.a(n76), .O(n119));
  xorx g047(.a(n83), .b(n119), .O(n120));
  xorx g048(.a(n120), .b(n75), .O(n121));
  xorx g049(.a(n113), .b(n111), .O(n122));
  xorx g050(.a(n122), .b(n110), .O(n123));
  andx g051(.a(n123), .b(n121), .O(n124));
  andx g052(.a(n124), .b(n107), .O(n125));
  andx g053(.a(n125), .b(n96), .O(n126));
  andx g054(.a(n115), .b(n121), .O(n127));
  xorx g055(.a(n94), .b(n89), .O(n128));
  xorx g056(.a(n128), .b(n88), .O(n129));
  andx g057(.a(n107), .b(n129), .O(n130));
  andx g058(.a(n130), .b(n127), .O(n131));
  xorx g059(.a(n105), .b(n100), .O(n132));
  xorx g060(.a(n132), .b(n99), .O(n133));
  andx g061(.a(n133), .b(n96), .O(n134));
  andx g062(.a(n134), .b(n127), .O(n135));
  orx  g063(.a(n135), .b(n131), .O(n136));
  orx  g064(.a(n136), .b(n126), .O(n137));
  orx  g065(.a(n137), .b(n118), .O(n138));
  xorx g066(.a(pi17), .b(pi20), .O(n139));
  xorx g067(.a(pi18), .b(pi19), .O(n140));
  xorx g068(.a(n140), .b(n139), .O(n141));
  andx g069(.a(pi01), .b(pi00), .O(n142));
  xorx g070(.a(pi06), .b(pi05), .O(n143));
  xorx g071(.a(pi15), .b(pi14), .O(n144));
  xorx g072(.a(n144), .b(n143), .O(n145));
  xorx g073(.a(pi09), .b(pi08), .O(n146));
  xorx g074(.a(pi16), .b(pi13), .O(n147));
  xorx g075(.a(n147), .b(n146), .O(n148));
  xorx g076(.a(n148), .b(n145), .O(n149));
  xorx g077(.a(n149), .b(n142), .O(n150));
  xorx g078(.a(n150), .b(n141), .O(n151));
  xorx g079(.a(pi24), .b(pi22), .O(n152));
  xorx g080(.a(pi23), .b(pi21), .O(n153));
  xorx g081(.a(n153), .b(n152), .O(n154));
  andx g082(.a(pi01), .b(pi12), .O(n155));
  invx g083(.a(n155), .O(n156));
  xorx g084(.a(pi27), .b(pi25), .O(n157));
  xorx g085(.a(pi29), .b(pi31), .O(n158));
  xorx g086(.a(n158), .b(n157), .O(n159));
  xorx g087(.a(pi28), .b(pi26), .O(n160));
  xorx g088(.a(pi30), .b(pi32), .O(n161));
  xorx g089(.a(n161), .b(n160), .O(n162));
  xorx g090(.a(n162), .b(n159), .O(n163));
  xorx g091(.a(n163), .b(n156), .O(n164));
  xorx g092(.a(n164), .b(n154), .O(n165));
  andx g093(.a(n165), .b(n151), .O(n166));
  xorx g094(.a(pi39), .b(pi38), .O(n167));
  xorx g095(.a(pi40), .b(pi37), .O(n168));
  xorx g096(.a(n168), .b(n167), .O(n169));
  andx g097(.a(pi01), .b(pi07), .O(n170));
  invx g098(.a(n170), .O(n171));
  xorx g099(.a(n162), .b(n148), .O(n172));
  xorx g100(.a(n172), .b(n171), .O(n173));
  xorx g101(.a(n173), .b(n169), .O(n174));
  xorx g102(.a(pi34), .b(pi36), .O(n175));
  xorx g103(.a(pi33), .b(pi35), .O(n176));
  xorx g104(.a(n176), .b(n175), .O(n177));
  andx g105(.a(pi01), .b(pi02), .O(n178));
  xorx g106(.a(n159), .b(n145), .O(n179));
  xorx g107(.a(n179), .b(n178), .O(n180));
  xorx g108(.a(n180), .b(n177), .O(n181));
  andx g109(.a(n181), .b(n174), .O(n182));
  andx g110(.a(n182), .b(n166), .O(n183));
  andx g111(.a(n183), .b(n138), .O(n184));
  andx g112(.a(n184), .b(n85), .O(n185));
  xorx g113(.a(n185), .b(pi05), .O(po00));
  andx g114(.a(n184), .b(n123), .O(n187));
  xorx g115(.a(n187), .b(pi06), .O(po01));
  andx g116(.a(n184), .b(n133), .O(n189));
  xorx g117(.a(n189), .b(pi14), .O(po02));
  andx g118(.a(n184), .b(n129), .O(n191));
  xorx g119(.a(n191), .b(pi15), .O(po03));
  xorx g120(.a(n172), .b(n170), .O(n193));
  xorx g121(.a(n193), .b(n169), .O(n194));
  invx g122(.a(n178), .O(n195));
  xorx g123(.a(n179), .b(n195), .O(n196));
  xorx g124(.a(n196), .b(n177), .O(n197));
  andx g125(.a(n197), .b(n166), .O(n198));
  andx g126(.a(n198), .b(n194), .O(n199));
  andx g127(.a(n199), .b(n138), .O(n200));
  andx g128(.a(n200), .b(n85), .O(n201));
  xorx g129(.a(n201), .b(pi08), .O(po04));
  andx g130(.a(n200), .b(n123), .O(n203));
  xorx g131(.a(n203), .b(pi09), .O(po05));
  andx g132(.a(n200), .b(n133), .O(n205));
  xorx g133(.a(n205), .b(pi13), .O(po06));
  andx g134(.a(n200), .b(n129), .O(n207));
  xorx g135(.a(n207), .b(pi16), .O(po07));
  invx g136(.a(n142), .O(n209));
  xorx g137(.a(n149), .b(n209), .O(n210));
  xorx g138(.a(n210), .b(n141), .O(n211));
  xorx g139(.a(n163), .b(n155), .O(n212));
  xorx g140(.a(n212), .b(n154), .O(n213));
  andx g141(.a(n213), .b(n211), .O(n214));
  andx g142(.a(n214), .b(n182), .O(n215));
  andx g143(.a(n215), .b(n138), .O(n216));
  andx g144(.a(n216), .b(n85), .O(n217));
  xorx g145(.a(n217), .b(pi25), .O(po08));
  andx g146(.a(n216), .b(n123), .O(n219));
  xorx g147(.a(n219), .b(pi27), .O(po09));
  andx g148(.a(n216), .b(n133), .O(n221));
  xorx g149(.a(n221), .b(pi31), .O(po10));
  andx g150(.a(n216), .b(n129), .O(n223));
  xorx g151(.a(n223), .b(pi29), .O(po11));
  andx g152(.a(n214), .b(n197), .O(n225));
  andx g153(.a(n225), .b(n194), .O(n226));
  andx g154(.a(n226), .b(n138), .O(n227));
  andx g155(.a(n227), .b(n85), .O(n228));
  xorx g156(.a(n228), .b(pi26), .O(po12));
  andx g157(.a(n227), .b(n123), .O(n230));
  xorx g158(.a(n230), .b(pi28), .O(po13));
  andx g159(.a(n227), .b(n133), .O(n232));
  xorx g160(.a(n232), .b(pi32), .O(po14));
  andx g161(.a(n227), .b(n129), .O(n234));
  xorx g162(.a(n234), .b(pi30), .O(po15));
  andx g163(.a(n198), .b(n174), .O(n236));
  andx g164(.a(n225), .b(n174), .O(n237));
  andx g165(.a(n165), .b(n211), .O(n238));
  andx g166(.a(n197), .b(n194), .O(n239));
  andx g167(.a(n239), .b(n238), .O(n240));
  andx g168(.a(n238), .b(n182), .O(n241));
  orx  g169(.a(n241), .b(n240), .O(n242));
  orx  g170(.a(n242), .b(n237), .O(n243));
  orx  g171(.a(n243), .b(n236), .O(n244));
  andx g172(.a(n134), .b(n116), .O(n245));
  andx g173(.a(n245), .b(n244), .O(n246));
  andx g174(.a(n246), .b(n151), .O(n247));
  xorx g175(.a(n247), .b(pi20), .O(po16));
  andx g176(.a(n246), .b(n213), .O(n249));
  xorx g177(.a(n249), .b(pi22), .O(po17));
  andx g178(.a(n246), .b(n181), .O(n251));
  xorx g179(.a(n251), .b(pi36), .O(po18));
  andx g180(.a(n246), .b(n194), .O(n253));
  xorx g181(.a(n253), .b(pi38), .O(po19));
  andx g182(.a(n117), .b(n129), .O(n255));
  andx g183(.a(n255), .b(n244), .O(n256));
  andx g184(.a(n256), .b(n151), .O(n257));
  xorx g185(.a(n257), .b(pi17), .O(po20));
  andx g186(.a(n256), .b(n213), .O(n259));
  xorx g187(.a(n259), .b(pi24), .O(po21));
  andx g188(.a(n256), .b(n181), .O(n261));
  xorx g189(.a(n261), .b(pi34), .O(po22));
  andx g190(.a(n256), .b(n194), .O(n263));
  xorx g191(.a(n263), .b(pi39), .O(po23));
  andx g192(.a(n134), .b(n124), .O(n265));
  andx g193(.a(n265), .b(n244), .O(n266));
  andx g194(.a(n266), .b(n151), .O(n267));
  xorx g195(.a(n267), .b(pi19), .O(po24));
  andx g196(.a(n266), .b(n213), .O(n269));
  xorx g197(.a(n269), .b(pi21), .O(po25));
  andx g198(.a(n266), .b(n181), .O(n271));
  xorx g199(.a(n271), .b(pi35), .O(po26));
  andx g200(.a(n266), .b(n194), .O(n273));
  xorx g201(.a(n273), .b(pi37), .O(po27));
  andx g202(.a(n125), .b(n129), .O(n275));
  andx g203(.a(n275), .b(n244), .O(n276));
  andx g204(.a(n276), .b(n151), .O(n277));
  xorx g205(.a(n277), .b(pi18), .O(po28));
  andx g206(.a(n276), .b(n213), .O(n279));
  xorx g207(.a(n279), .b(pi23), .O(po29));
  andx g208(.a(n276), .b(n181), .O(n281));
  xorx g209(.a(n281), .b(pi33), .O(po30));
  andx g210(.a(n276), .b(n194), .O(n283));
  xorx g211(.a(n283), .b(pi40), .O(po31));
endmodule


