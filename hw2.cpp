#include "hw2.h"
#include <iostream>
Img::Img() {
    srcImg=new int[128];
    width=64;
    height=64;
    x_c=64;
    y_c=64;
}
Img::Img(int w, int h) {
    srcImg=new int [w*h];
    height=h;
    width=w;
    y_c=h/2;
    x_c=w/2;
}
Img::Img(const int *src, int w, int h) {
    this->srcImg=new int[w*h];
    width=w;
    height=h;
    x_c=w/2;
    y_c=h/2;
    for (int i = 0; i <w*h ; ++i) {
        this->srcImg[i]=src[i];
    }
}
Img::~Img(){
    delete[] srcImg;
    srcImg= nullptr;
}
ImageProcess::ImageProcess() {
    srcImg=new Img;
    processedImg=new Img;
    mask=new Img(3,3);
    for (int i = 0; i < mask->width*mask->width; ++i) {
        mask->srcImg[i]=0;
        if(i==1||i==3||i==4||i==5||i==7){
            mask->srcImg[i]=1;
        }
    }
}
ImageProcess::ImageProcess(int w, int h) {
    srcImg=new Img(w,h);
    processedImg=new Img(w,h);
    mask=new Img(3,3);
    for (int i = 0; i < mask->width*mask->width; ++i) {
        mask->srcImg[i]=0;
        if(i==1||i==3||i==4||i==5||i==7){
            mask->srcImg[i]=1;
        }
    }
}
ImageProcess::ImageProcess(const char *fileName) {
    FILE* flog=fopen(fileName,"r");
    int ver,ger;
    fscanf(flog,"%d\t%d",&ver,&ger);
    srcImg=new Img(ver,ger);
    processedImg=new Img(ver,ger);
    char buf;
    int k=0;
    while (fscanf(flog,"%c",&buf)!=EOF) {
        if(buf!='\n'&& buf!='\t'){
            srcImg->srcImg[k]=buf-'0';
            k++;
        }
    }
    fclose(flog);
    mask= new Img(3,3);
    for (int i = 0; i < mask->width*mask->width; ++i) {
        mask->srcImg[i]=0;
        if(i==1||i==3||i==4||i==5||i==7){
            mask->srcImg[i]=1;
        }
    }
}
ImageProcess::ImageProcess(const Img *img) {
    srcImg=new Img(img->width,img->height);
    processedImg=new Img(img->width,img->height);
    mask= new Img(3,3);
    for (int i = 0; i < mask->width*mask->width; ++i) {
        mask->srcImg[i]=0;
        if(i==1||i==3||i==4||i==5||i==7){
            mask->srcImg[i]=1;
        }

        for (int j = 0; j < img->width*img->height; ++j) {
            srcImg->srcImg[j]=img->srcImg[j];

        }
    }
}
ImageProcess::~ImageProcess() {
    delete srcImg;
    delete processedImg;
    delete mask;
    mask= nullptr;
    srcImg= nullptr;
    processedImg= nullptr;

}

int ImageProcess::updateMask(const Img &mask) {
    delete this->mask;
    this->mask = new Img(mask.width,mask.height);
    for (int i = 0; i < mask.width*mask.height; ++i) {
        this->mask->srcImg[i]=mask.srcImg[i];
    }
    return 0;
}

int ImageProcess::updateSrcImg() {
    Img* buf;
    buf=this->srcImg;
    this->srcImg=this->processedImg;
    this->processedImg=buf;
    return 0;
}
int ImageProcess::dilatation(int srcImg) {
    Img* temp;
    if(srcImg==1){
        temp=this->srcImg;
    }
    if(srcImg==0){
        temp=this->processedImg;
    }
    Img* buffer;
    buffer=make_buff_img(buffer,temp->width+2*temp->x_c,temp->height+2*temp->y_c);
    Img* sec_buffer;
    sec_buffer=make_buff_img(sec_buffer,temp->width+2*temp->x_c,temp->height+2*temp->y_c);
    for (int i = 0; i < buffer->width*buffer->height; i++) {
        if(mask->srcImg[mask->y_c*mask->width+mask->x_c]== buffer->srcImg[i]){
            for (int l = 0,z=0, y = i - mask->y_c*(buffer->width-mask->width)-mask->y_c*mask->width-mask->x_c
                                                                                         ;l < mask->height; ++l) {
                for (int x = y,f=z,k=0; k < mask->width; ++k,x++,f++) {
                    if (mask->srcImg[f] != buffer->srcImg[x]) {
                        sec_buffer->srcImg[x]=1;
                    }
                }
                z+=mask->width;
                y += (buffer->width - mask->width)+mask->width;
            }
        }
    }
    save_to_proccesed(sec_buffer);
    delete buffer;
    delete sec_buffer;
    return 0;
}

int ImageProcess::erosion(int srcImg) {
    Img* temp;
    if(srcImg==1){
        temp=this->srcImg;
    }
    if(srcImg==0){
        temp=this->processedImg;
    }
    Img* buffer;
    buffer=make_buff_img(buffer,temp->width+2*temp->x_c,temp->height+2*temp->y_c);
    Img* sec_buffer;
    sec_buffer=make_buff_img(sec_buffer,temp->width+2*temp->x_c,temp->height+2*temp->y_c);
    for (int i = 0; i < buffer->width*buffer->height; i++) {
        if(mask->srcImg[mask->y_c*mask->width+mask->x_c]== buffer->srcImg[i]){
            for (int l = 0,z=0, y = i - mask->y_c*(buffer->width-mask->width)-mask->y_c*mask->width-mask->x_c
                    ;l < mask->height; ++l) {
                for (int x = y,f=z,k=0; k < mask->width; ++k,x++,f++) {
                    if (mask->srcImg[f] != buffer->srcImg[x]&&mask->srcImg[f]==1) {
                        sec_buffer->srcImg[i]=0;
                    }
                }
                z+=mask->width;
                y += (buffer->width - mask->width)+mask->width;
            }
        }
    }
    save_to_proccesed(sec_buffer);
    delete buffer;
    delete sec_buffer;
    return 0;
}

int ImageProcess::saveImgToFile(const char *fileName, int format) {
    FILE* flog = fopen(fileName, "w");
    for (int i = 0; i < processedImg->height * processedImg->width; i += processedImg->width) {
        for (int j = i, l = 0; l < srcImg->width; ++j, l++) {
            fprintf(flog, "%d", processedImg->srcImg[j]);
        }
        if(format==1) {
            fprintf(flog, "\n");
        }
    }
    fclose(flog);
    return 0;
}

int ImageProcess::loadImgFromFile(const char *fileName, int format) {
    FILE* flog=fopen(fileName,"r");
    int ver;
    int ger;
    fscanf(flog,"%d\t%d",&ver,&ger);
    if(format==1||format==0) {
        clearImg(this->srcImg);
        srcImg = new Img(ver, ger);
        char buf;
        int k=0;
        while (fscanf(flog,"%c",&buf)!=EOF) {
            if (buf != '\n' && buf != '\t') {
                srcImg->srcImg[k] = buf - '0';
                k++;
            }
        }

    }
    fclose(flog);
    return 0;
}
void ImageProcess::clearImg(Img *img) {
    delete img;
}
Img* ImageProcess::make_buff_img(Img* temp,int width,int height) {
    temp=new Img(width,height);
    int g=0;
    for (int i = 0,k=0; k < temp->height; i+=temp->width,k++) {
        for (int j = i,l=0; l <temp->width; ++j,l++) {
            if(k<srcImg->y_c||k>srcImg->height+srcImg->y_c-1||l<srcImg->x_c||l>srcImg->width+srcImg->x_c-1){
                temp->srcImg[j]=0;
            }else{
                temp->srcImg[j]=this->srcImg->srcImg[g];
                g++;
            }
        }
    }
    return temp;
}

void ImageProcess::save_to_proccesed(Img *sec_temp) {
    clearImg(processedImg);
    processedImg=new Img(srcImg->width,srcImg->height);
    int count=0;
    for (int i = 0,k=0; k < sec_temp->height; i+=sec_temp->width,k++) {
        for (int j = i,l=0; l <sec_temp->width; ++j,l++) {
            if(k<processedImg->y_c||k>processedImg->height+processedImg->y_c-1||l<processedImg->x_c
                                                                    ||l>processedImg->width+processedImg->x_c-1){
                continue;
            }else{
                this->processedImg->srcImg[count]=sec_temp->srcImg[j];
                count++;
            }
        }
    }
}

//
// Created by User on 20.05.2023.
//
