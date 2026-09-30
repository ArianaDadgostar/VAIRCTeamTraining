#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main() {
    
    Mat right = imread("/Users/arianadadgostar/Downloads/TopRight(1).png");
    Mat left = imread("'/Users/arianadadgostar/Downloads/TopLeft(1).png'");
    Mat images[] = {left, right};
    Mat original = imread("/Users/arianadadgostar/Downloads/opencv_stitch_left.jpg");
    Mat final = Mat(original.rows, original.cols, CV_64FC3);

    OutputArray output = final;

    Stitcher stitcher = Stitcher::create(Stitcher::Mode::SCANS);
    Stitcher::Status status = stitcher.stitch(images, output);
    final = output.GetMat();
    if(status != Stitcher.Status.OK)
    {
        Console.WriteLine(status);
    }

    Cv2.ImShow("My image", final);
    Cv2.WaitKey(0);
    Cv2.DestroyAllWindows();
}