import numpy as np

class FMF:
	def __init__(self,filename):
		f=open(filename,mode='rb')
		version,forLength=np.fromfile(f,np.uint32,2)
		formatting=f.read(forLength)
		bitsper,rows,cols=np.fromfile(f,np.uint32,3)
		bytesperChunk,N=np.fromfile(f,np.uint64,2)
		offsetHeader=f.tell()
		f.close()

		if (formatting!="RGB8"):
			if (bitsper==8):
				typeData=np.uint8
			elif (bitsper==16):
				typeData=np.uint16
			
			dataformat=np.dtype([('t',np.double),('I',typeData,(rows,cols))])
		else:
			dataformat=np.dtype([('t',np.double),('I',np.uint8,(rows,cols,3))])

		self.mapper=np.memmap(filename,dtype=dataformat,mode='r',offset=offsetHeader)
		self.I=self.mapper['I']
		self.T=self.mapper['t']
