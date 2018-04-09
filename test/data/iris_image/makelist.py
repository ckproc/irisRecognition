import glob
import sys
f1=open('db_list.txt','w+')
f2=open('test_list.txt','w+')
num=0
for file in glob.glob("Image/*/*.jpg"):
 fullname="data/iris_image/"+file
 path=fullname+"\n"
 if num<4:
  f2.write(path)
 else:
  f1.write(path)
 num=num+1
 if num==12:
  num=0
 