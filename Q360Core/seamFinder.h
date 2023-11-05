//
//  SeamFind.h
//  PhotoTrans
//
//  Created by Hyun Joon Shin on 11/1/14.
//  Copyright (c) 2014 Hyun Joon Shin. All rights reserved.
//
 
#ifndef __PhotoTrans__SeamFind__
#define __PhotoTrans__SeamFind__
 
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include "GCGraph.h"
const float terminal_cost_ =      10000.f;
const float bad_region_penalty_ = 200000.f;
 

void findSeamInPair(int ww, const cv::Mat& img1, cv::Mat& mask1, const cv::Point& tl1,
                  const cv::Mat& img2, cv::Mat& mask2, const cv::Point& tl2);

#endif /* defined(__PhotoTrans__SeamFind__) */