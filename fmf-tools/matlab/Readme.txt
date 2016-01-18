To open .fmf files in Matlab you need to use the provided FMF.m file
=> place in a good location where Matlab always looks or copy to your working folder
Then type:
f=FMF('E:path\to\file.fmf');    => will open the file and read the header. For now only MONO8, MONO12 and MONO14 implemented
f.w,f.h,f.n_files               => these give you info: width, height and amount of files in the movie
[I,t]=f.getFrame(50);           => I will be the image in double format and t the timestamp. 
imshow(I,[])                    => extra ,[] required for Matlab to use the true range of the image, otherwise just a white image is shown.
