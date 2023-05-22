
#include "hw_2.h"
int main() {
    ImageProcess fir("hw2.txt");
    int* ar=new int[6]{1,1,1};
    Img* massk=new Img(ar,3,1);
    fir.updateMask(*massk);
    fir.erosion(1);
    return 0;
}
