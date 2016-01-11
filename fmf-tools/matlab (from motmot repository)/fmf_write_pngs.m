function F = fmr_write_pngs( filename, f_start, nframes, show_progress, directory )
% F = fmf_write_pngs( filename, f_start, nframes, show_progress, directory )
%
% writes FlyMovieFormat FILENAME frames to individual .png files in
% DIRECTORY [defaults to ./]
%
% starts with frame F_START (1-based) [defaults to 1], writes NFRAMES
% [defaults to inf]. Shows progress bar if SHOW_PROGRESS == 1 [defaults to 0]
% Returns F, the number of frames written to png filess
% 
% Designed to work with linux command:
% ffmpeg -b 80000 -f mpeg2video -r 30 -i frame%06d.png movie.mpeg
% # NOTE: frame numbers *must* start with 1
% 
% PTW 10/16/07

if nargin < 5, directory = './'; end
if nargin < 4, show_progress = 0; end
if nargin < 3, nframes = inf; end
if nargin < 2, f_start = 1; end

if ~strncmpi(fliplr(directory),'/',1);
    directory = strcat(directory,'/');
end

if ~strncmpi(fliplr(filename),'fmf.',4);
    filename = strcat(filename,'.fmf');
end

[data, stamps, F] = fmf_read( filename, f_start, nframes);

% add wait bar
if show_progress
 bar_handle = waitbar(0,'writing');
end
for f=1:F,
    if show_progress
      waitbar(f/F, bar_handle, sprintf('writing %d/%d',f,F))
    end
    
    fn = int2str(f);
    imname = '000000.png';
    imname(6-length(fn)+1:6) = fn;
    imname = [directory,imname];
    imwrite(data(:,:,f),imname,'png');
end

if show_progress
  close(bar_handle)
end
