#include <opencv2/line_descriptor.hpp>


#include "opencv2/core/utility.hpp"
#include "opencv2/core/private.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

static const char* keys =
{
    "{@image_path | | Image path }"
};


int main( int argc, char** argv )
{
    /* get parameters from comand line */
    CommandLineParser parser( argc, argv, keys );
    String image_path = parser.get<String>( 0 );

    if(image_path.empty())
    {
        // help();
        return -1;
    }

    /* load image */
    cv::Mat imageMat = imread(image_path, 0);
    if(imageMat.data == NULL)
    {
        std::cout << "Error, image could not be loaded. Please, check its path" << std::endl;
    }

    /* create a ramdom binary mask */
//    cv::Mat mask(imageMat.size(), CV_8UC1);
//    cv::randu(mask, Scalar::all(0), Scalar::all(1));
    cv::Mat mask = Mat::ones(imageMat.size(), CV_8UC1);

    /* create a pointer to a BinaryDescriptor object with deafult parameters */
    Ptr<BinaryDescriptor> bd = BinaryDescriptor::createBinaryDescriptor();

    /* create a structure to store extracted lines */
    vector<KeyLine> lines;

    /* extract lines */
    bd->detectKL(imageMat, lines, mask);

    /* draw lines extracted from octave 0 */
    cv::Mat output = imageMat.clone();
    cvtColor(output, output, COLOR_GRAY2BGR);
    for(size_t i = 0; i<lines.size(); i++)
    {
        KeyLine kl = lines[i];
        if(kl.octave == 0)
        {
            /* get a random color */
            int R = (rand() % (int)(255 + 1));
            int G = (rand() % (int)(255 + 1));
            int B = (rand() % (int)(255 + 1));

            /* get extremes of line */
            Point pt1 = Point(kl.startPointX, kl.startPointY);
            Point pt2 = Point(kl.endPointX, kl.endPointY);

            /* draw line */
            line(output, pt1, pt2, Scalar(B,G,R), 5);
            std::cout << "Dati linea" << " " << kl.startPointX << " " <<
                         kl.startPointY << " " << kl.endPointX << " " <<
                         kl.endPointY << " " << kl.lineLength << std::endl;
        }

    }

    /* show lines on image */
    imshow("Lines", output);
    waitKey();
}
