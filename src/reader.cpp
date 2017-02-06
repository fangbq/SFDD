#include "reader.h"

Reader::Reader(const char* pro_file) {
    ifstream p_f(pro_file);  // 打开要写入的文本文件
    if(!p_f.is_open()) {
        string pro_str(pro_file);
        cout << "cannot open " << pro_str << endl;
        return;
    }

    string tmp; p_f >> tmp; p_f >> tmp;
    p_f >> var_num;
    p_f >> cla_num;
    // cout << var_num << " " << cla_num << endl;
    while (!p_f.eof()) {
        vector<int> cla;
        int index;
        p_f >> index;
        while (index != 0) {
            // cout << "index: " << index << endl;
            cla.push_back(index);
            p_f >> index;
        }
        if (cla.size() != 0)
            fml.push_back(cla);
    }

    // // print
    // cout << "clause num : " << fml.size() << endl;
    // for (vector<vector<int> >::const_iterator cla = fml.begin(); cla != fml.end(); ++cla)
    // {
    //     for (vector<int>::const_iterator lit = cla->begin(); lit != cla->end(); ++lit)
    //         cout << *lit << " ";
    //     cout << endl;
    // }

	p_f.close();
    return;
}

SFDD Reader::get_SFDD() const {
    SFDD sfdd;
    return sfdd;
}
