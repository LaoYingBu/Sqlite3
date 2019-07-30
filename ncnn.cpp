#include "include/ncnn.hpp"
#include "dense-opt.mem.h"
#include "dense-opt.id.h"
#define STB_IMAGE_IMPLEMENTATION
#include "include/imageHelper/stb_image.h"


static void get_img_gravity_center_top(const uchar* src, const int W, const int H, float* sx, float* sy){
	const int srcsize_x = W;
	const int srcsize_y = H;
	const int margin = 0;
	float ax = 0;
	float ay = 0;
	float am = 0;
	for (int y = margin; y < srcsize_y - margin; y++)
		for (int x = margin; x < srcsize_x - margin; x++)
		{
			float pixel = 255 - src[y * srcsize_x + x];
			//float pixel = src[y * srcsize_x + x] < 128;
			ax += x * pixel;
			ay += y * pixel;
			am += pixel;
		}
	if (am == 0){
		*sx = srcsize_x / 2;
		*sy = srcsize_y / 2;
	}
	else{
		*sx = ax / am;
		*sy = ay / am;
	}
}
static void img_crop_center(const uchar* src, const int W, const int H, const float mean, const float scale, const int dstsize, float* dst){
	const int srcsize_x = W;
	const int srcsize_y = H;
	int dst_size = dstsize;
	int start_x = static_cast<int>((srcsize_x - dst_size) / 2);     //(500 - 112) / 2
	int start_y = static_cast<int>((srcsize_y - dst_size) / 2);     //(500 - 112) / 2
	for (int y = 0; y < dst_size; y++)
		for (int x = 0; x < dst_size; x++)
			dst[y * dst_size + x] = (src[(start_y + y) * W + (start_x + x)] - mean) * scale;
}
static void img_crop_gravity(const uchar* src, const int W, const int H, const float mean, const float scale, const int dstsize, float* dst){
	const int srcsize_x = W;
	const int srcsize_y = H;
	const int margin = 0;
	const int dst_size = dstsize;
	float x = 0., y = 0.;
	get_img_gravity_center_top(src, W, H, &x, &y);
	int sx = min(float(srcsize_x - margin - dst_size), max(float(margin), x - float(dst_size - 1) / 2)) + 0.5;
	int sy = min(float(srcsize_y - margin - dst_size), max(float(margin), y - float(dst_size - 1) / 2)) + 0.5;
	for (int y = 0; y < dst_size; y++)
		for (int x = 0; x < dst_size; x++)
			dst[y * dst_size + x] = (src[(sy + y) * W + (sx + x)] - mean) * scale;
}


//static int ncnn_detect_net(const cv::Mat& gray, std::vector<float>& cls_scores)
static int ncnn_detect_net(const uchar* gray, const int w, const int h, std::vector<float>& cls_scores)
{
    ncnn::Net net;
    #ifdef NCNN_VULKAN
    net.opt.use_vulkan_compute = true;
    #endif
    net.load_param(dense_opt_param_bin);
    net.load_model(dense_opt_bin);

    // ncnn::Mat in = ncnn::Mat::from_pixels_resize(gray.data, ncnn::Mat::PIXEL_GRAY, gray.cols, gray.rows, 160, 160);
    // const float norm_vals[3] = {1/256.f}; 
    // const float mean_vals[3] = {128.f}; 
    // in.substract_mean_normalize(mean_vals, norm_vals);
 
    static float tmp[160*160] = {0.};
    //img_crop_gravity(gray.data, gray.cols, gray.rows, 128., 1/256., 160, tmp);
    img_crop_gravity(gray, w, h, 128., 1/256., 160, tmp);
    ncnn::Mat in(160, 160, tmp);
    
    // cooling soc
    Sleep(10*1000);
    // warmup
    for (int i = 0; i < 10; i++)
    {
        ncnn::Extractor ex = net.create_extractor();
        ex.input(dense_opt_param_id::BLOB_data, in);
        ncnn::Mat out;
        ex.extract(dense_opt_param_id::BLOB_prob, out);
        // {
        //     ncnn::Layer* softmax = ncnn::create_layer("Softmax");
        //     ncnn::ParamDict pd;
        //     softmax->load_param(pd);
        //     softmax->forward_inplace(out);
        //     delete softmax;
        // }
    }

    double time_min = DBL_MAX, time_max = -DBL_MAX, time_ave = 0;
    ncnn::Mat out;
    for (int i = 0; i < 100000; i++)
    {
       double start = ncnn::get_current_time(); 
       ncnn::Extractor ex = net.create_extractor();
        ex.input(dense_opt_param_id::BLOB_data, in);
        ex.extract(dense_opt_param_id::BLOB_prob, out);
        // {
        //     ncnn::Layer* softmax = ncnn::create_layer("Softmax");
        //     ncnn::ParamDict pd;
        //     softmax->load_param(pd);
        //     softmax->forward_inplace(out);
        //     delete softmax;
        // }
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
    // cv::Mat im = cv::imread(filename, 0);
    int h, w, c;
    uchar *data = stbi_load(filename, &w, &h, &c, 1);
    fprintf(stderr, "%d-%d-%d\n", w, h, c);
    // if(im.empty())
    // {
    //     fprintf(stderr, "load one empty image: %s.\n", filename);
    // }
    #if NCNN_VULKAN
    ncnn::create_gpu_instance();
    #endif

    ncnn::Option opt;
    opt.lightmode = true;
    opt.num_threads = 2;
    opt.use_winograd_convolution = true;
    opt.use_sgemm_convolution =true;

    std::vector<float> cls_scores;
    //ncnn_detect_net(im, cls_scores);
    ncnn_detect_net(data, w, h, cls_scores);
    #if NCNN_VULKAN
    ncnn::destroy_gpu_instance();
    #endif
    ncnn_print_topk(cls_scores, 2);
    stbi_image_free(data);
}
