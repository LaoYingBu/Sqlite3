#include "include/cv.hpp"

int cv_demo(char* filename)
{
    Mat im;
    try{
        im = imread(filename, 0);
    }
    catch(Exception e){
        fprintf(stderr, "read data err: %s\n", e.what());
    }
    if(im.data){
        fprintf(stderr, "load data err, is NULL!");
    }
    imshow("lzw", im);
    waitKey(0);
    return 0;
}
