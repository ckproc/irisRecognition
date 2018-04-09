import glob
import Image
import os
import cv2
import shutil
path=glob.glob("Image/*/*.jpg")





for img in path:
 #image=cv2.imread(img)
 name=os.path.split(img)[1]
 di=os.path.split(img)[0]
 #out=cv2.resize(image,(240,270))
 #subpath="image/"+os.path.split(img)[1]
 #cv2.imwrite(subpath,out)
 newpath="/home/ckp/irisRecognition/iris-data/traindata/"+di[6:]+"_"+name
 shutil.copyfile(img,newpath)
 
 
 
 
 
 
''' 
 image=Image.open(img)
 name=os.path.split(img)[1]
 dic=os.path.split(img)[0]
 newpath1=dic+"/"+"left_"+name
 newpath2=dic+"/"+"right_"+name
 #image=image.rotate()
 area1=(0,0,480,540)
 area2=(480,0,960,540)
 cropimg1=image.crop(area1)
 cropimg2=image.crop(area2)
 cropimg1.save(newpath1)
 cropimg2.save(newpath2)
'''