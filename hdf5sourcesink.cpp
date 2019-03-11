#include "hdf5sourcesink.h"
#include "opencv2/opencv.hpp"
#include <QInputDialog>
#include <QMessageBox>

using namespace H5;

bool Hdf5SourceSink::Init()
{
    return true;
}

bool Hdf5SourceSink::StartAcquisition(QString dev)
{
//    H5std_string FILE_NAME=dev.toStdString();
//    H5std_string DATASET_NAME( "data" );
    try {
#ifdef Q_OS_WIN32
        hFile = new H5File( dev.toStdString().c_str(), H5F_ACC_RDONLY );
#else
        hFile = new H5File( dev.toUtf8().data(), H5F_ACC_RDONLY );
#endif

        // first find the name of the dataset. This opens all of them to see which ones have 3 dimensions
        // if there are multiple datasets. Give the option to the user which one to choose.
        hsize_t amDataSets= hFile->getNumObjs();
//        qDebug()<<"Amount datasets is: "<<amDataSets;
        std::vector<std::string> goodSets;
        QStringList items;  // only used if more than one valid dataset present
        timepresent=false;
        std::string timestring;
        for (uint i=0;i<amDataSets;i++) {
            std::string theName=hFile->getObjnameByIdx(i);
            if (QString::fromStdString(theName).contains("time")) {
                timepresent=true;
                timestring=theName;
            }
//            qDebug()<<"Dataset: "<<QString::fromStdString(theName);
            DataSet dumbset=hFile->openDataSet(theName);
            DataSpace dumbspace=dumbset.getSpace();
            int dumbrank = dumbspace.getSimpleExtentNdims();
            if (dumbrank==3) {  // only requirement is that they have 3 dimensions
                goodSets.push_back(theName);
                items<<QString::fromStdString(theName);
            }
            dumbspace.close();
            dumbset.close();
        }

        std::string finalDataset;
        if (goodSets.size()==0) {
            qDebug()<<"No valid 3D dataset found";
            return false;
        } else if (goodSets.size()==1) {
            finalDataset=goodSets[0];
        } else {
            bool ok;
            QString item = QInputDialog::getItem(nullptr, "Which dataset to open",
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

        int ndims = dataspace.getSimpleExtentDims( dims, nullptr);
        if (ndims!=3) qDebug()<<"Rank is not 3: "<<ndims;
        /*qDebug() << "rank " << ndims << ", dimensions " <<
          (unsigned long)(dims[0]) << " x " <<
          (unsigned long)(dims[1]) << " x " <<
          (unsigned long)(dims[2]);*/

        dataclass = dataset.getTypeClass();
        if( dataclass == H5T_INTEGER ) {

            /*
            Attribute myatt_out = dataset.openAttribute("PIXFORMAT");
            myatt_out.read(strdatatype, strreadbuf);

            // Display attribute contents
            qDebug() << "Attribute contents: " << QString::fromStdString(strreadbuf);
            */

            //H5std_string strreadbuf ("");
            // Open attribute and read its contents

            bool hasFormat=dataset.attrExists("PIXFORMAT");
            if (hasFormat) {
               Attribute attr_pix=dataset.openAttribute("PIXFORMAT");
               std::string strbuf;
               StrType strdatatype(PredType::C_S1, 10);
               attr_pix.read(strdatatype,strbuf);
               dataformat=QString::fromStdString(strbuf);
            }
//            qDebug()<<"Attribute contents: "<<dataformat;


             IntType intype = dataset.getIntType();
             size_t size = intype.getSize();
             if (size==1) {
                 if (dataformat=="RGB8") {
                     readType=PredType::NATIVE_UCHAR;
                     frame=cv::Mat(static_cast<int>(dims[1]),static_cast<int>(round(dims[2]/3.)),CV_8UC3);
//                     dataformat="RGB8";
                 } else {
                    readType=PredType::NATIVE_UCHAR;
                    frame=cv::Mat(static_cast<int>(dims[1]),static_cast<int>(dims[2]),CV_8U);
                    if (!hasFormat) {
                        dataformat="MONO8";
                    }
                 }
             } else if (size==2) {
                 readType=PredType::NATIVE_UINT16;
                 frame=cv::Mat(static_cast<int>(dims[1]),static_cast<int>(dims[2]),CV_16U);
                 if (!hasFormat) {
                    dataformat="MONO14"; // not knowing if it is 12 or 14, 14 is the safer choice for displaying will check attribute later.
                 }
             } else {
                 qDebug()<<"Integer size not yet handled: "<<size;
                 return false;
             }



        } else if (dataclass== H5T_FLOAT){
            dataformat="FLOAT";
            readType=PredType::NATIVE_FLOAT;
            frame=cv::Mat(static_cast<int>(dims[1]),static_cast<int>(dims[2]),CV_32F);
        } else if (dataclass==H5T_COMPOUND) { //typically a complex number => no meaningfull way to show this so exit
            qDebug()<<"Data set has compound type - will exit";
            return false;
        } else if (dataclass==4) { // This appears to be the bool used by pytables.
            qDebug()<<"Detected a bool dataset";
            dataformat="BOOL";
            readType=PredType::NATIVE_B8;
            frame=cv::Mat(static_cast<int>(dims[1]),static_cast<int>(dims[2]),CV_8U);
        } else {
            qDebug()<<"Data set has unknown type - will exit"<<dataclass;
            return false;
        }

        // if timestamps present, read them all at once into a matrix

        if (timepresent) {
            timestamps.clear();
            DataSet timeset=hFile->openDataSet(timestring);
            DataSpace timespace = timeset.getSpace();
            hsize_t timedim[1];
            int ndims = timespace.getSimpleExtentDims( timedim, nullptr);
            if (ndims!=1) qDebug()<<"Rank is not 1: "<<ndims;

            cv::Mat timeMat=cv::Mat(1,static_cast<int>(timedim[0]),CV_64F);
            timeset.read(timeMat.data,PredType::NATIVE_DOUBLE);

            for (int i=0;i<static_cast<int>(timedim[0]);i++) {
                timestamps.push_back(timeMat.at<double>(0,i));
            }
        }


        index=0;

    } catch (const Exception& e) {
        QMessageBox::critical(nullptr, "Error", QString::fromStdString(e.getDetailMsg()));
        return false;
    }

    return true;
}

bool Hdf5SourceSink::GrabFrame(ImagePacket &target, int indexIncrement)
{
    if ((static_cast<int>(index)+indexIncrement >= static_cast<int>(dims[0]))||(static_cast<int>(index)+indexIncrement <0)) { // watch out for conversion
        return true;
    }
    try {
        if (dataformat=="RGB8") {
            hsize_t dimsSlab[3]={1,dims[1],dims[2]};
            hsize_t offset[3]={index,0,0};
            dataspace.selectHyperslab(H5S_SELECT_SET, dimsSlab, offset);
        } else {
            hsize_t dimsSlab[3]={1,dims[1],dims[2]};
            hsize_t offset[3]={index,0,0};
            dataspace.selectHyperslab(H5S_SELECT_SET, dimsSlab, offset);
        }

        hsize_t readDims[2]={dims[1],dims[2]};
        DataSpace memspace(2,readDims);

        dataset.read(frame.data,readType,memspace,dataspace);

        dataspace.selectNone();
/*
        double min,max;
        cv::minMaxLoc(frame,&min,&max);
        double stretch=255.0/(max-min);
        double shift=-min*stretch;
        frame.convertTo(target.image,CV_8U,stretch,shift);
*/
        target.image=frame.clone();

        target.pixFormat=dataformat;
        target.seqNumber=static_cast<int>(index); // be carefull of overflows!!
        if (timepresent) {
            target.timeStamp=timestamps[index];
        }
        index=index+static_cast<ulong>(indexIncrement);
        return true;
    } catch (const Exception& e) {
        QMessageBox::critical(nullptr, "Error", QString::fromStdString(e.getDetailMsg()));
        return false;
    }

}

bool Hdf5SourceSink::StopAcquisition()
{
    try {
        hFile->close();
        delete hFile;
    } catch (const Exception& e) {
        QMessageBox::critical(nullptr, "Error", QString::fromStdString(e.getDetailMsg()));
        return false;
    }

    return true;
}

bool Hdf5SourceSink::ReleaseCamera()
{
    return true;
}

bool Hdf5SourceSink::StartRecording(QString recFold, QString codec, int, int cols, int rows)
{
    QString basename;
    QDir basedir(recFold);
    if (basedir.exists()) {
        QDateTime mom = QDateTime::currentDateTime();
        basename=recFold+"/"+mom.toString("yyyyMMdd-hhmmss");
    } else {
        QRegExp rx("(.+)/(.+)$"); //somepath/somename.someextension
        int pos=0;
        pos=rx.indexIn(recFold);
        if (pos>-1) {
            basename=rx.cap(1)+"/"+rx.cap(2);
        } else {
            qDebug()<<"Recording Folder does not exist";
            QDateTime mom = QDateTime::currentDateTime();
            basename=recFold+"/"+mom.toString("yyyyMMdd-hhmmss");
        }
//        qDebug()<<"recFold"<<dir<<" basename "<<basename;
    }


    QString filenam=basename+".h5";
    std::string datasetname="data";
    timestamps.clear();

//    qDebug()<<"Init recording: "<<filenam<<" with codec "<<codec;
#ifdef Q_OS_WIN32
    hFile=new H5File(filenam.toStdString().c_str(),H5F_ACC_TRUNC);
#else
    hFile=new H5File(filenam.toUtf8().data(),H5F_ACC_TRUNC);
#endif
    if (codec=="HDF8") {
        readType=PredType::NATIVE_UCHAR;
        dataformat="MONO8";
    } else if (codec=="HDF12") {
        dataformat="MONO12";
        readType=PredType::NATIVE_UINT16;
    } else if (codec=="HDF14") {
        dataformat="MONO14";
        readType=PredType::NATIVE_UINT16;
    } else if (codec=="HDFBAYERRG8") {
        dataformat="BAYERRG8";
        readType=PredType::NATIVE_UCHAR;
    } else if (codec=="HDFBAYERGB8") {
        dataformat="BAYERGB8";
        readType=PredType::NATIVE_UCHAR;
    } else if (codec=="HDFBAYERRG12") {
        dataformat="BAYERRG12";
        readType=PredType::NATIVE_UINT16;
    } else if (codec=="HDFRGB8") {
        readType=PredType::NATIVE_UCHAR;
        dataformat="RGB8";
    } else {
        qDebug()<<"This format not yet implemented: "<<codec;
        return false;
    }


    /*
     * Create the data space with unlimited size in the first dimension.
     */



    recrows=static_cast<hsize_t>(rows);
    reccols=static_cast<hsize_t>(cols);
    if (dataformat=="RGB8") {
        reccols=static_cast<hsize_t>(cols*3);
    }
    hsize_t      dims[3]  = { 1,recrows,reccols};  // dataset dimensions at creation

    hsize_t unlimSize=static_cast<hsize_t>(static_cast<hssize_t>(-1));
    hsize_t      maxdims[3] = {unlimSize, recrows,reccols};
    DataSpace mspace1( 3, dims, maxdims);
    /*
     * Modify dataset creation properties, i.e. enable chunking.
     */

    hsize_t      chunk_dims[3] ={1,recrows, reccols};
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

    // create an attribute to this dataset to describe the format (8, 12 or 14 bit or Bayer)
    DataSpace attr_dataspace = DataSpace(H5S_SCALAR);
    if (dataformat=="MONO8") {
        StrType strdatatype(PredType::C_S1, 5); // of length 6 characters => MONO8, MONO12, MONO14
        Attribute myatt_in = dataset.createAttribute("PIXFORMAT", strdatatype, attr_dataspace);
        myatt_in.write(strdatatype, "MONO8");
    } else if (dataformat=="MONO12") {
        StrType strdatatype(PredType::C_S1, 6); // of length 6 characters => MONO8, MONO12, MONO14
        Attribute myatt_in = dataset.createAttribute("PIXFORMAT", strdatatype, attr_dataspace);
        myatt_in.write(strdatatype, "MONO12");
    } else if (dataformat=="MONO14") {
        StrType strdatatype(PredType::C_S1, 6); // of length 6 characters => MONO8, MONO12, MONO14
        Attribute myatt_in = dataset.createAttribute("PIXFORMAT", strdatatype, attr_dataspace);
        myatt_in.write(strdatatype, "MONO14");
    } else if (dataformat=="BAYERRG8") {
        StrType strdatatype(PredType::C_S1, 8); // of length 6 characters => MONO8, MONO12, MONO14
        Attribute myatt_in = dataset.createAttribute("PIXFORMAT", strdatatype, attr_dataspace);
        myatt_in.write(strdatatype, "BAYERRG8");
    } else if (dataformat=="BAYERGB8") {
        StrType strdatatype(PredType::C_S1, 8); // of length 6 characters => MONO8, MONO12, MONO14
        Attribute myatt_in = dataset.createAttribute("PIXFORMAT", strdatatype, attr_dataspace);
        myatt_in.write(strdatatype, "BAYERGB8");
    } else if (dataformat=="BAYERRG12") {
        StrType strdatatype(PredType::C_S1, 9); // of length 6 characters => MONO8, MONO12, MONO14
        Attribute myatt_in = dataset.createAttribute("PIXFORMAT", strdatatype, attr_dataspace);
        myatt_in.write(strdatatype, "BAYERRG12");
    } else if (dataformat=="RGB8") { // this will be converted to grayscale for the moment
        StrType strdatatype(PredType::C_S1, 4); // of length 6 characters => MONO8, MONO12, MONO14
        Attribute myatt_in = dataset.createAttribute("PIXFORMAT", strdatatype, attr_dataspace);
        myatt_in.write(strdatatype, "RGB8");
    }

    index=0;
    return true;
}

bool Hdf5SourceSink::RecordFrame(ImagePacket &source)
{
/*    cv::Mat temp;
    if (source.pixFormat=="RGB8") { // too much changes to make it work in colour for webcams=> just convert to gray.
        cv::Mat temp(recrows,reccols,CV_8U);
        temp.data=source.image.data;
        //cv::cvtColor(source.image,gray,CV_RGB2GRAY);
//        source.image=Fakegray;
    } else {
        temp=source.image;
    }
*/
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
        DataSpace memspac(2,dumbIm,nullptr);

        dataset.write(source.image.data,readType,memspac,filespace);

    }
    index++;
    timestamps.push_back(source.timeStamp);
    return true;
}

bool Hdf5SourceSink::StopRecording()
{
    // first record timestamps
    cv::Mat doubleT=cv::Mat(1,static_cast<int>(timestamps.size()),CV_64F);
    for (size_t l=0;l<timestamps.size();l++) {
        doubleT.at<double>(0,static_cast<int>(l))=timestamps[l];
    }
    hsize_t  hdims[1]={timestamps.size()};
    H5::DataSpace dataspaceT=DataSpace( 1, hdims );
    H5::DataSet datasetT = hFile->createDataSet( "time", PredType::NATIVE_DOUBLE, dataspaceT);
    datasetT.write(doubleT.data,PredType::NATIVE_DOUBLE);

    // then close everything
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
        skipping=static_cast<int>(round(dims[0]/10.0));
    } else {
        skipping=-static_cast<int>(round(dims[0]/50.0));
    }
//    qDebug()<<"Will try to skip "<<skipping<<" frames";

    if ((static_cast<int>(index)+skipping >= static_cast<int>(dims[0]-1))||(static_cast<int>(index)+skipping <0)) {
        return true;
    }

    index+=static_cast<ulong>(skipping);
    return true;
}
