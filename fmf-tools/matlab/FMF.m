classdef FMF
    % FMF object allows to read an FMF movie
    %
    % This class was based on the functions written by the MOTMOT project
    
    properties
        filePointer; % a pointer to the open file
        datatype; 
        data_format;
        header_size;
        h;
        w; 
        bytes_per_chunk;
        n_frames;
    end
    
    methods
        function obj=FMF(filename)
            obj.filePointer = fopen( filename, 'r' );
            obj=readHeader(obj);
%            obj.datatype = fmf_get_datatype(obj.data_format);
            
        end
        
        function [image,time] = getFrame(obj,index)
            if (index>obj.n_frames),
                warning('index is larger than amount of frames present');
                time = 9e9;
                image = zeros( obj.h, obj.w, 'double' );
            else,
                fseek( obj.filePointer, obj.header_size+(index-1)*obj.bytes_per_chunk, 'bof' );
                [image,time]=readData(obj);
            end;
            
        end
    end
    
    methods (Access = private)    
        function obj=readHeader(obj)
            % read header
            header_size = 28;
            version = double( fread( obj.filePointer, 1, 'uint32' ) );
            if version ~= 1,
              if version ~= 3,
                error( 'version not supported -- FMF versions 1 and 3 only' );
              end
            end

            if version == 1,
              obj.data_format = 'MONO8';
            end

            if version == 3,
              format_len = double( fread( obj.filePointer, 1, 'uint32' ) );
              obj.data_format = strcat( char( fread( obj.filePointer, format_len))');
              bits_per_pixel = double( fread( obj.filePointer, 1, 'uint32' ) );
              obj.header_size = header_size + format_len + 8;
            end
            
            obj=translateDataFormat(obj);

            obj.h = double( fread( obj.filePointer, 1, 'uint32' ) );
            obj.w = double( fread( obj.filePointer, 1, 'uint32' ) );
            %bytes_per_chunk = double( fread( fp, 1, 'uint64' ) );
            obj.bytes_per_chunk = double( fread( obj.filePointer, 1, 'long' ) );
            %frame_count_location = ftell(fp);
            %max_n_frames = double( fread( fp, 1, 'uint64' ) );
            temp=fread( obj.filePointer, 2, 'uint32' );
            obj.n_frames = double( temp(2) );  % ugly 'hack' to read this correctly, will not work for very large files
            %warning('Noted amount of frames: %d ',obj.n_frames);

            if obj.n_frames <= 0,
              fseek(obj.filePointer,0,'eof');
              endPos=ftell(obj.filePointer);
              obj.n_frames=int16((endPos-obj.header_size)/obj.bytes_per_chunk);
              warning('Could not read number of frames, calculated it to be %d ',obj.n_frames);
            %  fseek( fp, frame_count_location, 'bof');
            %  fwrite( fp, max_n_frames, 'uint64' );
            end
        end
        
        function obj=translateDataFormat(obj)
            if strcmp(obj.data_format,'MONO8'),
              obj.datatype = 'uint8'; % replace with '*uint8' if you want the output to be uint8 instead of double
            else,
              if (strcmp(obj.data_format,'MONO12')||strcmp(obj.data_format,'MONO14')||strcmp(obj.data_format,'BAYERRG12')),
                obj.datatype = 'uint16';
              else,
                if strcmp(obj.data_format,'BAYERRG8'),
                    obj.datatype = 'uint8';
                else,
                  if strcmp(obj.data_format,'BAYERGB8'),
                    obj.datatype = 'uint8';
                  else,
                    error( 'Unrecognised data type' );
                    obj.datatype='uint8';
                  end
                end
              end
            end
        end
        
        function [data,stamp]=readData(obj)
            if feof( obj.filePointer ),
              stamp = 9e9;
              data = zeros( obj.h, obj.w, obj.datatype );
            else
              stamp = fread( obj.filePointer, 1, 'double' );
            end

            if feof( obj.filePointer ),
              stamp = 9e9;
              data = zeros( obj.h, obj.w, obj.datatype );
            else
              buf = fread( obj.filePointer, obj.h*obj.w, obj.datatype);
              if size( buf, 2 ) == 0 | size( buf, 1 ) ~= obj.w*obj.h
                stamp = 9e9;
                data = zeros( obj.h, obj.w, obj.datatype );
              else
                data = reshape( buf, obj.w, obj.h )';
              end
              %old: data = reshape( fread( obj.filePointer, h*w ), w, h )';
            end
        end
        
        function delete(obj)
            fclose( obj.filePointer );
        end
    end
end
