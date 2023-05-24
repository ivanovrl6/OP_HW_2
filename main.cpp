
#include "hw2.h"
int main() {
    ImageProcess fir("hw2.txt");
    fir.dilatation(1);
    fir.erosion(1);
    fir.updateSrcImg();
    fir.saveImgToFile("results.txt",1);
    return 0;
}
