#ifndef HDF5SOURCESINK_H
#define HDF5SOURCESINK_H

#include "imagepacket.h"
#include "imagesourcesink.h"
#include <H5Cpp.h>


class Hdf5SourceSink : public ImageSourceSink  //#include <hdf5/serial/H5Cpp.h>
{
public:
    bool Init();
    bool StartAcquisition(QString dev="0");
    bool StopAcquisition();
    bool ReleaseCamera();
    bool GrabFrame(ImagePacket& target,int indexIncrement=1);
    bool RecordFrame(ImagePacket& source);
    bool StartRecording(QString recFold, QString codec, int fps, int cols, int rows);
    bool StopRecording();
    bool IsOpened();
    bool SkipFrames(bool forward);

private:
    H5::H5File *hFile;
    H5::DataSpace dataspace;
    H5::DataSet dataset;
    H5T_class_t dataclass;
    H5::DataSpace* memspace;
    hsize_t dims[3];
    hsize_t recrows,reccols;
    H5::DataType readType;
    H5::DSetCreatPropList cparms;

    unsigned long index;

    cv::Mat frame;
    QString dataformat;
    std::vector<double> timestamps;
    bool timepresent;

};

#endif // HDF5BACKEND_H
