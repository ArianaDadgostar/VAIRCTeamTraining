#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class PixelGrouping
{
    public:
        static inline int nChange;
        static inline int eChange;
        static inline int wChange;
        static inline int sChange;
        static int Hue_Transit(int g1, int g2, int g3, int v1, int v3)
        {
            if((g1 < g2 && g2 < g3) || (g1 > g2 && g2 > g3)) 
                return v1 + (v3 - v1) * (g2 - g1) / (g3 - g1);
            
            return (v1 + v3) / 2 + (g2 * 2 - g1 - g3) / 4;
        }

        static bool IsSmallest(int chosen, int val1, int val2, int val3)
        {
            if(chosen > val1 || chosen > val2 || chosen > val3) return false;

            return true;
        }

        static int Calculate_Green(int north, int east, int west, int south, int current,
                                    int gNorth, int gEast, int gWest, int gSouth)
        {
            nChange = std::abs(north - current) * 2 + std::abs(gNorth - gSouth);
            eChange = std::abs(east - current) * 2 + std::abs(gEast - gWest);
            wChange = std::abs(west - current) * 2 + std::abs(gEast - gWest);
            sChange = std::abs(south - current) * 2 + std::abs(gNorth - gSouth);

            if(IsSmallest(nChange, eChange, wChange, sChange)) 
                return (gNorth * 3 + gSouth + current - north) / 4;
            if(IsSmallest(eChange, nChange, wChange, sChange))
                return (gEast * 3 + gWest + current - east) / 4;
            if(IsSmallest(wChange, nChange, eChange, sChange))
                return (gWest * 3 + gEast + current - west) / 4;
            else 
                return (gSouth * 3 + gNorth + current - south) / 4;
        }

        static int Calculate_Diagonal(int current, int NE, int SE, int NW, int SW, 
                                        int altNE, int altSE, int altNW, int altSW,
                                        int gCurrent, int gNE, int gSE, int gNW, int gSW)
        {
            eChange = std::abs(altNE - altSW) + std::abs(NE - current) 
                    + std::abs(current - SW) + std::abs(gNE - gCurrent)
                    + std::abs(gCurrent - gSW);
            wChange = std::abs(altNW - altSE) + std::abs(NW - current) 
                    + std::abs(current - SE) + std::abs(gNW - gCurrent)
                    + std::abs(gCurrent - gSE);
            
            if(IsSmallest(eChange, wChange, eChange + 1, eChange + 1))
                return Hue_Transit(gNE, gCurrent, gSW, altNE, altSW);
            else
                return Hue_Transit(gNW, gCurrent, gSE, altNW, altSE);
        }
};

int main() {

    Mat mat = imread("/Users/arianadadgostar/Documents/VEX_AI_Arena_3ms_Exposure_0dB_Gain.bmp", IMREAD_COLOR);
    Mat gray = imread("/Users/arianadadgostar/Documents/VEX_AI_Arena_3ms_Exposure_0dB_Gain.bmp", IMREAD_GRAYSCALE);
    Mat openCV;
    std::clock_t start = std::clock();

    cv::cvtColor(gray, openCV, cv::COLOR_BayerBG2BGR);

    std::clock_t end = std::clock();

    printf("\n OpenCV: %ld", end - start);
    imshow("My Image", openCV);
    waitKey(0);

    start = std::clock();

    int index = 0;
    int current = 0;
    int value = 0;
    int altValue = 0;
    std::vector<Vec3b> RGBVals(mat.total());
    std::memcpy(&RGBVals[0], &mat.data[0], RGBVals.size() * sizeof(Vec3b));

    index = 2;
    for(int i = 2; i < mat.rows - 2; i += 1 )
    {
        for(int j = index; j < mat.cols - 1; j += 2)
        {
            current = i * mat.cols + j;
            value = (index == 0) ? 0 : 2;
            RGBVals[current][1] = PixelGrouping::Calculate_Green
                (RGBVals[current - 2 * mat.cols][value], // NORTH
                RGBVals[current + 2][value], // EAST
                RGBVals[current - 2][value], // WEST
                RGBVals[current + 2 * mat.cols][value], // SOUTH
                RGBVals[current][value],
                RGBVals[current - mat.cols][1], // GREEN NORTH
                RGBVals[current + 1][1], // GREEN EAST
                RGBVals[current - 1][1], // GREEN WEST
                RGBVals[current + mat.cols][1]); // GREEN SOUTH
        }
        index = (index == 1) ? 0 : 1;
    }
    
    index = 2;
    for(int i = 1; i < mat.rows - 1; i += 1 )
    {
        for(int j = index; j < mat.cols - 1; j += 2)
        {
            current = i * mat.cols + j;

            RGBVals[current][0] = PixelGrouping::Hue_Transit
                (RGBVals[current - mat.cols][1], // GREEN --> RED
                RGBVals[current][1],
                RGBVals[current + mat.cols][1],
                RGBVals[current - mat.cols][0],
                RGBVals[current + mat.cols][0]);

            RGBVals[current][2] = PixelGrouping::Hue_Transit
                (RGBVals[current - 1][1], // GREEN --> BLUE
                RGBVals[current][1],
                RGBVals[current + 1][1], 
                RGBVals[current - 1][2], 
                RGBVals[current + 1][2]);
        }
        index = (index == 2) ? 1 : 2;
    }

    index = 2;
    for(int i = 2; i < mat.rows - 2; i += 1)
    {
        for(int j = index; j < mat.cols - 2; j += 2)
        {
            current = i * mat.cols + j;
            value = (index == 2) ? 0 : 2;
            altValue = (value == 0) ? 2 : 0;
            RGBVals[current][value] = PixelGrouping::Calculate_Diagonal
                (RGBVals[current][value],
                RGBVals[current - mat.cols + 2][value], // NE
                RGBVals[current + mat.cols + 2][value], // SE
                RGBVals[current - mat.cols - 2][value], // NW
                RGBVals[current + mat.cols - 2][value], // SW
                RGBVals[current - mat.cols + 1][altValue], // altNE
                RGBVals[current + mat.cols + 1][altValue], // altSE
                RGBVals[current - mat.cols - 1][altValue], // altNW
                RGBVals[current + mat.cols - 1][altValue], // altSW
                RGBVals[current][1], // GREEN CURRENT
                RGBVals[current - mat.cols + 1][1], // GREEN NE
                RGBVals[current + mat.cols + 1][1], // GREEN SE
                RGBVals[current - mat.cols - 1][1], // GREEN NW
                RGBVals[current + mat.cols - 1][1]); // GREEN SW
        }
        index = (index == 2) ? 1 : 2;
    }

    std::memcpy(mat.data, &RGBVals[0], RGBVals.size() * sizeof(Vec3b));
    if (mat.empty()) {
        cout << "Image File "
             << "Not Found" << endl;

        // wait for any key press
        cin.get();
        return -1;
    }
    end = std::clock();
    printf("\n Pixel Grouping: %ld", end - start);
    imshow("My Image", mat);
    waitKey(0);
    return 0;
}