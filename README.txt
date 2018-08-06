The goal of this program is to create a Qt and opengl-based viewer capable of working
with scientific cameras (GigE and USB3 based), modify their important settings such as
framerate and exposure levels and allow to save the acquired images to disk as fast as possible.

The program is compatible with the PvApi, Vimba  and IDS (µ-eye) SDK's, which allows it to connect
to many GigE-compliant and USB3 cameras (and Firewire in Windows). Through OpenCV, many other cameras can be accessed such as webcams.
The program is cross-platform and works on Linux, Mac and Windows. However, most cameras are only supported on Linux
and Windows due to restrictions of the underlying libraries. The Mac version serves mostly to view the recorded videos for now.

Recording to disk is focussed on the scientific usage of the data and therefore does not use typical movie
containers such as AVI or MPEG. Instead, movies are stored to HDF5 (=> https://www.hdfgroup.org/HDF5/) or FMF (=> http://code.astraw.com/projects/motmot/fly-movie-format.html) formats in which
a precise timestamp can be recorded per image (hence allowing variable framerates).
This also allows easy access to the video as a 3D matrix from Python and other languages.

Installation:
-------------
* Detailed instructions for installation on Linux, Mac and Windows can be found in the CompilingLinux/Mac/Windows text files.
* Precompiled (standalone) binaries are available for Windows which do not require any installation.
* The format of the recorded FMF files is documented on http://code.astraw.com/projects/motmot/fly-movie-format.html. From this site,
  also some tools can be downloaded for converting these files. A simple access from python is also possible using the included FMF.py file.
  To use it do:
    import FMF
    vid=FMF.FMF("myfile.fmf")
    vid.I    #=> this is the 3D-matrix containing your movie; to show a single image do imshow(vid.I[0])
    vid.T    #=> This contains the corresponding timestamps : print vid.T[0]


