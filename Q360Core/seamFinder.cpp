//
//  SeamFind.cpp
//  PhotoTrans
//
//  Created by Hyun Joon Shin on 11/1/14.
//  Copyright (c) 2014 Hyun Joon Shin. All rights reserved.
//
 
#include "SeamFinder.h"
//#include <opencv/cv.hpp>
//#include "opencv2/stitching/detail/seam_finders.hpp"
#include <opencv2/imgProc/imgProc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/stitching/detail/blenders.hpp>
 
#include "GCGraph.h"
#define TAG "SeamFinder"
using namespace cv;
using namespace cv::detail;
 
 
static inline bool overlapRoi(cv::Point tl1, cv::Point tl2, cv::Size sz1, cv::Size sz2, cv::Rect &roi)
{
    int x_tl = max(tl1.x, tl2.x), y_tl = max(tl1.y, tl2.y);
    int x_br = min(tl1.x + sz1.width, tl2.x + sz2.width);
    int y_br = min(tl1.y + sz1.height, tl2.y + sz2.height);

    if (x_tl < x_br && y_tl < y_br) { roi = cv::Rect(x_tl, y_tl, x_br - x_tl, y_br - y_tl); return true; }
    return false;
}
 
static inline float normL2(const cv::Point3f& a) { return a.x * a.x + a.y * a.y + a.z * a.z; }
static inline float normL2(const cv::Point3f& a, const cv::Point3f& b) { return normL2(a - b); }
static inline float normL2(const cv::Vec3b& a, const cv::Vec3b& b) { float d0 = (float)a[0]-b[0], d1 = (float)a[1]-b[1], d2 = (float)a[2]-b[2]; return d0*d0+d1*d1+d2*d2; }
static inline float normL2(const cv::Vec3s& a, const cv::Vec3s& b) { float d0 = (float)a[0]-b[0], d1 = (float)a[1]-b[1], d2 = (float)a[2]-b[2]; return d0*d0+d1*d1+d2*d2; }
static inline float normL2(const cv::Vec3f& a, const cv::Vec3f& b) { float d0 = (float)a[0]-b[0], d1 = (float)a[1]-b[1], d2 = (float)a[2]-b[2]; return d0*d0+d1*d1+d2*d2; }
static inline float normL2(const cv::Vec4b& a, const cv::Vec4b& b) { float d0 = (float)a[0]-b[0], d1 = (float)a[1]-b[1], d2 = (float)a[2]-b[2]; return d0*d0+d1*d1+d2*d2; }
static inline float normL2(const cv::Vec4s& a, const cv::Vec4s& b) { float d0 = (float)a[0]-b[0], d1 = (float)a[1]-b[1], d2 = (float)a[2]-b[2]; return d0*d0+d1*d1+d2*d2; }
static inline float normL2(const cv::Vec4f& a, const cv::Vec4f& b) { float d0 = (float)a[0]-b[0], d1 = (float)a[1]-b[1], d2 = (float)a[2]-b[2]; return d0*d0+d1*d1+d2*d2; }
 
template<class T> static void setGraphWeightsColorC3(const Mat &img1, const Mat &img2,
                          const Mat &mask1, const Mat &mask2, GCGraph<float> &graph,
                          float terminal_cost_=10000.f, float bad_region_penalty_ = 1000.f) {
    const Size img_size = img1.size(); int w=img_size.width, h=img_size.height;
    for(int y=0; y<h; ++y) {
        const uchar *m1 = mask1.ptr<uchar>(y), *m2=mask2.ptr<uchar>(y);
        for (int x=0; x<w; ++x) graph.addTermWeights(graph.addVtx(), *m1++?terminal_cost_:0.f, *m2++?terminal_cost_:0.f);
    }
    const float weight_eps = 1.f;
    float weight;
    for(int y=0; y<h-1; ++y) {
        const T *p01=img1.ptr<T>(y), *py1=(T*)((uchar*)p01+img1.step);
        const T *p02=img2.ptr<T>(y), *py2=(T*)((uchar*)p02+img2.step);
        const uchar *m01 = mask1.ptr<uchar>(y), *my1=m01+mask1.step;
        const uchar *m02 = mask2.ptr<uchar>(y), *my2=m02+mask2.step;
        int v = y*w;
        for (int x = 0; x < w-1; ++x) {
            float e00 =normL2(*p01, *p02);
            weight = e00 + normL2(*(p01+1), *(p02+1)) + weight_eps;
            if (!*m01 || !*(m01+1) || !*m02 || !*(m02+1)) weight += bad_region_penalty_;
            graph.addEdges(v, v + 1, weight, weight);
            weight = e00 + normL2(*py1, *py2) + weight_eps;
            if (!*m01 || !*my1 || !*m02 || !my2 ) weight += bad_region_penalty_;
            graph.addEdges(v, v + w, weight, weight);
            v++; m01++; m02++; my1++; my2++; p01++; p02++; py1++; py2++;
        }
        int x = w-1;
        v = y * w + x;
        weight = normL2(*p01, *p02)+normL2(*py1,*py2) + weight_eps;
        if (!*m01 || !*my1 || !*m02 || !*my2) weight += bad_region_penalty_;
        graph.addEdges(v, v + img_size.width, weight, weight);
    }
    int y= h-1, v=y*w;
    const T *p01=img1.ptr<T>(y), *p02=img2.ptr<T>(y);
    const uchar *m01 = mask1.ptr<uchar>(y), *m02 = mask2.ptr<uchar>(y);
    for (int x = 0; x < w-1; ++x) {
        weight = normL2(*p01, *p02) + normL2(*(p01+1), *(p02+1)) + weight_eps;
        if (!*m01 || !*(m01+1) || !*m02 || !*(m02+1)) weight += bad_region_penalty_;
        graph.addEdges(v, v + 1, weight, weight);
        v++; m01++; m02++; p01++; p02++;
    }
}
 
template<class T> static void setGraphWeightsColorC4
(Mat &img1, Mat &img2, GCGraph<float> &graph, float terminal_cost_=1000000.f, float bad_region_penalty_ = 100000.f) {
    const float weight_eps = 1.f;
    float weight;
    const Size img_size = img1.size(); int w=img_size.width, h=img_size.height;
    for(int y=0; y<h; ++y) {
        T *p1 = img1.ptr<T>(y), *p2=img2.ptr<T>(y);
        for (int x=0; x<w; ++x) graph.addTermWeights(graph.addVtx(), (*p1++)[3]?terminal_cost_:0.f, (*p2++)[3]?terminal_cost_:0.f);
    }
    for(int y=0; y<h-1; ++y) {
        T *p01=img1.ptr<T>(y), *py1=(T*)((uchar*)p01+img1.step);
        T *p02=img2.ptr<T>(y), *py2=(T*)((uchar*)p02+img2.step);
        int v = y*w;
        for (int x = 0; x < w-1; ++x) {
            float e00 =normL2(*p01, *p02);
            weight = e00 + normL2(*(p01+1), *(p02+1)) + weight_eps;
            if(!(*p01)[3] || !(*(p01+1))[3] || !(*p02)[3] || !(*(p02+1))[3]) weight += bad_region_penalty_;
            graph.addEdges(v, v + 1, weight, weight);
            weight = e00 + normL2(*py1, *py2) + weight_eps;
            if(!(*p01)[3] || !(*py1)[3] || !(*p02)[3] || !(*py2)[3] ) weight += bad_region_penalty_;
            graph.addEdges(v, v + w, weight, weight);
            v++; p01++; p02++; py1++; py2++;
        }
        weight = normL2(*p01, *p02)+normL2(*py1,*py2) + weight_eps;
        if (!(*p01)[3] || !(*py1)[3] || !(*p02)[3] || !(*py2)[3] ) weight += bad_region_penalty_;
        graph.addEdges(v, v + img_size.width, weight, weight);
    }
    int y= h-1, v=y*w;
    const T *p01=img1.ptr<T>(y), *p02=img2.ptr<T>(y);
    for (int x = 0; x < w-1; ++x) {
        weight = normL2(*p01, *p02) + normL2(*(p01+1), *(p02+1)) + weight_eps;
        if(!(*p01)[3] || !(*(p01+1))[3] || !(*p02)[3] || !(*(p02+1))[3]) weight += bad_region_penalty_;
        graph.addEdges(v, v + 1, weight, weight);
        v++; p01++; p02++;
    }
}
 
static inline void setMaskFromGraph(const GCGraph<float>& graph, Mat& mask1, const Point& tl1, Mat& mask2, const Point& tl2, int gap, const Rect& roi) {
    for (int y = 0; y < roi.height; ++y) {
        uchar* m1=mask1.ptr(roi.y - tl1.y + y)+roi.x - tl1.x;
        uchar* m2=mask2.ptr(roi.y - tl2.y + y)+roi.x - tl2.x;
        int idx = (y + gap) * (roi.width + 2 * gap) + gap;
        for (int x = 0; x < roi.width; ++x)
            if( graph.vtcs[idx++].t == 0 )  if (*m1++) *m2++=0; else m2++;
            else                            if (*m2++) *m1++=0; else m1++;
    }
}
 
template<class T> void findSeamInPairC3(int ww, const Mat& img1, Mat& mask1, const Point& tl1,
                  const Mat& img2, Mat& mask2, const Point& tl2, int type, int gap ) {
    Rect roi;
    if( !overlapRoi(tl1, tl2, img1.size(), img2.size(), roi)) return;
    const int w = roi.width+2*gap, h = roi.height+2*gap;
    GCGraph<float> graph(w*h, w*h*2-w-h);
 
    Mat subimg1=Mat::zeros(h,w,type), subimg2 = Mat::zeros(h,w,type), submask1=Mat::zeros(h,w,CV_8U), submask2=Mat::zeros(h,w,CV_8U);
    Rect roiGapped(roi.tl() + Point(-gap,-gap), subimg1.size()), roi1, roi2;
    overlapRoi(roiGapped.tl(), tl1, roiGapped.size(), img1.size(), roi1 );
    overlapRoi(roiGapped.tl(), tl2, roiGapped.size(), img2.size(), roi2 );
 
    img1(Rect(roi1.tl()-tl1,roi1.size())).copyTo(subimg1(Rect(roi1.tl()-roiGapped.tl(),roi1.size())));
    img2(Rect(roi2.tl()-tl2,roi2.size())).copyTo(subimg2(Rect(roi2.tl()-roiGapped.tl(),roi2.size())));
    mask1(Rect(roi1.tl()-tl1,roi1.size())).copyTo(submask1(Rect(roi1.tl()-roiGapped.tl(),roi1.size())));
    mask2(Rect(roi2.tl()-tl2,roi2.size())).copyTo(submask2(Rect(roi2.tl()-roiGapped.tl(),roi2.size())));
    setGraphWeightsColorC3<T>(subimg1, subimg2, submask1, submask2, graph);
 
    graph.maxFlow();
    setMaskFromGraph(graph, mask1, tl1, mask2, tl2, gap, roi);
}
 
template<class T> void findSeamInPairC4(const Mat& img1, Mat& mask1, const Point& tl1,
                  const Mat& img2, Mat& mask2, const Point& tl2, int type, int gap) {
    Rect roi;
    if( !overlapRoi(tl1, tl2, img1.size(), img2.size(), roi)) return;
    const int w = roi.width+2*gap, h = roi.height+2*gap;
    GCGraph<float> graph(w*h, w*h-w-h);
    Rect roi0(roi.tl() + Point(-gap,-gap), Size(w,h)), roi1, roi2;
    overlapRoi(roi0.tl(), tl1, roi0.size(), img1.size(), roi1 );
    overlapRoi(roi0.tl(), tl2, roi0.size(), img2.size(), roi2 );
 
    Mat subimg1 = Mat::zeros(h,w, type), subimg2 = Mat::zeros(h,w, type);
    img1(Rect(roi1.tl()-tl1,roi1.size())).copyTo(subimg1(Rect(roi1.tl()-roi0.tl(),roi1.size())));
    img2(Rect(roi2.tl()-tl2,roi2.size())).copyTo(subimg2(Rect(roi2.tl()-roi0.tl(),roi2.size())));
    setGraphWeightsColorC4<T>(subimg1, subimg2, graph);
 
    graph.maxFlow();
    setMaskFromGraph(graph,mask1,tl1,mask2,tl2,gap, roi);
}
 
void findSeamInPair(int ww, const Mat& img1, Mat& mask1, const Point& tl1,
                  const Mat& img2, Mat& mask2, const Point& tl2) {

//	Mat img1,img2, mask1, mask2;
//	Point tl1, tl2;
//	mask1 = Mat(img1.size(),CV_8U,255);
//	mask2 = Mat(img2.size(),CV_8U,255);
	switch(img1.type()) {
        case CV_8UC4 : findSeamInPairC4<Vec4b>(img1,mask1,tl1,img2,mask2,tl2, CV_8UC4,  4); break;
        case CV_16SC4: findSeamInPairC4<Vec4s>(img1,mask1,tl1,img2,mask2,tl2, CV_16SC4, 4); break;
        case CV_32FC4: findSeamInPairC4<Vec4f>(img1,mask1,tl1,img2,mask2,tl2, CV_32FC4, 4); break;
        case CV_8UC3 : findSeamInPairC3<Vec3b>(ww,img1,mask1,tl1,img2,mask2,tl2, CV_8UC3,  4);
        case CV_16SC3: findSeamInPairC3<Vec3s>(ww,img1,mask1,tl1,img2,mask2,tl2, CV_16SC3, 4); break;
        case CV_32FC3: findSeamInPairC3<Vec3f>(ww,img1,mask1,tl1,img2,mask2,tl2, CV_32FC3, 4); break;
    }
	Point t11=tl1; if( t11.x<tl2.x) t11.x+=ww; else t11.x-=ww;
	switch(img1.type()) {
        case CV_8UC4 : findSeamInPairC4<Vec4b>(img1,mask1,t11,img2,mask2,tl2, CV_8UC4,  4); break;
        case CV_16SC4: findSeamInPairC4<Vec4s>(img1,mask1,t11,img2,mask2,tl2, CV_16SC4, 4); break;
        case CV_32FC4: findSeamInPairC4<Vec4f>(img1,mask1,t11,img2,mask2,tl2, CV_32FC4, 4); break;
        case CV_8UC3 : findSeamInPairC3<Vec3b>(ww,img1,mask1,t11,img2,mask2,tl2, CV_8UC3,  4);
        case CV_16SC3: findSeamInPairC3<Vec3s>(ww,img1,mask1,t11,img2,mask2,tl2, CV_16SC3, 4); break;
        case CV_32FC3: findSeamInPairC3<Vec3f>(ww,img1,mask1,t11,img2,mask2,tl2, CV_32FC3, 4); break;
    }
}
 
