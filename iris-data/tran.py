import glob
import Image
import os
path=glob.glob("./testdata/*.jpg")
for img in path:
 image=Image.open(img)
 image=image.rotate(90)
 image.save(img)
 