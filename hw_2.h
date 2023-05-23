
//
// Created by User on 20.05.2023.
//

#ifndef DILATION_HW_2_H
#define DILATION_HW_2_H
#include <list>
struct Img {
        int* srcImg;
        int width;
        int height;

        int x_c;
        int y_c;

        Img();
        Img(int w, int h);
        Img( const int* src, int w, int h);
        ~Img();
};

class ImageProcess {
private :
    Img* srcImg;
    Img* processedImg;
    Img* mask;
    void clearImg(Img* img);
public :
    ImageProcess();

    ImageProcess(int w, int h);

    ImageProcess(const Img* img);

     ImageProcess(const char* fileName);
    ~ImageProcess();

    int updateMask(const Img& mask);

    int updateSrcImg();

    int dilatation(int srcImg);

    int erosion(int srcImg);

    int loadImgFromFile(const char* fileName, int format);
    int saveImgToFile(const char* fileName, int format);

    std::list<std::list<std::pair<int /*x*/,int /*y*/>>> getListContours();
};
#endif //DILATION_HW_2_H
