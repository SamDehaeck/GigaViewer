The goal of this program is to create an opengl viewer capable of working
(mainly) with GigE cameras and modify their important settings such as
framerate and exposure levels, while allowing to save the acquired images to disk.

The program is compatible with the PvApi and Vimba SDK's, which allows it to connect
to AVT cameras. But other camera types can be added if necessary.
For the moment it works on linux and windows provided that it can find
the opencv and QT libraries.

Installing on Linux:
--------------------

* Easiest way is to install qtcreator and open the project file + compile

Post-install tips for Linux:
----------------------------

* The included gigaviewer.desktop file should be copied to the ~/.local/share/applications folder and its executable bit set
* The GigaViewer.svg file should be copied to ~/.local/share/icons folder

Now you should be able to make a launcher e.g. on ubuntu by dragging the desktop file to the sidebar
or e.g. on gnome3 by first starting the .desktop file by double-clicking it. The icon will be visible now in the sidebar; just add it to the favourites.
Note that it could happen that it only shows up after a restart.

To correctly associate the created .fmf movies with the application do the following:

* copy video-fmf.xml to the ~/.local/share/mime/packages folders
* copy GigaViewer.svg to /usr/share/icons/gnome/scalable/mimetypes/video-fmf.svg
* update-mime-database ~/.local/share/mime
* sudo gtk-update-icon-cache /usr/share/icons/gnome

if this is not working, copy it to your iconset folder mimetypes/scalable folder on apply gtk-update-icon-cache to this folder

