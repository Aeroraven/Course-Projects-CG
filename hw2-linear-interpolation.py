import numpy as np
import cv2 as cv
import albumentations as albu
import matplotlib.pyplot as plt

def transforms():
    transform_list = [
        albu.Resize(290, 290)
    ]
    return albu.Compose(transform_list)


src = cv.imread("cat.png")
dst = cv.imread("tiger.png")
src = transforms()(image=src)['image']
dst = transforms()(image=dst)['image']

# Linear Interpolation
INTERPOLATION_STEPS = 15
for i in range(INTERPOLATION_STEPS+1):
    ins = np.zeros_like(src)
    for j in range(290):
        for k in range(290):
            for l in range(3):
                ins[j][k][l] = dst[j][k][l]*i/INTERPOLATION_STEPS+src[j][k][l]*(INTERPOLATION_STEPS-i)/INTERPOLATION_STEPS
    cv.imwrite("Linear-interpolation-"+str(i)+".jpg",ins)
