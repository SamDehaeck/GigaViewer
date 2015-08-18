#include "hdf5sourcesink.h"
#include "opencv2/opencv.hpp"
#include <QInputDialog>

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

        // now open the selected one
        dataset=hFile->openDataSet(finalDataset);
        dataspace = dataset.getSpace();

        int ndims = dataspace.getSimpleExtentDims( dims, NULL);
        if (ndims!=3) qDebug()<<"Rank is not 3: "<<ndims;
        /*qDebug() << "rank " << ndims << ", dimensions " <<
          (unsigned long)(dims[0]) << " x " <<
          (unsigned long)(dims[1]) << " x " <<
          (unsigned long)(dims[2]);*/

        dataclass = dataset.getTypeClass();
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
        } else if (dataclass==H5T_COMPOUND) { //typically a complex number => no meaningfull way to show this so exit
            qDebug()<<"Data set has compound type - will exit";
            return false;
        } else {
            qDebug()<<"Data set has unknown type - will exit";
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

bool Hdf5SourceSink::StartRecording(QString recFold, QString codec, int, int cols, int rows)
{
    QDateTime mom = QDateTime::currentDateTime();
    QString filenam=recFold+"/"+mom.toString("yyyyMMdd-hhmmss")+".h5";
    std::string datasetname="data";
    std::string timename="time";

//    qDebug()<<"Init recording: "<<filenam<<" with codec "<<codec;

    hFile=new H5File(filenam.toStdString(),H5F_ACC_TRUNC);
    if (codec=="HDF8") {
        readType=PredType::NATIVE_UCHAR;
        dataformat="MONO8";
    } else if (codec=="HDF12") {
        qDebug()<<"This format not yet implemented: "<<codec;
        dataformat="MONO12";
    } else if (codec=="HDF14") {
        qDebug()<<"This format not yet implemented: "<<codec;
        dataformat="MONO14";
    } else if (codec=="HDFBAYERRG8") {
        qDebug()<<"This format not yet implemented: "<<codec;
        dataformat="BAYERRG8";
    } else if (codec=="HDFRGB8") {
        readType=PredType::NATIVE_UCHAR;
        dataformat="RGB8";
        qDebug()<<"This format not yet fully implemented, will convert to gray: "<<codec;
    } else {
        qDebug()<<"This format not yet implemented: "<<codec;
    }


    /*
     * Create the data space with unlimited size in the first dimension.
     */
    hsize_t      dims[3]  = { 1,(uint)rows, (uint)cols};  // dataset dimensions at creation
    recrows=rows;
    reccols=cols;
    hsize_t      maxdims[3] = {H5S_UNLIMITED, (uint)rows,(uint)cols};
    DataSpace mspace1( 3, dims, maxdims);
    /*
     * Modify dataset creation properties, i.e. enable chunking.
     */

    hsize_t      chunk_dims[3] ={1,(uint)rows, (uint)cols};
    cparms.setChunk( 3, chunk_dims );
    /*
     * Set fill value for the dataset
     */
    int fill_val = 0;
    cparms.setFillValue( readType, &fill_val);
//  investigate if compression is usefull.
//    cparms.setDeflate(5);
    /*
     * Create a new dataset within the file using cparms
     * creation properties.
     */
    dataset = hFile->createDataSet( datasetname, readType, mspace1, cparms);

    index=0;
    return true;
}

bool Hdf5SourceSink::RecordFrame(ImagePacket &source)
{
    if (source.pixFormat=="RGB8") { // too much changes to make it work in colour for webcams=> just convert to gray.
        cv::Mat gray(source.image.rows,source.image.cols,CV_8U);
        cv::cvtColor(source.image,gray,CV_RGB2GRAY);
        source.image=gray;
    }
    if (index==0) { // no need for slab selection or anything
        dataset.write(source.image.data,readType);
    } else {
        hsize_t newdims[3]  = {index+1,recrows, reccols};
        dataset.extend(newdims);
        DataSpace filespace=dataset.getSpace();
        hsize_t offset[3];
        offset[0]=index;
        offset[1]=0;
        offset[2]=0;
        hsize_t dimsIm[3];
        dimsIm[0]=1;
        dimsIm[1]=recrows;
        dimsIm[2]=reccols;
        filespace.selectHyperslab(H5S_SELECT_SET,dimsIm,offset);

        hsize_t dumbIm[2];
        dumbIm[0]=recrows;
        dumbIm[1]=reccols;
        DataSpace memspac(2,dumbIm,NULL);

        dataset.write(source.image.data,readType,memspac,filespace);

    }
    index++;
    return false;
}

bool Hdf5SourceSink::StopRecording()
{
    cparms.close();
    hFile->close();
    delete hFile;
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
