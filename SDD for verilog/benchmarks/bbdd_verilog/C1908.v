// Benchmark "C1908.iscas" written by ABC on Fri Feb  7 13:29:59 2014

module C1908.iscas ( 
    pi00, pi01, pi02, pi03, pi04, pi05, pi06, pi07, pi08, pi09, pi10, pi11,
    pi12, pi13, pi14, pi15, pi16, pi17, pi18, pi19, pi20, pi21, pi22, pi23,
    pi24, pi25, pi26, pi27, pi28, pi29, pi30, pi31, pi32,
    po00, po01, po02, po03, po04, po05, po06, po07, po08, po09, po10, po11,
    po12, po13, po14, po15, po16, po17, po18, po19, po20, po21, po22, po23,
    po24  );
  input  pi00, pi01, pi02, pi03, pi04, pi05, pi06, pi07, pi08, pi09,
    pi10, pi11, pi12, pi13, pi14, pi15, pi16, pi17, pi18, pi19, pi20, pi21,
    pi22, pi23, pi24, pi25, pi26, pi27, pi28, pi29, pi30, pi31, pi32;
  output po00, po01, po02, po03, po04, po05, po06, po07, po08, po09, po10,
    po11, po12, po13, po14, po15, po16, po17, po18, po19, po20, po21, po22,
    po23, po24;
  wire n58, n59, n60, n61, n62, n63, n64, n65, n66, n67, n68, n69, n70, n71,
    n72, n73, n74, n75, n76, n77, n78, n79, n80, n81, n82, n83, n84, n85,
    n86, n87, n88, n89, n90, n91, n92, n93, n94, n95, n96, n97, n98, n99,
    n100, n101, n102, n103, n104, n105, n106, n107, n108, n109, n110, n111,
    n112, n113, n114, n115, n116, n117, n118, n119, n120, n121, n122, n123,
    n124, n125, n126, n127, n128, n129, n130, n131, n132, n133, n134, n135,
    n136, n137, n138, n139, n140, n141, n142, n143, n144, n145, n146, n147,
    n148, n149, n150, n151, n152, n153, n154, n155, n156, n157, n158, n159,
    n160, n161, n162, n163, n164, n166, n167, n168, n169, n170, n171, n172,
    n174, n175, n176, n178, n179, n180, n182, n183, n184, n185, n186, n187,
    n188, n189, n190, n192, n193, n194, n196, n197, n199, n200, n201, n202,
    n203, n205, n207, n208, n210, n211, n212, n214, n215, n217, n218, n219,
    n220, n221, n223, n225, n226, n227, n229, n231, n232, n233, n234, n235,
    n236, n237, n238, n239, n240, n241, n242, n243, n244, n245, n246, n247,
    n248, n249, n250, n251, n252, n253, n254, n255, n256, n257, n258, n259,
    n260, n261, n262, n263, n264, n265, n266, n267, n268, n269, n270, n271,
    n272, n273, n274, n275, n276, n277, n278, n279, n280, n281, n282, n283,
    n284, n285, n286, n287, n288, n289, n290, n292, n293, n294, n295, n296,
    n298, n299, n300, n302, n303, n304, n306, n307, n308, n310, n311, n312,
    n314, n315, n316, n317, n318, n319, n321, n322, n323, n324, n325, n326,
    n327, n328, n329, n330, n332, n333, n334, n335;
  invx g000(.a(pi19), .O(n58));
  xorx g001(.a(pi26), .b(pi23), .O(n59));
  invx g002(.a(n59), .O(n60));
  xorx g003(.a(n60), .b(pi25), .O(n61));
  xorx g004(.a(pi12), .b(pi30), .O(n62));
  xorx g005(.a(n62), .b(pi28), .O(n63));
  xorx g006(.a(n63), .b(n61), .O(n64));
  invx g007(.a(pi22), .O(n65));
  invx g008(.a(pi20), .O(n66));
  andx g009(.a(pi15), .b(pi16), .O(n67));
  andx g010(.a(n67), .b(n66), .O(n68));
  xorx g011(.a(n68), .b(n65), .O(n69));
  xorx g012(.a(n69), .b(n64), .O(n70));
  andx g013(.a(n70), .b(n58), .O(n71));
  andx g014(.a(n58), .b(pi15), .O(n72));
  invx g015(.a(n72), .O(n73));
  andx g016(.a(n73), .b(pi17), .O(n74));
  xorx g017(.a(n74), .b(n71), .O(n75));
  invx g018(.a(n75), .O(n76));
  invx g019(.a(pi13), .O(n77));
  andx g020(.a(n77), .b(pi06), .O(n78));
  andx g021(.a(n78), .b(n66), .O(n79));
  xorx g022(.a(n79), .b(pi04), .O(n80));
  xorx g023(.a(pi30), .b(pi21), .O(n81));
  xorx g024(.a(n81), .b(pi08), .O(n82));
  xorx g025(.a(pi25), .b(pi29), .O(n83));
  xorx g026(.a(n83), .b(pi12), .O(n84));
  xorx g027(.a(pi22), .b(pi27), .O(n85));
  xorx g028(.a(n85), .b(pi11), .O(n86));
  xorx g029(.a(n86), .b(n84), .O(n87));
  xorx g030(.a(n87), .b(n82), .O(n88));
  xorx g031(.a(n88), .b(n80), .O(n89));
  andx g032(.a(n89), .b(n58), .O(n90));
  xorx g033(.a(n90), .b(pi05), .O(n91));
  andx g034(.a(n91), .b(n76), .O(n92));
  andx g035(.a(n58), .b(n77), .O(n93));
  invx g036(.a(n93), .O(n94));
  andx g037(.a(n94), .b(pi07), .O(n95));
  invx g038(.a(n95), .O(n96));
  invx g039(.a(pi23), .O(n97));
  xorx g040(.a(n84), .b(n97), .O(n98));
  andx g041(.a(n66), .b(pi03), .O(n99));
  invx g042(.a(n99), .O(n100));
  xorx g043(.a(n100), .b(n98), .O(n101));
  invx g044(.a(n101), .O(n102));
  xorx g045(.a(pi24), .b(pi09), .O(n103));
  xorx g046(.a(n103), .b(pi04), .O(n104));
  xorx g047(.a(n104), .b(n82), .O(n105));
  xorx g048(.a(pi31), .b(pi28), .O(n106));
  xorx g049(.a(n106), .b(n105), .O(n107));
  invx g050(.a(n107), .O(n108));
  andx g051(.a(n108), .b(n102), .O(n109));
  andx g052(.a(n107), .b(n101), .O(n110));
  orx  g053(.a(n110), .b(pi19), .O(n111));
  orx  g054(.a(n111), .b(n109), .O(n112));
  andx g055(.a(n94), .b(pi06), .O(n113));
  invx g056(.a(n113), .O(n114));
  xorx g057(.a(n114), .b(n112), .O(n115));
  andx g058(.a(n115), .b(n96), .O(n116));
  andx g059(.a(n73), .b(pi16), .O(n117));
  invx g060(.a(n117), .O(n118));
  xorx g061(.a(pi26), .b(pi28), .O(n119));
  andx g062(.a(n66), .b(pi01), .O(n120));
  xorx g063(.a(n120), .b(n119), .O(n121));
  xorx g064(.a(n104), .b(n84), .O(n122));
  xorx g065(.a(n122), .b(n86), .O(n123));
  xorx g066(.a(n123), .b(n121), .O(n124));
  andx g067(.a(n124), .b(n58), .O(n125));
  xorx g068(.a(n125), .b(pi02), .O(n126));
  andx g069(.a(n126), .b(n118), .O(n127));
  andx g070(.a(n127), .b(n116), .O(n128));
  andx g071(.a(n128), .b(n92), .O(n129));
  invx g072(.a(pi15), .O(n130));
  orx  g073(.a(n77), .b(n130), .O(n131));
  invx g074(.a(pi00), .O(n132));
  andx g075(.a(pi19), .b(n132), .O(n133));
  andx g076(.a(n133), .b(pi20), .O(n134));
  andx g077(.a(n134), .b(n131), .O(n135));
  andx g078(.a(n66), .b(pi14), .O(n136));
  andx g079(.a(n136), .b(n131), .O(n137));
  orx  g080(.a(n137), .b(n135), .O(n138));
  xorx g081(.a(pi31), .b(pi21), .O(n139));
  xorx g082(.a(n139), .b(pi24), .O(n140));
  xorx g083(.a(pi29), .b(pi12), .O(n141));
  xorx g084(.a(n141), .b(pi27), .O(n142));
  xorx g085(.a(n142), .b(n140), .O(n143));
  andx g086(.a(pi15), .b(pi17), .O(n144));
  andx g087(.a(n144), .b(n66), .O(n145));
  xorx g088(.a(n145), .b(n143), .O(n146));
  andx g089(.a(n146), .b(n58), .O(n147));
  xorx g090(.a(n147), .b(pi18), .O(n148));
  invx g091(.a(n148), .O(n149));
  invx g092(.a(pi10), .O(n150));
  invx g093(.a(pi11), .O(n151));
  andx g094(.a(n77), .b(pi07), .O(n152));
  andx g095(.a(n152), .b(n66), .O(n153));
  xorx g096(.a(n153), .b(pi29), .O(n154));
  xorx g097(.a(n154), .b(n151), .O(n155));
  xorx g098(.a(n155), .b(n61), .O(n156));
  xorx g099(.a(pi31), .b(pi08), .O(n157));
  xorx g100(.a(n157), .b(pi09), .O(n158));
  xorx g101(.a(n158), .b(n156), .O(n159));
  andx g102(.a(n159), .b(n58), .O(n160));
  xorx g103(.a(n160), .b(n150), .O(n161));
  andx g104(.a(n161), .b(n149), .O(n162));
  andx g105(.a(n162), .b(n138), .O(n163));
  andx g106(.a(n163), .b(n129), .O(n164));
  xorx g107(.a(n164), .b(pi04), .O(po00));
  invx g108(.a(n91), .O(n166));
  andx g109(.a(n166), .b(n76), .O(n167));
  andx g110(.a(n167), .b(n128), .O(n168));
  xorx g111(.a(n160), .b(pi10), .O(n169));
  andx g112(.a(n169), .b(n149), .O(n170));
  andx g113(.a(n170), .b(n138), .O(n171));
  andx g114(.a(n171), .b(n168), .O(n172));
  xorx g115(.a(n172), .b(pi09), .O(po01));
  andx g116(.a(n161), .b(n148), .O(n174));
  andx g117(.a(n174), .b(n138), .O(n175));
  andx g118(.a(n175), .b(n168), .O(n176));
  xorx g119(.a(n176), .b(pi24), .O(po02));
  andx g120(.a(n166), .b(n75), .O(n178));
  andx g121(.a(n178), .b(n163), .O(n179));
  andx g122(.a(n179), .b(n128), .O(n180));
  xorx g123(.a(n180), .b(pi28), .O(po03));
  andx g124(.a(n91), .b(n75), .O(n182));
  andx g125(.a(n182), .b(n128), .O(n183));
  invx g126(.a(pi32), .O(n184));
  andx g127(.a(pi19), .b(n184), .O(n185));
  andx g128(.a(n185), .b(pi20), .O(n186));
  andx g129(.a(n186), .b(n131), .O(n187));
  orx  g130(.a(n187), .b(n137), .O(n188));
  andx g131(.a(n188), .b(n174), .O(n189));
  andx g132(.a(n189), .b(n183), .O(n190));
  xorx g133(.a(n190), .b(pi12), .O(po04));
  andx g134(.a(n169), .b(n148), .O(n192));
  andx g135(.a(n192), .b(n188), .O(n193));
  andx g136(.a(n193), .b(n129), .O(n194));
  xorx g137(.a(n194), .b(pi29), .O(po05));
  andx g138(.a(n188), .b(n170), .O(n196));
  andx g139(.a(n196), .b(n183), .O(n197));
  xorx g140(.a(n197), .b(pi25), .O(po06));
  invx g141(.a(n126), .O(n199));
  andx g142(.a(n199), .b(n118), .O(n200));
  andx g143(.a(n200), .b(n116), .O(n201));
  andx g144(.a(n201), .b(n92), .O(n202));
  andx g145(.a(n202), .b(n171), .O(n203));
  xorx g146(.a(n203), .b(pi08), .O(po07));
  andx g147(.a(n202), .b(n175), .O(n205));
  xorx g148(.a(n205), .b(pi21), .O(po08));
  andx g149(.a(n182), .b(n163), .O(n207));
  andx g150(.a(n207), .b(n201), .O(n208));
  xorx g151(.a(n208), .b(pi30), .O(po09));
  andx g152(.a(n201), .b(n167), .O(n210));
  andx g153(.a(n192), .b(n138), .O(n211));
  andx g154(.a(n211), .b(n210), .O(n212));
  xorx g155(.a(n212), .b(pi31), .O(po10));
  andx g156(.a(n196), .b(n178), .O(n214));
  andx g157(.a(n214), .b(n201), .O(n215));
  xorx g158(.a(n215), .b(pi23), .O(po11));
  xorx g159(.a(n113), .b(n112), .O(n217));
  andx g160(.a(n217), .b(n96), .O(n218));
  andx g161(.a(n218), .b(n127), .O(n219));
  andx g162(.a(n219), .b(n92), .O(n220));
  andx g163(.a(n220), .b(n196), .O(n221));
  xorx g164(.a(n221), .b(pi11), .O(po12));
  andx g165(.a(n220), .b(n189), .O(n223));
  xorx g166(.a(n223), .b(pi27), .O(po13));
  andx g167(.a(n188), .b(n182), .O(n225));
  andx g168(.a(n225), .b(n162), .O(n226));
  andx g169(.a(n226), .b(n219), .O(n227));
  xorx g170(.a(n227), .b(pi22), .O(po14));
  andx g171(.a(n219), .b(n214), .O(n229));
  xorx g172(.a(n229), .b(pi26), .O(po15));
  andx g173(.a(n118), .b(n96), .O(n231));
  andx g174(.a(n231), .b(n76), .O(n232));
  andx g175(.a(n232), .b(n149), .O(n233));
  andx g176(.a(n233), .b(n161), .O(n234));
  andx g177(.a(n199), .b(n217), .O(n235));
  andx g178(.a(n235), .b(n166), .O(n236));
  andx g179(.a(n236), .b(n234), .O(n237));
  orx  g180(.a(n237), .b(pi20), .O(n238));
  orx  g181(.a(n238), .b(pi14), .O(n239));
  orx  g182(.a(n205), .b(n203), .O(n240));
  orx  g183(.a(n208), .b(n172), .O(n241));
  orx  g184(.a(n241), .b(n240), .O(n242));
  orx  g185(.a(n212), .b(n164), .O(n243));
  orx  g186(.a(n180), .b(n176), .O(n244));
  orx  g187(.a(n244), .b(n243), .O(n245));
  orx  g188(.a(n245), .b(n242), .O(n246));
  orx  g189(.a(n229), .b(n227), .O(n247));
  orx  g190(.a(n215), .b(n194), .O(n248));
  orx  g191(.a(n248), .b(n247), .O(n249));
  orx  g192(.a(n197), .b(n190), .O(n250));
  orx  g193(.a(n223), .b(n221), .O(n251));
  orx  g194(.a(n251), .b(n250), .O(n252));
  orx  g195(.a(n252), .b(n249), .O(n253));
  orx  g196(.a(n253), .b(n246), .O(n254));
  andx g197(.a(n218), .b(n200), .O(n255));
  andx g198(.a(n178), .b(n137), .O(n256));
  andx g199(.a(n256), .b(n162), .O(n257));
  andx g200(.a(n257), .b(n255), .O(n258));
  andx g201(.a(n255), .b(n167), .O(n259));
  andx g202(.a(n174), .b(n137), .O(n260));
  andx g203(.a(n260), .b(n259), .O(n261));
  andx g204(.a(n170), .b(n137), .O(n262));
  andx g205(.a(n262), .b(n259), .O(n263));
  orx  g206(.a(n263), .b(n261), .O(n264));
  orx  g207(.a(n264), .b(n258), .O(n265));
  andx g208(.a(n167), .b(n162), .O(n266));
  andx g209(.a(n137), .b(n117), .O(n267));
  andx g210(.a(n267), .b(n199), .O(n268));
  andx g211(.a(n268), .b(n218), .O(n269));
  andx g212(.a(n269), .b(n266), .O(n270));
  invx g213(.a(pi14), .O(n271));
  andx g214(.a(n137), .b(n95), .O(n272));
  andx g215(.a(n272), .b(n217), .O(n273));
  andx g216(.a(n273), .b(n200), .O(n274));
  andx g217(.a(n274), .b(n266), .O(n275));
  orx  g218(.a(n275), .b(n271), .O(n276));
  orx  g219(.a(n276), .b(n270), .O(n277));
  andx g220(.a(n162), .b(n137), .O(n278));
  andx g221(.a(n278), .b(n167), .O(n279));
  andx g222(.a(n279), .b(n219), .O(n280));
  orx  g223(.a(n280), .b(n238), .O(n281));
  andx g224(.a(n137), .b(n92), .O(n282));
  andx g225(.a(n282), .b(n162), .O(n283));
  andx g226(.a(n283), .b(n255), .O(n284));
  andx g227(.a(n278), .b(n210), .O(n285));
  orx  g228(.a(n285), .b(n284), .O(n286));
  orx  g229(.a(n286), .b(n281), .O(n287));
  orx  g230(.a(n287), .b(n277), .O(n288));
  orx  g231(.a(n288), .b(n265), .O(n289));
  orx  g232(.a(n289), .b(n254), .O(n290));
  andx g233(.a(n290), .b(n239), .O(po16));
  xorx g234(.a(n107), .b(n101), .O(n292));
  andx g235(.a(n113), .b(pi19), .O(n293));
  andx g236(.a(n293), .b(n254), .O(n294));
  xorx g237(.a(n294), .b(n292), .O(n295));
  orx  g238(.a(n66), .b(pi14), .O(n296));
  andx g239(.a(n296), .b(n295), .O(po17));
  andx g240(.a(pi19), .b(pi02), .O(n298));
  andx g241(.a(n298), .b(n254), .O(n299));
  xorx g242(.a(n299), .b(n124), .O(n300));
  andx g243(.a(n300), .b(n296), .O(po18));
  andx g244(.a(pi19), .b(pi10), .O(n302));
  andx g245(.a(n302), .b(n254), .O(n303));
  xorx g246(.a(n303), .b(n159), .O(n304));
  andx g247(.a(n304), .b(n296), .O(po19));
  andx g248(.a(pi19), .b(pi18), .O(n306));
  andx g249(.a(n306), .b(n254), .O(n307));
  xorx g250(.a(n307), .b(n146), .O(n308));
  andx g251(.a(n308), .b(n296), .O(po20));
  andx g252(.a(n74), .b(pi19), .O(n310));
  andx g253(.a(n310), .b(n254), .O(n311));
  xorx g254(.a(n311), .b(n70), .O(n312));
  andx g255(.a(n312), .b(n296), .O(po21));
  andx g256(.a(n246), .b(n66), .O(n314));
  andx g257(.a(pi20), .b(n132), .O(n315));
  orx  g258(.a(n315), .b(n107), .O(n316));
  xorx g259(.a(n316), .b(n314), .O(n317));
  andx g260(.a(pi00), .b(pi03), .O(n318));
  orx  g261(.a(n318), .b(n66), .O(n319));
  xorx g262(.a(n319), .b(n317), .O(po22));
  andx g263(.a(n253), .b(n66), .O(n321));
  invx g264(.a(n87), .O(n322));
  andx g265(.a(n322), .b(n59), .O(n323));
  andx g266(.a(pi20), .b(n184), .O(n324));
  andx g267(.a(n87), .b(n60), .O(n325));
  orx  g268(.a(n325), .b(n324), .O(n326));
  orx  g269(.a(n326), .b(n323), .O(n327));
  xorx g270(.a(n327), .b(n321), .O(n328));
  andx g271(.a(pi32), .b(pi01), .O(n329));
  orx  g272(.a(n329), .b(n66), .O(n330));
  xorx g273(.a(n330), .b(n328), .O(po23));
  andx g274(.a(pi19), .b(pi05), .O(n332));
  andx g275(.a(n332), .b(n254), .O(n333));
  xorx g276(.a(n333), .b(n88), .O(n334));
  xorx g277(.a(n334), .b(n80), .O(n335));
  andx g278(.a(n335), .b(n296), .O(po24));
endmodule


