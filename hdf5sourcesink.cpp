#include "hdf5sourcesink.h"
#include "opencv2/opencv.hpp"
#include <hdf5/serial/H5Cpp.h>

using namespace H5;

bool Hdf5SourceSink::Init()
{
    return true;
}

bool Hdf5SourceSink::StartAcquisition(QString dev)
{
    H5std_string FILE_NAME=dev.toStdString();
//    H5std_string DATASET_NAME( "data" );
    try {
        hFile = new H5File( FILE_NAME, H5F_ACC_RDONLY );

        // first find the name of the dataset. This opens all of them to see which ones have 3 dimensions
        // if there are multiple datasets. Give the option to the user which one to choose.
        hsize_t amDataSets= hFile->getNumObjs();
//        qDebug()<<"Amount datasets is: "<<amDataSets;
        std::vector<std::string> goodSets;
        QStringList items;  // only used if more than one valid dataset present
        for (uint i=0;i<amDataSets;i++) {
            std::string theName=hFile->getObjnameByIdx(i);
//            qDebug()<<"Dataset: "<<QString::fromStdString(theName);
            DataSet dumbset=hFile->openDataSet(theName);
            DataSpace dumbspace=dumbset.getSpace();
            int dumbrank = dumbspace.getSimpleExtentNdims();
            if (dumbrank==3) {  // only requirement is that they have 3 dimensions
                goodSets.push_back(theName);
                items<<QString::fromStdString(theName);
            }
        }

        std::string finalDataset;
        if (goodSets.size()==0) {
            qDebug()<<"No valid 3D dataset found";
            return false;
        } else if (goodSets.size()==1) {
            finalDataset=goodSets[0];
        } else {
            bool ok;
            QString item = QInputDialog::getItem(NULL, "Which dataset to open",
                                                "Options:", items, 0, false, &ok);
            if (ok && !item.isEmpty()) {
                finalDataset=item.toStdString();
//                qDebug()<<"Selected "<<item;
            } else {
                return false;
            }

        }

        dataset=hFile->openDataSet(finalDataset);
        dataclass = dataset.getTypeClass();


        /*
        * Get dataspace of the dataset.
        */
        dataspace = dataset.getSpace();
//        int rank = dataspace.getSimpleExtentNdims();
        int ndims = dataspace.getSimpleExtentDims( dims, NULL);
        if (ndims!=3) qDebug()<<"Rank is not 3: "<<ndims;

        /*qDebug() << "rank " << ndims << ", dimensions " <<
          (unsigned long)(dims[0]) << " x " <<
          (unsigned long)(dims[1]) << " x " <<
          (unsigned long)(dims[2]);*/

        if( dataclass == H5T_INTEGER ) {
             IntType intype = dataset.getIntType();
             size_t size = intype.getSize();
             if (size==1) {
                readType=PredType::NATIVE_UCHAR;
                frame=cv::Mat(dims[1],dims[2],CV_8U);
             } else {
                 qDebug()<<"Integer size not yet handled: "<<size;
                 return false;
             }
        } else if (dataclass== H5T_FLOAT){
            readType=PredType::NATIVE_FLOAT;
            frame=cv::Mat(dims[1],dims[2],CV_32F);
        } else if (dataclass==H5T_COMPOUND) {
            qDebug()<<"Data set has compound type - will exit";
            return false;
        } else {
            qDebug()<<"Data set has different type - will exit";
            return false;
        }

        index=0;

    } catch (Exception e) {
        qDebug()<<"Error occured: "<<e.getCDetailMsg();
        return false;
    }

    return true;
}

bool Hdf5SourceSink::StopAcquisition()
{
    try {
        hFile->close();
        delete hFile;
    } catch (Exception e) {
        qDebug()<<"Error occured: "<<e.getCDetailMsg();
        return false;
    }

    return true;
}

bool Hdf5SourceSink::ReleaseCamera()
{
    return true;
}

bool Hdf5SourceSink::GrabFrame(ImagePacket &target, int indexIncrement)
{
    if ((index+indexIncrement >= dims[0]-1)||((int)index+indexIncrement <0)) {
        return true;
    }
    try {
        hsize_t dimsSlab[3]={1,dims[1],dims[2]};
        hsize_t offset[3]={index,0,0};
        dataspace.selectHyperslab(H5S_SELECT_SET, dimsSlab, offset);

        hsize_t readDims[2]={dims[1],dims[2]};
        DataSpace memspace(2,readDims);

        dataset.read(frame.data,readType,memspace,dataspace);

        dataspace.selectNone();

        double min,max;
        cv::minMaxLoc(frame,&min,&max);
        double stretch=255.0/(max-min);
        double shift=-min*stretch;
        frame.convertTo(target.image,CV_8U,stretch,shift);

        target.pixFormat="MONO8";
        target.seqNumber=index;
        index=index+indexIncrement;
        return true;
    } catch (Exception e) {
        qDebug()<<"Error occured: "<<e.getCDetailMsg();
        return false;
    }

}

bool Hdf5SourceSink::RecordFrame(ImagePacket &source)
{
    return false;
}

bool Hdf5SourceSink::StartRecording(QString recFold, QString codec, int, int cols, int rows)
{
    return true;
}

bool Hdf5SourceSink::StopRecording()
{
    return true;
}

bool Hdf5SourceSink::IsOpened()
{
    return true;
}

bool Hdf5SourceSink::SkipFrames(bool forward) {
    int skipping = 0;
    if (forward) {
        skipping=dims[0]/10;
    } else {
        skipping=-dims[0]/50;
    }
//    qDebug()<<"Will try to skip "<<skipping<<" frames";

    if ((index+skipping >= dims[0]-1)||((int)index+skipping <0)) {
        return true;
    }

    index+=skipping;
    return true;
}
