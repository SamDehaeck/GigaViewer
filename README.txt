The goal of this program is to create an opengl viewer capable of working
with Prosilica/AVT cameras and modify their important settings such as
framerate and exposure levels. And allow saving the acquired images to disk.

For the moment it works on linux and compiles on windows.
However, it can lock up the webcam on windows using opencv and also the display does not seem right.
Should do more testing to find out why this is, but unfortunately I do not have a windows test box..

Installing on Linux:
--------------------

* Easiest way is to install qtcreator and open the project file + compile

Post-install tips for Linux:
----------------------------

* The included gigaviewer.desktop file should be copied to the ~/.local/share/applications folder and its executable bit set
* The GigaViewer.svg file should be copied to ~/.local/share/icons folder

Now you should be able to make a launcher e.g. on ubuntu by dragging the desktop file to the sidebar.
Note that it could happen that it only shows up after a restart.

To correctly associate the created .fmf movies with the application do the following:

* copy video-fmf.xml to the ~/.local/share/mime/packages folders
* copy GigaViewer.svg to /usr/share/icons/gnome/scalable/mimetypes/video-fmf.svg
* update-mime-database ~/.local/share/mime
* sudo gtk-update-icon-cache /usr/share/icons/gnome
