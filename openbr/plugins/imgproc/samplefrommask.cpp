#include <openbr/plugins/openbr_internal.h>

using namespace cv;

namespace br
{

/*!
 * \ingroup transforms
 * \brief Samples pixels from a mask.
 * \author Scott Klum \cite sklum
 */
class SampleFromMaskTransform : public UntrainableTransform
{
    Q_OBJECT

    void project(const Template &src, Template &dst) const
    {
        Mat mask = src.file.get<Mat>("Mask");
        Mat indices;
        findNonZero(mask,indices);

        if (indices.total() > 0) {
            dst.m() = Mat(1,indices.total(),src.m().type());

            Mat masked;
            src.m().copyTo(masked, mask);

            for (size_t j=0; j<indices.total(); j++)
                dst.m().at<uchar>(0,j) = masked.at<uchar>(indices.at<Point>(j).y,indices.at<Point>(j).x);
        } else {
            dst.file.fte = true;
            qWarning("No mask content for %s.",qPrintable(src.file.baseName()));
        }
    }
};

BR_REGISTER(Transform, SampleFromMaskTransform)

} // namespace br

#include "imgproc/samplefrommask.moc"
