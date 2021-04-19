// Do not forget to compile project in Release, in Debug significantly fewer fps = aim assist will not work correctly 
// Make shure you don't use Display settings -> Scale 100%+ (Set it to 100%)

#include "WindowCapture.h"

using namespace cv;

// Magic numbers are bad, ookay
static constexpr int RECT_THICKNESS = 2;
static constexpr float SCALE_FACTOR = 1.1f;
static constexpr int MIN_NEIGHBORS = 10;
static constexpr int OUT_W = 600;
static constexpr int OUT_H = 600;
static constexpr int AIMBOT_SPEED = 3;

int main()
{
    WindowCapture* d_capture = new WindowCapture();
    Point capture_center{ d_capture->m_capture_width / 2,  d_capture->m_capture_height / 2 };
    Point screen_center{ d_capture->m_desctop_width / 2, d_capture->m_desctop_height / 2 };
    Point closest_to_center{};
    std::vector<Point>target_center{};
    std::vector<Rect> rects{};

    // Set up mouse buffer for aim assist
    INPUT mouse_buffer{};
    MouseEvents::MouseSetup(&mouse_buffer, d_capture->m_desktop);

    // Setup to make screenshot
    d_capture->CaptureInit();

    // Our cascade file goes here
    CascadeClassifier head_cascade{};
    head_cascade.load("Cascade/cascadeN1_11.xml"); // In Cascade folder I have multiple files, 13s are very strict
    if (head_cascade.empty())
    {
        std::cout << "[!] Cascade file not loaded" << std::endl;
        std::cin.get();
        return 1;
    }

    // Create opencv window
    namedWindow("Output", WINDOW_NORMAL);
    resizeWindow("Output", OUT_W, OUT_H);

    // Create Mat for screenshot
    Mat screenshot{};
    screenshot.create(d_capture->m_capture_height, d_capture->m_capture_width, CV_8UC4);

    while (!GetAsyncKeyState(VK_END) & 1)
    {
        rects.clear();
        target_center.clear();

        // Make and convert screenshot to Mat 
        d_capture->ConvertToMat(screenshot);

        // Detect needed objects from the screenshot and save rectangles position 
        head_cascade.detectMultiScale(screenshot, rects, SCALE_FACTOR, MIN_NEIGHBORS);
        
        for (int i{}; i < rects.size(); i++)
        {
            // Draw rectangle for visualization 
            rectangle(screenshot, rects[i].tl(), rects[i].br(), Color::Green, RECT_THICKNESS);

            // Save the position of the center of the rectangle 
            target_center.push_back(Point(rects[i].x + rects[i].width / 2, rects[i].y + rects[i].height / 2));
        }


        // Find the rectangle closest to the center
        closest_to_center = Math::FindClosest(capture_center, target_center);
        if (closest_to_center.x && closest_to_center.y)
        {
            // Draw a cross in the center of the nearest rectangle
            drawMarker(screenshot, closest_to_center, Color::Red, MARKER_CROSS);
            
            // Move our crosshair to the closest target
            Aimbot::AimAt(&mouse_buffer, screen_center, Point(closest_to_center.x + d_capture->m_capture_x,
                closest_to_center.y + d_capture->m_capture_y), AIMBOT_SPEED);
        }

        // Show all stuff in the created open cv window
        imshow("Output", screenshot);

        waitKey(1);
    }

    return 0;
}
