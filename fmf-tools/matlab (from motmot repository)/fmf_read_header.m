function [header_size, version, f_height, f_width, bytes_per_chunk, max_n_frames, data_format] = fmf_read_header( filename )
% [header_size, version, f_height, f_width, bytes_per_chunk, max_n_frames] = fmf_read_header( filename )
%
% reads FlyMovieFormat header data from FILENAME
%
% HEADER_SIZE is in bytes for the appropriate file format (fmf VERSION)
% F_HEIGHT and F_WIDTH are the number of pixels in a frame
% BYTES_PER_CHUNK is the number of bytes per frame in the file
% MAX_N_FRAMES is the number of frames in the file
%
% JAB 7/1/04

header_size = 28;

fp = fopen( filename, 'r+' );

% read header
version = double( fread( fp, 1, 'uint32' ) );
if version ~= 1,
  if version ~= 3,
    error( 'version not supported -- FMF versions 1 and 3 only' );
  end
end

if version == 1,
  data_format = 'MONO8';
  bits_per_pixel = 8;
end

if version == 3,
  format_len = double( fread( fp, 1, 'uint32' ) );
  data_format = strcat( char( fread( fp, format_len))');
  bits_per_pixel = double( fread( fp, 1, 'uint32' ) );
  header_size = header_size + format_len + 8;
end

f_height = double( fread( fp, 1, 'uint32' ) );
f_width = double( fread( fp, 1, 'uint32' ) );
%bytes_per_chunk = double( fread( fp, 1, 'uint64' ) );
bytes_per_chunk = double( fread( fp, 1, 'long' ) );
frame_count_location = ftell(fp);
%max_n_frames = double( fread( fp, 1, 'uint64' ) );
max_n_frames = double( fread( fp, 1, 'long' ) );

if max_n_frames <= 0,
  while 1,
    [data, stamp] = fmf_read_frame( fp, f_height, f_width, ...
	bytes_per_chunk, data_format );
    if stamp == 9e9,
      break
    end
    max_n_frames = max_n_frames + 1;
  end
  fseek( fp, frame_count_location, 'bof');
  fwrite( fp, max_n_frames, 'uint64' );
end

fclose( fp );
