//
// Created by bosch on 18.11.2025.
//

#ifndef UNTITLED_GELESENEDATENC_H
#define UNTITLED_GELESENEDATENC_H
#include <vector>
#include <array>

class GeleseneDatenC {
public:
    int LineNumber[4]{};
    std::pmr::vector<std::array <float, 12>> LineContent;
    int kinematicNumber;
    std::pmr::vector<std::array <float, 8>> kinematicContent;
    std::array <float, 12> helperLine{} ;
    std::array <float, 8> helperkinematic{};

    GeleseneDatenC(const int* x, float arrx[1625][12],int y, float arry[1625][8]){
        for (int j = 0; j < 4; j++) {
            LineNumber[j] = x[j];
        }
        kinematicNumber = y;
        for (int j = 0; j < LineNumber[0]; j++) {
            LineContent.push_back(helperLine);
        }
        for (int j = 0; j < 4; j++) {
            for (int i = 0+3*j; i < 3+3*j; i++) {
                for (int k = 0; k < LineNumber[j]; k++) {
                    LineContent[i][k] = arrx[i][k];
                }
            }
        }
        for (int j = 0; j < kinematicNumber; j++) {
            kinematicContent.push_back(helperkinematic);
            for (int i = 0; i < 8; i++){
                kinematicContent[j][i] = LineContent[j][i];
        }
        }



    };



};


#endif //UNTITLED_GELESENEDATENC_H