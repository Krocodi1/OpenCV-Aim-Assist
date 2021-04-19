#include "WindowCapture.h"

WindowCapture::WindowCapture()
{
    m_hwnd = GetDesktopWindow();
    GetWindowRect(m_hwnd, &m_desktop);
    m_desctop_width = m_desktop.right;
    m_desctop_height = m_desktop.bottom;

    // Area where the program will be searching for needed objects
    // The larger the zone, the smaller the fps will be
    m_capture_width = 300; 
    m_capture_height = 300;

    // Where the area starts (in the middle)
    m_capture_x = m_desctop_width / 2 - m_capture_width / 2;
    m_capture_y = m_desctop_height / 2 - m_capture_height / 2;
}

WindowCapture::~WindowCapture()
{
    DeleteObject(m_hb_window);
    DeleteDC(m_hwnd_compatible_dc);
    ReleaseDC(m_hwnd, m_hwnd_dc);
}

void WindowCapture::CaptureInit() // https://stackoverflow.com/questions/34466993/opencv-desktop-capture
{
    m_hwnd_dc = GetDC(m_hwnd);
    m_hwnd_compatible_dc = CreateCompatibleDC(m_hwnd_dc);
    SetStretchBltMode(m_hwnd_compatible_dc, COLORONCOLOR);

    m_hb_window = CreateCompatibleBitmap(m_hwnd_dc, m_capture_width, m_capture_height);
    m_b_info.biSize = sizeof(BITMAPINFOHEADER);
    m_b_info.biWidth = m_capture_width;
    m_b_info.biHeight = -m_capture_height;
    m_b_info.biPlanes = 1;
    m_b_info.biBitCount = 32;
    m_b_info.biCompression = BI_RGB;
    m_b_info.biSizeImage = 0;
    m_b_info.biXPelsPerMeter = 0;
    m_b_info.biYPelsPerMeter = 0;
    m_b_info.biClrUsed = 0;
    m_b_info.biClrImportant = 0;

    SelectObject(m_hwnd_compatible_dc, m_hb_window);
}

void WindowCapture::ConvertToMat(Mat& mat)
{
    StretchBlt(m_hwnd_compatible_dc, 0, 0, m_capture_width, m_capture_height, m_hwnd_dc, m_capture_x, 
        m_capture_y, m_capture_width, m_capture_height, SRCCOPY);
    GetDIBits(m_hwnd_compatible_dc, m_hb_window, 0, m_capture_height, mat.data, (BITMAPINFO*)&m_b_info, 
        DIB_RGB_COLORS);
}