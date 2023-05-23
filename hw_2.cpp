#include "hw_2.h"
#include <iostream>
#include <cstring>
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
    int ver;
    int ger;
    fscanf(flog,"%d\t%d",&ver,&ger);
    srcImg=new Img(ver,ger);
    processedImg=new Img(ver,ger);
    char* buffer = new char[ver*ger];
    char buf;
    int k=0;
    while (fscanf(flog,"%c",&buf)!=EOF) {
        if(buf!='\n'&& buf!='\t'){
            buffer[k]=buf;
            k++;
        }
    }
    buffer[ver*ger]='\0';
    for (int i = 0; i < srcImg->width*srcImg->height; ++i) {
        srcImg->srcImg[i]=buffer[i]-'0';
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

        for (int i = 0; i < img->width*img->height; ++i) {
            srcImg->srcImg[i]=img->srcImg[i];

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
    int* buffer=new int[temp->height*temp->height];
    for (int i = 0; i <temp->height*temp->height ; ++i) {
        buffer[i]=temp->srcImg[i];
    }
    int buf_xc=temp->x_c;
    int buf_yc=temp->y_c;
    int fir_buf=temp->width;
    int sec_buf=temp->height;
    int korob_size_w=temp->width;
    int korob_size_h=temp->height;
    int* korob = new int [korob_size_h*korob_size_w];
    for (int i = 0; i < temp->width*temp->height; ++i) {
        korob[i]=temp->srcImg[i];
    }
    delete temp;
    temp=new Img(fir_buf+2*buf_xc,sec_buf+2*buf_yc);
    int g=0;
    for (int i = 0,k=0; k < temp->height; i+=temp->width,k++) {
        for (int j = i,l=0; l <temp->width; ++j,l++) {
            if(j==temp->height*temp->width){
                break;
            }
            if(k<buf_yc||k>sec_buf+buf_yc-1||l<buf_xc||l>fir_buf+buf_xc-1){
                temp->srcImg[j]=0;
            }else{
                temp->srcImg[j]=buffer[g];
                g++;
            }
        }
    }
    delete[] buffer;
    buffer=new int[temp->width*temp->height];
    for (int i = 0; i < temp->width*temp->height; ++i) {
        buffer[i]=temp->srcImg[i];
    }
    for (int i = 0; i < temp->width*temp->height; i++) {
        if(mask->srcImg[mask->y_c*mask->width+mask->x_c]== temp->srcImg[i]){
            for (int l = 0,z=0, y = i - mask->y_c*(temp->width-mask->width)-mask->y_c*mask->width-mask->x_c
                    ;l < mask->height; ++l) {
                for (int x = y,f=z,k=0; k < mask->width; ++k,x++,f++) {
                    if (mask->srcImg[f] != temp->srcImg[x]) {
                        buffer[x]=1;
                    }
                }
                z+=mask->width;
                y += (temp->width - mask->width)+mask->width;
            }
        }
    }
    int last_w=temp->width;
    int last_h=temp->height;
    if(srcImg==1){
        clearImg(this->srcImg);
        this->srcImg= new Img(korob_size_w,korob_size_h);
        for (int i = 0; i <korob_size_w*korob_size_h ; ++i) {
            this->srcImg->srcImg[i]=korob[i];
        }
    }
    clearImg(this->processedImg);
    this->processedImg=new Img(this->srcImg->width,this->srcImg->height);
    int count=0;
    for (int i = 0,k=0; k < last_h; i+=last_w,k++) {
        for (int j = i,l=0; l <last_w; ++j,l++) {
            if(k<buf_yc||k>sec_buf+buf_yc-1||l<buf_xc||l>fir_buf+buf_xc-1){
                continue;
            }else{
                this->processedImg->srcImg[count]=buffer[j];
                count++;
            }
        }
    }

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
    int* buffer=new int[temp->height*temp->height];
    for (int i = 0; i <temp->height*temp->height ; ++i) {
        buffer[i]=temp->srcImg[i];
    }
    int buf_xc=temp->x_c;
    int buf_yc=temp->y_c;
    int fir_buf=temp->width;
    int sec_buf=temp->height;
    int korob_size_w=temp->width;
    int korob_size_h=temp->height;
    int* korob = new int [korob_size_h*korob_size_w];
    for (int i = 0; i < temp->width*temp->height; ++i) {
        korob[i]=temp->srcImg[i];
    }
    delete temp;
    temp=new Img(fir_buf+2*buf_xc,sec_buf+2*buf_yc);
    int g=0;
    for (int i = 0,k=0; k < temp->height; i+=temp->width,k++) {
        for (int j = i,l=0; l <temp->width; ++j,l++) {
            if(j==temp->height*temp->width){
                break;
            }
            if(k<buf_yc||k>sec_buf+buf_yc-1||l<buf_xc||l>fir_buf+buf_xc-1){
                temp->srcImg[j]=0;
            }else{
                temp->srcImg[j]=buffer[g];
                g++;
            }
        }
    }
    delete[] buffer;
    buffer=new int[temp->width*temp->height];
    for (int i = 0; i < temp->width*temp->height; ++i) {
        buffer[i]=temp->srcImg[i];
    }
    for (int i = 0; i < temp->width*temp->height; i++) {
        if(mask->srcImg[mask->y_c*mask->width+mask->x_c]== temp->srcImg[i]){
            for (int l = 0,z=0, y = i - mask->y_c*(temp->width-mask->width)-mask->y_c*mask->width-mask->x_c
                    ;l < mask->height; ++l) {
                for (int x = y,f=z,k=0; k < mask->width; ++k,x++,f++) {
                    if (mask->srcImg[f] != temp->srcImg[x]&&mask->srcImg[f]==1) {
                        buffer[i]=0;
                    }

                }
                z+=mask->width;
                y += (temp->width - mask->width)+mask->width;
            }
        }
    }
    int last_w=temp->width;
    int last_h=temp->height;
    if(srcImg==1){
        clearImg(this->srcImg);
        this->srcImg= new Img(korob_size_w,korob_size_h);
        for (int i = 0; i <korob_size_w*korob_size_h ; ++i) {
            this->srcImg->srcImg[i]=korob[i];
        }
    }
    clearImg(this->processedImg);
    this->processedImg=new Img(this->srcImg->width,this->srcImg->height);
    int count=0;
    for (int i = 0,k=0; k < last_h; i+=last_w,k++) {
        for (int j = i,l=0; l <last_w; ++j,l++) {
            if(k<buf_yc||k>sec_buf+buf_yc-1||l<buf_xc||l>fir_buf+buf_xc-1){
                continue;
            }else{
                this->processedImg->srcImg[count]=buffer[j];
                count++;
            }
        }
    }

    return 0;
}

int ImageProcess::saveImgToFile(const char *fileName, int format) {
    FILE* flog = fopen(fileName, "w");
    if(format==1) {
        for (int i = 0; i < this->srcImg->height * this->srcImg->width; i += this->srcImg->width) {
            for (int j = i, l = 0; l < this->srcImg->width; ++j, l++) {
                fprintf(flog, "%d", this->srcImg->srcImg[j]);
            }
            fprintf(flog, "\n");

        }
    }
    if(format==0){
        for (int i = 0; i < this->srcImg->height*this->srcImg->width; i+=this->srcImg->width) {
            for (int j = i,l=0; l <this->srcImg->width ; ++j,l++) {
                fprintf(flog,"%d",this->srcImg->srcImg[j]);
            }
        }
    }
    fclose(flog);
}

int ImageProcess::loadImgFromFile(const char *fileName, int format) {
    FILE* flog=fopen(fileName,"r");
    int ver;
    int ger;
    fscanf(flog,"%d\t%d",&ver,&ger);
    if(format==1) {
        clearImg(this->srcImg);
        srcImg = new Img(ver, ger);
        char *buffer = new char[ver * ger];
        char buf;
        int k = 0;
        while (fscanf(flog, "%c", &buf) != EOF) {
            if (buf != '\n' && buf != '\t') {
                buffer[k] = buf;
                k++;
            }
        }
        buffer[ver * ger] = '\0';
        for (int i = 0; i < srcImg->width * srcImg->height; ++i) {
            srcImg->srcImg[i] = buffer[i] - '0';
        }
    }else{
        if(format==0){
            clearImg(this->srcImg);
            srcImg=new Img(ver,ger);
            char* buffer = new char[ver*ger];
            char buf;
            int k=0;
            while (fscanf(flog,"%c",&buf)!=EOF) {
                if(buf!='\n'&& buf!='\t'){
                    buffer[k]=buf;
                    k++;
                }
            }
            buffer[ver*ger]='\0';
            for (int i = 0; i < srcImg->width*srcImg->height; ++i) {
                srcImg->srcImg[i]=buffer[i]-'0';
            }
            fclose(flog);
        }
    }
    fclose(flog);
}
void ImageProcess::clearImg(Img *img) {
    delete img;
    img= nullptr;
}

//
// Created by User on 20.05.2023.
//
