#include "Helpers.h"

std::string DebugOpenCV::TypeToStr(int type) // https://stackoverflow.com/questions/10167534/how-to-find-out-what-type-of-a-mat-object-is-with-mattype-in-opencv
{
    std::string str;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch (depth)
    {
    case CV_8U:  str = "8U";   break;
    case CV_8S:  str = "8S";   break;
    case CV_16U: str = "16U";  break;
    case CV_16S: str = "16S";  break;
    case CV_32S: str = "32S";  break;
    case CV_32F: str = "32F";  break;
    case CV_64F: str = "64F";  break;
    default:     str = "User"; break;
    }

    str += "C";
    str += (chans + '0');

    return str;
}

void HaarUtils::SaveScreenshotManualy(const Mat& mat, std::string& pos_name, std::string& neg_name, int& i_pos, int& i_neg)
{
    if (GetAsyncKeyState(VK_NUMPAD4) & 1) // Save screenshot to Positive folder
    {
        pos_name = "Positive\\positive" + std::to_string(i_pos++) + ".jpg";
        imwrite(pos_name, mat);
        std::cout << pos_name << " saved" << std::endl;
    }

    if (GetAsyncKeyState(VK_NUMPAD5) & 1) // Save screenshot to Negative folder
    {
        neg_name = "Negative\\negative" + std::to_string(i_neg++) + ".jpg";
        imwrite(neg_name, mat);
        std::cout << neg_name << " saved" << std::endl;
    }
}

void HaarUtils::GenerateNegativeDescriptionFile()
{
    std::ofstream fstream("Resources\\negativeA.txt"); // txt file will be created automaticaly if it doesn't exist
    if (!fstream)
    {
        std::cout << "[!] ofstream error" << std::endl;
        return;
    }

    // Loop throught all files in Negative folder and add path for each file to negative.txt
    for (const auto& entry : std::filesystem::directory_iterator("Negative"))
        fstream << entry.path() << "\n";

    fstream.close();
}

Point Math::FindClosest(const Point& capture_center, const std::vector<Point>& target_center)
{
    Point closest{};
    int smallest_dist{}; 
    int cur_dist{};

    for (int i{}; i < target_center.size(); i++)
    {
        cur_dist = ((capture_center.x - target_center[i].x) * (capture_center.x - target_center[i].x) +
            (capture_center.y - target_center[i].y) * (capture_center.y - target_center[i].y));

        if(smallest_dist == 0 || smallest_dist > cur_dist)
        {
            smallest_dist = cur_dist;
            closest = target_center[i];
        }
    }

    return closest;
}

void MouseEvents::MouseSetup(INPUT* mouse_buffer, const RECT& screen)
{
    mouse_buffer->type = INPUT_MOUSE;
    mouse_buffer->mi.dx = 0;
    mouse_buffer->mi.dy = 0;
    mouse_buffer->mi.mouseData = 0;
    mouse_buffer->mi.dwFlags = MOUSEEVENTF_MOVE;
    mouse_buffer->mi.time = 0;
    mouse_buffer->mi.dwExtraInfo = 0;
}

void MouseEvents::MouseMove(INPUT* mouse_buffer, const Point& target)
{
    mouse_buffer->mi.dx = target.x;
    mouse_buffer->mi.dy = target.y;

    SendInput(1, mouse_buffer, sizeof(INPUT));
}

void Aimbot::AimAt(INPUT* mouse_buffer, const Point& screen_center, const Point& target, const float speed)
{
    Point move{};

    if (screen_center.x > target.x)
    {
        move.x = target.x - screen_center.x;
        move.x /= speed;
    }
    else
    {
        move.x = std::abs(screen_center.x - target.x);
        move.x /= speed;
    }

    if (screen_center.y > target.y)
    {
        move.y = target.y - screen_center.y;
        move.y /= speed;
    }
    else
    {
        move.y = std::abs(screen_center.y - target.y);
        move.y /= speed;
    }

    MouseEvents::MouseMove(mouse_buffer, move);
}
