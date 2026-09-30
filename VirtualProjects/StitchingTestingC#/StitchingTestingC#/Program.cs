// See https://aka.ms/new-console-template for more information
using OpenCvSharp;

namespace StitchingTest
{
    class Program
    {
        static Mat topRight = new Mat("/Users/arianadadgostar/Downloads/TopRight(1).png", ImreadModes.Color);
        static Mat topLeft = new Mat("'/Users/arianadadgostar/Downloads/TopLeft(1).png'", ImreadModes.Color);

        static int HorizontalOverlap(Vec3b[] rightPixels, Vec3b[] leftPixels)
        {
            for(int i = 0; i < right.Cols; i++)
            {
                if(rightPixels[i] != leftPixels[i]) continue;

                return i;
            }
            return -1;
        }

        static void CalculateWeights(InputArray rightWeights, InputArray leftWeights, int overlap)
        {
            Mat tempRight = rightWeights.GetMat();
            Mat tempLeft = leftWeights.GetMat();
            for(int i = 0; i < rightWeights.Cols(); i++)
            {
                tempRight.At<byte>(0, i) = (i > overlap) ? (byte)1 : (byte)0;
                tempLeft.At<byte>(0, i) = (i < overlap) ? (byte)1 : (byte)0;
            }
        }

        static void Main()
        {
            // Vec3b[] rightPixels;
            // right.GetArray(out rightPixels);

            // Vec3b[] leftPixels;
            // left.GetArray(out leftPixels);

            // InputArray rightWeights = right;
            // InputArray leftWeights = left;

            // int overlap = HorizontalOverlap(rightPixels, leftPixels);
            // Mat final = new Mat(right.Rows, right.Cols - overlap + left.Cols, MatType.CV_8UC3);

            // CalculateWeights(rightWeights, leftWeights, overlap);

            // Cv2.BlendLinear(left, right, rightWeights, leftWeights, final);

            Mat[] images = {left, right};
            Mat original = new Mat("/Users/arianadadgostar/Downloads/opencv_stitch_left.jpg");
            Mat final = new Mat(original.Rows, original.Cols, MatType.CV_64FC3);

            OutputArray output = final;

            Stitcher stitcher = Stitcher.Create(Stitcher.Mode.Scans);
            Stitcher.Status status = stitcher.Stitch(images, output);
            final = output.GetMat();
            if(status != Stitcher.Status.OK)
            {
                Console.WriteLine(status);
            }

            Cv2.ImShow("My image", final);
            Cv2.WaitKey(0);
            Cv2.DestroyAllWindows();
        }
    }
}