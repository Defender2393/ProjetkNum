//
// Created by bosch on 18.11.2025.
//

#ifndef UNTITLED_GELESENEDATENU_H
#define UNTITLED_GELESENEDATENU_H
#include <vector>
#include <array>


class GeleseneDatenU {
    std::pmr::vector<std::array <float, 3>> MainDatax;
public:
    GeleseneDatenU(int x, float** arrx){
        for (int i = 0; i < x; i++) {
            MainDatax.push_back(0);
            MainDatax[i][0] = arrx[i][0];
            MainDatax[i][1] = arrx[i][1];
            MainDatax[i][2] = arrx[i][2];
        }
    };
};


#endif //UNTITLED_GELESENEDATENU_H