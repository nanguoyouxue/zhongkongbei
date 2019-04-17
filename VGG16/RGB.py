from PIL import Image
import os
path = 'pic/train/xuehua/' #图片目录
for file in os.listdir(path):
     extension = file.split('.')[-1]
     if extension == 'jpg':
           fileLoc = path+file
           img = Image.open(fileLoc)
           if img.mode != 'RGB':
                 print(file+', '+img.mode)