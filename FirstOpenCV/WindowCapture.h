#pragma once

#include "Helpers.h"

using namespace cv;

class WindowCapture
{
public:
    HWND m_hwnd{};
    RECT m_desktop{};
    int m_desctop_width{};
    int m_desctop_height{};
    int m_capture_width{};
    int m_capture_height{};
    int m_capture_x{};
    int m_capture_y{};
    HDC m_hwnd_dc{};
    HDC m_hwnd_compatible_dc{};
    HBITMAP m_hb_window{};
    BITMAPINFOHEADER  m_b_info{};

public:
    WindowCapture();
    ~WindowCapture();

    void CaptureInit();
    void ConvertToMat(Mat& mat);
};
