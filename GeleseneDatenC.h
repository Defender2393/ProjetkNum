//
// Created by bosch on 18.11.2025.
//

#ifndef UNTITLED_GELESENEDATENC_H
#define UNTITLED_GELESENEDATENC_H
#include <vector>
#include <array>

class GeleseneDatenC {
    std::pmr::vector<std::array <float, 3>> MainDatax;
    std::pmr::vector<std::array <float, 3>> MainDatay;
    std::pmr::vector<std::array <float, 3>> MainDataz;
    public:
    GeleseneDatenC(int* x, float** arrx,int y, float** arry){
        for (int i = 0; i < x[0]; i++) {
            MainDatax.push_back(0);
            MainDatax[i][0] = arrx[i][0];
            MainDatax[i][1] = arrx[i][1];
            MainDatax[i][2] = arrx[i][2];
        }
        for (int i = 0; i < x[1]; i++) {
            MainDatay.push_back(0);
            MainDatay[i][0] = arry[i][0];
            MainDatay[i][1] = arry[i][1];
            MainDatay[i][2] = arry[i][2];
        }
        for (int i = 0; i < x[3]; i++) {
            MainDatax.push_back(0);
            MainDataz[i][0] = arrz[i][0];
            MainDataz[i][1] = arrz[i][1];
            MainDataz[i][2] = arrz[i][2];
        }
    };

};


#endif //UNTITLED_GELESENEDATENC_H