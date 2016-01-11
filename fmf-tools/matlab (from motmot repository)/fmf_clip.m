function fmf_clip( raw_filename, clip_filename, clip_x_vect, clip_y_vect, frame_vect, progress_meters )
% fmf_clip( raw_filename, clip_filename, clip_vect, start_frame, end_frame, progress_meters )
%
% outputs a smaller version of RAW_FILENAME in CLIP_FILENAME
% CLIP_X_VECT and CLIP_Y_VECT are the pixel coordinates which will be output
%   (default [] := whole frame)
% FRAME_VECT are the frame numbers which will be output (default [] := whole movie)
% PROGRESS_METERS is 0 for none, 1 for text, 2 for graphical (default none)
%
% JAB 5/11/07

% default arguments
if nargin < 3, clip_x_vect = []; end
if nargin < 4, clip_y_vect = []; end
if nargin < 5, frame_vect = []; end
if nargin < 6, progress_meters = 0; end

% read header from raw_filename
[header_size, version, f_height, f_width, bytes_per_chunk, max_n_frames, data_format] = ...
   fmf_read_header( raw_filename );
% open input file
in_fp = fopen( raw_filename );

% fill in ambiguous argument values
if isempty( clip_x_vect ), clip_x_vect = 1:f_width; end
if isempty( clip_y_vect ), clip_y_vect = 1:f_height; end
if isempty( frame_vect ), frame_vect = 1:max_n_frames; end

% write new output header
out_fp = fmf_write_header( clip_filename, header_size, version, length( clip_y_vect ), length( clip_x_vect ), ...
   length( clip_x_vect )*length( clip_y_vect ) + 8, length( frame_vect ), data_format );

if progress_meters == 1, fprintf( 1, 'frame %4d/%4d', 0, length( frame_vect ) )
elseif progress_meters == 2, wb = waitbar( 0, sprintf( 'frame 0/%d', length( frame_vect ) ) ); end

% process frames
for fr = frame_vect
   if progress_meters == 1, fprintf( 1, '\b\b\b\b\b\b\b\b\b%4d/%4d', find( frame_vect == fr ), length( frame_vect ) )
   elseif progress_meters == 2,
      f = find( frame_vect == fr );
      waitbar( f / length( frame_vect ), wb, sprintf( 'frame %d/%d', f, length( frame_vect ) ) )
   end
   
   % read input frame
   fseek( in_fp, header_size + (fr-1)*bytes_per_chunk, 'bof' );
   [frame, stamp] = fmf_read_frame( in_fp, f_height, f_width, bytes_per_chunk, data_format );
   
   % write output frame
   fmf_write_frame( out_fp, frame(clip_y_vect,clip_x_vect), stamp, data_format );
end % for each output frame

fclose( in_fp );
fclose( out_fp );

if progress_meters == 1, fprintf( 1, '\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b' )
elseif progress_meters == 2, close( wb ); end
