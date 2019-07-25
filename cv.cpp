#include "include/cv.hpp"
#include <exception>
#include <opencv2/dnn.hpp>

static int cv_detect_net(const cv::Mat bgr)
{
    cv::dnn::Net net = cv::dnn::readNet("net.prototxt", "net.caffemodel");
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    cv::Mat blob;
      
    Sleep(10*1000);
    for (int i = 0; i < 10; i++)
    {
        //cv::Mat clone = bgr.clone();
        cv::dnn::blobFromImage(bgr, blob, 1/255., cv::Size(227, 227), cv::Scalar(0, 0, 0), 0);
        net.setInput(blob, "data");
        cv::Mat pred = net.forward("prob"); 
    }
    fprintf(stdout, "OK!\n");

    cv::Mat pred;
    double time_min = DBL_MAX, time_max = -DBL_MAX, time_ave = 0;
    double freq = cv::getTickFrequency();
    for (int i = 0; i < 10; i++)
    {
        double start = cv::getTickCount()*1000/freq;
        //cv::Mat clone = bgr.clone();
        cv::dnn::blobFromImage(bgr, blob, 1/255., cv::Size(227, 227), cv::Scalar(0, 0, 0), 0);
        net.setInput(blob, "data");
        pred = net.forward("prob");
        double end = cv::getTickCount()*1000/freq;
        double time = end - start;
        time_min = min(time_min, time);
        time_max = max(time_max, time);
        time_ave += time;
    }
    time_ave /= 10;
    fprintf(stdout, "%20s   min = %7.2f max = %7.2f ave = %7.2f\n", "net", time_min, time_max, time_ave);
    
    cv::Point classIdPoint;
    double confidence;
    cv::minMaxLoc(pred.reshape(1, 1), 0, &confidence, 0, &classIdPoint);
    int classId =classIdPoint.x;
    char label[256];
    sprintf(label, "Inference time: %7.2f ms\n", time_ave);
    cv::putText(bgr, label, cv::Point(0, 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));
    sprintf(label, "%d= %.8f", classId, confidence);
    cv::putText(bgr, label, cv::Point(0, 40), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));
    cv::imshow("LZW", bgr);
    cv::waitKey(0);
    return 0;
}

int cv_demo(char* filename)
{
    cv::Mat im;
    try{
        im = cv::imread(filename, 1);
    }
    catch(std::exception& e){
        fprintf(stderr, "read data err: %s\n", e.what());
    }
    if(im.empty()){
        fprintf(stderr, "load data err, is NULL!");
    }
    cv_detect_net(im);
    return 0;
}
