function [data, stamps, f] = fmf_read( filename, f_start, nframes, show_progress )
% [data, stamps, f] = fmf_read( filename, f_start, nframes )
%
% reads FlyMovieFormat data from FILENAME
%
% starts with frame F_START (1-based) [defaults to 1], reads NFRAMES [defaults to inf]
%
% DATA is HxWxNFRAMES where H and W are the height and width of a
% frame STAMPS are the timestamps for each frame, double format F is
% the number of frames actually read
%
% JAB 6/30/04

if nargin < 4, show_progress = 0; end
if nargin < 3, nframes = inf; end
if nargin < 2, f_start = 1; end

[header_size, version, h, w, bytes_per_chunk, max_n_frames, data_format] = fmf_read_header( filename );
if nframes == inf,
	nframes = max_n_frames - f_start + 1;
end
if nframes + f_start - 1 > max_n_frames | nframes < 0,
	nframes = max_n_frames - f_start + 1;
	warning( sprintf( 'not enough frames in file -- returning %d frames', nframes ) );
end

datatype = fmf_get_datatype(data_format);
data = zeros( h, w, nframes, datatype );
stamps = zeros( 1, nframes );

% read frames
fp = fopen( filename, 'r' );
fseek( fp, header_size, 'bof' );
fseek( fp, (f_start-1)*bytes_per_chunk, 'cof' );

% add wait bar
if show_progress
 bar_handle = waitbar(0,'reading');
end
for f=1:nframes,
    if show_progress
      waitbar(f/nframes, bar_handle, sprintf('reading %d/%d',f,nframes))
    end
    
    [data(:,:,f), stamps(f)] = fmf_read_frame( fp, h, w, ...
	    bytes_per_chunk, data_format );
end
fclose( fp );

if show_progress
  close(bar_handle)
end
