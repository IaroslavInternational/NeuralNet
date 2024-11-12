import sys
import cv2

bin_img = []

def add_pixel(val: int):
    bin_img.append(int(val != 255))

def image2binary(path: str):
    img = cv2.imread(path, 0)
    rows,cols = img.shape
    
    for i in range(rows):
        for j in range(cols):
            add_pixel(img[i, j])
    
    f = open('nni/proc_data.nni', 'w')
    
    c = 0
    for pixel in bin_img:
        if c % 7 == 0 and c != 0:
            f.write('\n')
        f.write(str(pixel) + ' ')
        c = c + 1

    f.close()


if __name__ == '__main__':
    arg = sys.argv[1]
    image2binary(arg)
