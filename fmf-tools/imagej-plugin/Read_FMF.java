/*
Author : Sam Dehaeck (based on work by Jerome Mutterer - Guess Raw)
Purpose : Read in a FMF file
Application example : Read in a FMF file
*/

import java.io.*;
import ij.*;
import ij.io.*;
import ij.plugin.PlugIn;
import ij.gui.*;


public class Read_FMF implements PlugIn {

        // variables init
        
        public String FILENAME = "";
        public String DIRECTORY = "";
        public long IMAGE_WIDTH = 0;
        public long FILE_LENGTH = 0;
        public long START_VALUE = 0;
        public File RAW;
        
        // additionnal setings for raw file openning can be edited here :
        
        public long NUMBER_IMAGES = 1;
        public long FILE_OFFSET = 41;
        public long FILE_GAP = 8;
        public boolean WHITE_BACKGROUND = false;
        
        // end additionnal settings
               
        
        public void run(String arg) {
        
            OpenDialog od = new OpenDialog("Read FMF file... ", arg);
                FILENAME = od.getFileName();
                    if (FILENAME == null) {IJ.error("no file selected"); return;}
                DIRECTORY = od.getDirectory();
                    if (DIRECTORY == null) {IJ.error("no file selected"); return;}
            
            RAW = new File(DIRECTORY, FILENAME);
            FILE_LENGTH = RAW.length();
            
            DataInputStream inFile = null;
            try {
                inFile=new DataInputStream(new FileInputStream(RAW));
                //int version=inFile.readInt();
                //int formatlen=inFile.readInt();
                //long b = version & 0xffffffffL; 
                
                byte[] buffer = new byte[4];
                inFile.readFully(buffer);
                int version = (buffer[0] & 0xFF) | (buffer[1] & 0xFF) << 8 | (buffer[2] & 0xFF) << 16 | (buffer[3] & 0xFF) << 24;
            
                inFile.readFully(buffer);
                int formatlen = (buffer[0] & 0xFF) | (buffer[1] & 0xFF) << 8 | (buffer[2] & 0xFF) << 16 | (buffer[3] & 0xFF) << 24;
                FILE_OFFSET = 48+(formatlen-4); // should add first timestamp of 8 bytes after real header size!!!
                
                //inFile.skipBytes(formatlen);
                char lettre='z';
                String form="";
                byte buflet;
                for (int i=0;i<formatlen;i++) {
//                    lettre=inFile.readChar();
                    buflet=inFile.readByte();
//                    lettre=buflet;
                    form=form+(char)buflet;
                }
                
                
                
                String format="8-bit";
                if (form.equals("MONO8")) {
					format="8-bit";
//					IJ.showMessage(form+" = "+"MONO8");
				} else if (form.equals("MONO12")) {
//					IJ.showMessage(form+" = "+"MONO12");
                    format="[16-bit Unsigned]";
                } else if (form.equals("MONO14")) {
//					IJ.showMessage(form+" = "+"MONO14");
                    format="[16-bit Unsigned]";
				} else if (form.equals("RGB8")) {
//					IJ.showMessage(form+" = "+"RGB8");
					format="[24-bit BGR]";
				} else {
					IJ.showMessage(form+" did not match anything.");
                }

                //other options
                //image=[24-bit RGB]
                //image=[16-bit Unsigned]
                
                int bitsPer=0;
                int rows=0;
                int cols=0;
                int amount=0;
                for (int i=0 ; i < 7 ; i++) {
                    inFile.readFully(buffer);
                    int newVal = (buffer[0] & 0xFF) | (buffer[1] & 0xFF) << 8 | (buffer[2] & 0xFF) << 16 | (buffer[3] & 0xFF) << 24;
                    if (i==0) {
                        bitsPer=newVal;
                    } else if (i==1) {
                        rows=newVal;
                    } else if (i==2) {
                        cols=newVal;
                    } else if (i==5) {
                        amount =newVal;
                    }

                }
                
                
                
            
                //IJ.showMessage("I hope this is it: "+version+" - "+formatlen+" - "+FILE_OFFSET+" - "+bitsPer+" - "+rows+" - "+cols+" - "+amount);
            

           
                IJ.run("Raw...", "open=["+DIRECTORY+FILENAME+"] image="+format+" width="+cols+" height="+rows+" offset="+FILE_OFFSET+" number="+amount+" gap="+FILE_GAP+" little-endian");
                
            } catch (Exception ioe) {
                ioe.printStackTrace();
            } finally {
                if (inFile != null) {
                    try {
                        inFile.close();
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }
                }
            }
            
            }

  }
