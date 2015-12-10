The goal of this program is to create a Qt and opengl-based viewer capable of working
with scientific cameras (GigE and USB3 based), modify their important settings such as
framerate and exposure levels and allow to save the acquired images to disk as fast as possible.

The program is compatible with the PvApi, Vimba  and IDS (µ-eye) SDK's, which allows it to connect
to many GigE-compliant cameras and USB3 camera. Through OpenCV, many other cameras can be accessed such as webcams.
The program is cross-platform and works on Linux, Mac and Windows. However, most cameras are only supported on Linux
and Windows due to restrictions of the underlying libraries. The Mac version serves mostly to view the recorded videos.

Recording to disk is also focussed on scientific usage of the data and therefore does not use typical movie
containers such as AVI or MPEG. Instead, movies are stored to HDF5 or FMF (=> link FlyMovieFormat) formats in which
a precise timestamp can be recorded per image (hence allowing variable framerates).
This also allows easy access to the video as a 3D matrix from Python and other languages.

Installation:
-------------
Detailed instruction for installation on Linux, Mac and Windows can be found in the CompilingLinux/Mac/Windows text files.
Precompiled (standalone) binaries are available for Windows which do not require any installation.

