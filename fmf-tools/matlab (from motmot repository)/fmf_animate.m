function fmf_animate( filename, f_start, nframes, incr, flip, cm, aoi, show_orientation )
% fmf_animate( filename, f_start, nframes, incr, flip, cm, aoi, show_orientation )
%
% F_START is the starting frame (1-based) [default 1]
% NFRAMES is the number of frames to animate [default inf]
% INCR is the frame index increment to plot [default 1]
% FLIP is a flag to flip the image left-to-right [default 0]
% CM is an optional colormap [default gray]
%   help graph3d to see colormap possibilities
% AOI is an optional area of interest, a vector containing x,y,w,h [default
%   full image] entering 'all' also displays full image
% SHOW_ORIENTATION is option to show line with orientation from file with
%   same name but .trx extension (0 - do not show; 1 - do show) [default 0]
%
% JAB 7/1/04
% Edited PTW 1/1/08

%    hsv        - Hue-saturation-value color map.
%    hot        - Black-red-yellow-white color map.
%    gray       - Linear gray-scale color map.
%    bone       - Gray-scale with tinge of blue color map.
%    copper     - Linear copper-tone color map.
%    pink       - Pastel shades of pink color map.
%    white      - All white color map.
%    flag       - Alternating red, white, blue, and black color map.
%    lines      - Color map with the line colors.
%    colorcube  - Enhanced color-cube color map.
%    vga        - Windows colormap for 16 colors.
%    jet        - Variant of HSV.
%    prism      - Prism color map.
%    cool       - Shades of cyan and magenta color map.
%    autumn     - Shades of red and yellow color map.
%    spring     - Shades of magenta and yellow color map.
%    winter     - Shades of blue and green color map.
%    summer     - Shades of green and yellow color map.

if nargin < 8, show_orientation = 0; end
if nargin < 6, cm = 'gray'; end
if nargin < 5, flip = 0; end
if nargin < 4, incr = 1; end
if nargin < 3, nframes = inf; end
if nargin < 2, f_start = 1; end

if strncmpi(fliplr(filename),'fmf.',4);
    trx_filename = filename(1:end-3);
    trx_filename = strcat(trx_filename,'trx');
else
    trx_filename = strcat(filename,'.trx');
end
if show_orientation == 1
	[background_image,trx_data] = read_trx(trx_filename);
	x=trx_data(1:2,:)-trx_data(4:5,:)+1;
	slopes = trx_data(3,:);
end

[header_size, version, h, w, frame_size, max_n_frames, data_format] = fmf_read_header(filename);
if( nframes ~= inf && nframes + f_start - 1 > max_n_frames ),
    nframes = max_n_frames - f_start + 1;
    warning( 'fmf_short', sprintf( 'not enough frames in file -- returning %d frames', nframes ) );
elseif nframes == inf,
    nframes = max_n_frames - f_start + 1;
end

data = uint8( zeros( h, w ) );
figure( 19 ); clf
pause( 1e-3 )

% read frames
fp = fopen( filename, 'r' );
fseek( fp, header_size, 'bof' );
fseek( fp, frame_size*(f_start-1), 'cof' );

for f=1:incr:nframes,
    for i=1:incr, data = fmf_read_frame( fp, h, w, frame_size, data_format ); end

    % flip
    if flip, data = fliplr( data ); end

    % truncate to area of interest
    if (nargin >= 7) & (aoi ~= 'all')
        data = data(aoi(2):aoi(2)+aoi(4)-1,aoi(1):aoi(1)+aoi(3)-1);
    end

    % for first frame, size figure to real size
    if f == 1,
        fprintf( 1, 'frame:                ' );
        imshow( data )
        eval( sprintf( 'colormap %s', cm ) )
    end

    % display image
    fprintf( 1, '\b\b\b\b\b\b\b\b\b\b\b\b\b%6d/%6d', f+f_start-1, nframes+f_start-1 );
    %imagesc( data )
    image( data )
    set( gca, 'visible', 'off' )

    if show_orientation == 1
        X = 1:w;
        Y = slopes(f+f_start-1)*(X-x(1,f+f_start-1)) + x(2,f+f_start-1);
        line(X,Y)
        line(x(1,f+f_start-1),x(2,f+f_start-1),'marker','o')
        %     % to show perpendicular line, uncomment these lines
        %     Y2 = -(1/slopes(f+f_start-1))*(X-x(1,f+f_start-1)) + x(2,f+f_start-1);
        %     line(X,Y2)
    end
    drawnow
    %pause( 1e-3 ) % allow plotting to finish
end
fclose( fp );
fprintf( 1, '\n' );
beep
