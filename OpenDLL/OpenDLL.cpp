#include "pch.h" 
#include <utility>
#include <limits.h>
#include "OpenDLL.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/mcc.hpp>

using namespace std;
using namespace cv;
using namespace mcc;
using namespace ccm;

vector<Ptr<mcc::CChecker>> checkers;

void ImageChart(string ImageLink, bool show) {
	Mat ChartImage = imread(ImageLink);
     
    Ptr<CCheckerDetector> detector = cv::mcc::CCheckerDetector::create();
    if (!detector->process(ChartImage, MCC24, 1)) {
        printf("ChartColor not detected \n");
    }        
    
    checkers = detector->getListColorChecker();
    for (Ptr<mcc::CChecker> checker : checkers) {
        Ptr<CCheckerDraw> cdraw = CCheckerDraw::create(checker);
        cdraw->draw(ChartImage);
        if (show == 1) {
            Mat show_image;
            resize(ChartImage, show_image, Size(700, 700), INTER_LINEAR);
            imshow("CCM", show_image);
            waitKey(0);
        }
    }
}

void PrintCCM() {
    for (Ptr<mcc::CChecker> checker : checkers) {
        Mat chartsRGB = checker->getChartsRGB();
        Mat src = chartsRGB.col(1).clone().reshape(3, chartsRGB.rows / 3);
        src /= 255.0;
        ColorCorrectionModel model1(src, COLORCHECKER_Macbeth);
        model1.run();
        Mat ccm = model1.getCCM();
        std::cout << ccm << endl;
    }
}

void CCM(string ImageLink, string SaveLink, bool show) {
    for (Ptr<mcc::CChecker> checker : checkers) {
        Mat chartsRGB = checker->getChartsRGB();
        Mat src = chartsRGB.col(1).clone().reshape(3, chartsRGB.rows / 3);
        src /= 255.0;
        ColorCorrectionModel model1(src, COLORCHECKER_Macbeth);
        model1.run();

        Mat cor_image;
        cvtColor(imread(ImageLink), cor_image, COLOR_BGR2RGB);
        cor_image.convertTo(cor_image, CV_64F);
        const int inp_size = 255;
        const int out_size = 255;
        cor_image = cor_image / inp_size;
        Mat calibrated_image = model1.infer(cor_image);
        Mat out_ = calibrated_image * out_size;
        out_.convertTo(out_, CV_8UC3);
        Mat out_image = min(max(out_, 0), out_size);
        Mat final_image;
        cvtColor(out_image, final_image, COLOR_RGB2BGR);

        if (show == 1) {
            Mat show_image;
            resize(out_image, show_image, Size(700, 700), INTER_LINEAR);
            imshow("CCM", show_image);
            waitKey(0);
        }

        imwrite("Result.jpg", final_image);
    }
}