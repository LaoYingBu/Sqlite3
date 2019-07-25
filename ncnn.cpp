#include "include/ncnn.hpp"

static int ncnn_detect_net(const cv::Mat& bgr, std::vector<float>& cls_scores)
{
    ncnn::Net net;
    #ifdef NCNN_VULKAN
    net.opt.use_vulkan_compute = true;
    #endif
    net.load_param("net.param");
    net.load_model("net.bin");
    ncnn::Mat in = ncnn::Mat::from_pixels_resize(bgr.data, ncnn::Mat::PIXEL_GRAY2BGR, bgr.cols, bgr.rows, 227, 227);
    const float norm_vals[3] = {1/255.f, 1/255.f, 1/255.f}; 
    in.substract_mean_normalize(0, norm_vals);

    // cooling soc
    Sleep(10*1000);
    // warmup
    for (int i = 0; i < 10; i++)
    {
        ncnn::Extractor ex = net.create_extractor();
        ex.input("data", in);
        ncnn::Mat out;
        ex.extract("pool10", out);
        {
            ncnn::Layer* softmax = ncnn::create_layer("Softmax");
            ncnn::ParamDict pd;
            softmax->load_param(pd);
            softmax->forward_inplace(out);
            delete softmax;
        }
    }

    double time_min = DBL_MAX, time_max = -DBL_MAX, time_ave = 0;
    ncnn::Mat out;
    for (int i = 0; i < 10; i++)
    {
       double start = ncnn::get_current_time(); 
       ncnn::Extractor ex = net.create_extractor();
        ex.input("data", in);
        ex.extract("pool10", out);
        {
            ncnn::Layer* softmax = ncnn::create_layer("Softmax");
            ncnn::ParamDict pd;
            softmax->load_param(pd);
            softmax->forward_inplace(out);
            delete softmax;
        }
       double end = ncnn::get_current_time();
       double time = end - start;
       time_min = min(time_min, time);
       time_max = max(time_max, time);
       time_ave += time;
    }
    time_ave /= 10;
    fprintf(stdout, "%20s   min = %7.2f max = %7.2f ave = %7.2f\n", "net", time_min, time_max, time_ave);
    
    cls_scores.resize(out.w);
    for (int i = 0; i < out.w; i++)
    {
        cls_scores[i] = out[i];
    }
    return 0;
}

static int ncnn_print_topk(const std::vector<float>& cls_score, int topk)
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

    ncnn::Option opt;
    opt.lightmode = true;
    opt.num_threads = 2;
    opt.use_winograd_convolution = true;
    opt.use_sgemm_convolution =true;

    std::vector<float> cls_scores;
    ncnn_detect_net(im, cls_scores);
    #if NCNN_VULKAN
    ncnn::destroy_gpu_instance();
    #endif
    ncnn_print_topk(cls_scores, 3);
}
