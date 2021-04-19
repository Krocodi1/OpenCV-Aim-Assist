#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc/types_c.h>
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <filesystem>

using namespace cv;

namespace DebugOpenCV
{
    std::string TypeToStr(int type);
}

namespace Color
{
    static const Scalar Red{ 0, 0, 255 };
    static const Scalar Green{ 0, 255, 0 };
    static const Scalar Blue{ 255, 0, 0 };
    static const Scalar White{ 255, 255, 255 };
    static const Scalar Black{ 0, 0, 0 };
}

namespace HaarUtils // This will help you to create your own cascade 
{
    void SaveScreenshotManualy(const Mat& mat, std::string& pos_name, std::string& neg_name, int& i_pos, int& i_neg);
    void GenerateNegativeDescriptionFile();
}

namespace Math
{
    Point FindClosest(const Point& screen_center, const std::vector<Point>& target_center);
}

namespace MouseEvents
{
    void MouseSetup(INPUT* mouse_buffer, const RECT& screeen);
    void MouseMove(INPUT* mouse_buffer, const Point& target);
}

namespace Aimbot
{
    void AimAt(INPUT* mouse_buffer, const Point& screen_center, const Point& target, const float speed);
}