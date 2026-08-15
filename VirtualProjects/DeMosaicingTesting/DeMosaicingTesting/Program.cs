// See https://aka.ms/new-console-template for more information

using OpenCvSharp;

namespace DemosaicAPI
{
    static class PixelGrouping
    {
        static int nChange;
        static int eChange;
        static int wChange;
        static int sChange;
        public static int Hue_Transit(int g1, int g2, int g3, int v1, int v3)
        {
            if((g1 < g2 && g2 < g3) || (g1 > g2 && g2 > g3)) 
                return v1 + ((v3 - v1) * (g2 - g1) / (g3 - g1));
            
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
            nChange = Math.Abs(north - current) * 2 + Math.Abs(gNorth - gSouth);
            eChange = Math.Abs(east - current) * 2 + Math.Abs(gEast - gWest);
            wChange = Math.Abs(west - current) * 2 + Math.Abs(gEast - gWest);
            sChange = Math.Abs(south - current) * 2 + Math.Abs(gNorth - gSouth);

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
            eChange = Math.Abs(altNE - altSW) + Math.Abs(NE - current) 
                    + Math.Abs(current - SW) + Math.Abs(gNE - gCurrent)
                    + Math.Abs(gCurrent - gSW);
            wChange = Math.Abs(altNW - altSE) + Math.Abs(NW - current) 
                    + Math.Abs(current - SE) + Math.Abs(gNW - gCurrent)
                    + Math.Abs(gCurrent - gSE);
            
            if(IsSmallest(eChange, wChange, eChange + 1, eChange + 1))
                return Hue_Transit(gNE, gCurrent, gSW, altNE, altSW);
            else
                return Hue_Transit(gNW, gCurrent, gSE, altNW, altSE);
        }
    }

    class Program
    {
        static Mat mat = new Mat("/Users/arianadadgostar/Documents/VEX_AI_Arena_3ms_Exposure_0dB_Gain.bmp", ImreadModes.Color);

        static void Main(string[] args)
        {
            //Vec3b pixel = mat.At<Vec3b>(i, j);
            int index = 0;

            Vec3b[] RGBVals;
            mat.GetArray(out RGBVals);

            for(int i = 1; i < mat.Rows; i += 2 )
            {
                for(int j = index; j < mat.Cols; j += 2) // Applies masks
                {
                    RGBVals[i * mat.Cols + j] = (Vec3b)PixelGrouping.Hue_Transit(RGBVals[i * mat.Cols + j - 3],
                                                                                RGBVals[i * mat.Cols + j],
                                                                                RGBVals[i * mat.Cols + j + 3], 
                                                                                RGBVals[i * mat.Cols + j - 2], 
                                                                                RGBVals[i * mat.Cols + j + 4]);
                }
                index = (index == 2) ? 1 : 2;
            }

            Cv2.ImShow("My Image", mat);
            Cv2.WaitKey(0); 
            Cv2.DestroyAllWindows();
        }
    }
}