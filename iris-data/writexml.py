import xml.etree.ElementTree as ET
tree = ET.parse('traineye.xml')
root = tree.getroot()
with open(r'keypoints.txt') as f:
 s=f.read()
s=[x.strip().split(" ") for x in s.strip().split("\n")]
j=0
for box in root.iter('box'):
 for i in range(20):
     partE=ET.Element('part')
     box.append(partE)
     xx=s[j][i]
     yy=s[j][i+20]
     #new_rank = int(rank.text) + 1
     #rank.text = str(new_rank)
     partE.set('name', str(i))
     partE.set('x', xx)
     partE.set('y', yy)
     #rank.set('x','89')
     #print int(rank.get('x'))+1
 j=j+1
 print j   
tree.write('output.xml')
