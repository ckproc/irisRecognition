
# Prepare Data

### Usage

1.Prepare training data. You need prepare the iris image and label files like data/train.
    
    & matlab -nodisplay
    > dataPre
    
	
2.write label file .xml
 
 first you should produce a .xml file which list train files and labels https://github.com/davisking/dlib/tree/master/tools/imglab
 
 then write the shape labels into .xml file.
 
    python writexml.py
    
	
3.put the .xml file into traindata/