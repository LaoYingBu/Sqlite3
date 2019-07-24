#include "include/ncnn.hpp"

int detect_net(const cv::Mat& bgr, std::vector<float>& cls_scores)
{
    ncnn::Net net;
    #ifdef NCNN_VULKAN
    net.opt.use_vulkan_compute = true;
    #endif
    net.load_param("net.param");
    net.load_model("net.bin");
    ncnn::Mat in = ncnn::Mat::from_pixels_resize(bgr.data, ncnn::Mat::PIXEL_BGR2GRAY, bgr.cols, bgr.rows, 112, 112);
    ncnn::Extractor ex = net.create_extractor();
    ex.input("data", in);
    ncnn::Mat out;
    ex.extract("out", out);
    {
        ncnn::Layer* softmax = ncnn::create_layer("softmax");
        ncnn::ParamDict pd;
        softmax->load_param(pd);
        softmax->forward_inplace(out);
        delete softmax;
    }
    cls_scores.resize(out.w);
    for (int i = 0; i < out.w; i++)
    {
        cls_scores[i] = out[i];
    }
    return 0;
}

int print_topk(const std::vector<float>& cls_score, int topk)
{
    int size = cls_score.size();
    std::vector<std::pair<float, int>> vec;
    vec.resize(size);
    for (int i = 0; i < size; i++)
    {
        vec[i] = std::make_pair(cls_score[i], i);
    }
    std::partial_sort(vec.begin(), vec.begin() + topk, vec.end(), std::greater<std::pair<float, int>>());
    for (int i = 0; i < topk; i++)
    {
        fprintf(stdout, "%d = %f\n", vec[i].second, vec[i].first);
    }
    return 0;
}

int ncnn_demo(char* filename)
{
    cv::Mat im = cv::imread(filename, 0);
    if(im.empty())
    {
        fprintf(stderr, "load one empty image: %s.\n", filename);
    }
    #if NCNN_VULKAN
    ncnn::create_gpu_instance();
    #endif
    std::vector<float> cls_scores;
    detect_net(im, cls_scores);
    #if NCNN_VULKAN
    ncnn::destroy_gpu_instance();
    #endif
    print_topk(cls_scores, 3);
}