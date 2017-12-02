#!/usr/bin/env python
import sys

import numpy as np
import cv2

def logImpl(img):
    # return the log of the image
    # Since log(0) is undefined, add 1 before performing the operation.
    # Be sure to normalize such that the maximum value is 1
    
    
def powerLaw(img, c, y):

def process(image, operation, args):
    
    # First, determine the input channel.
    
        # If the image has 3 channels, assume it is BGR, convert it to HSV, and operate on the value channel.
        # Use http://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_imgproc/py_colorspaces/py_colorspaces.html to guide you here.
        
        # If the image has 1 channel, just operate on it directly
        
    # Convert the inputChannel to be 32-bit floating point representation, and scale it such
    # that the values are between 0 and 1
    
    print "input from %f to %f " % (np.min(inputChannel.flatten()), np.max(inputChannel.flatten()))
    
    if (operation == "log"):
        # For the log operation, implement and call logImpl above
        
    elif (operation == "powerlaw"):
        # Parse c and y from the argument list, and convert them to floats
        
        # Call powerLaw
        
    elif (operation == "ilog"):
        # the inverse log is log(1 - inputChannel)
        
    elif (operation == "negative"):
        # What is the negative? Should be easy to code :D
        
    elif (operation == "npow"):
        # Parse n from the argument list and convert it to a float.
        
        # compute inputChannel to the power of x. 
        
    elif (operation == "nroot"):
        # Nth rooth is the same as npow(1.0/n)
        
    else:
        raise Exception('Unknown operation {}'.format(operation))
        
    print "result from %f to %f " % (np.min(resultChannel.flatten()), np.max(resultChannel.flatten()))
        
        
    # Convert the resultChannel to be 8-bit unsigned representation, and scale it such
    # that the values are between 0 and 255
    
    
    # Construct the result image
    
        # If the image had 3 channels, replace the V channel of the HSV image with the result channel,
        # and convert it back to BGR
        
        # If the image has 1 channel, just return it
        
    
    return resultImage

if __name__ == "__main__":
    try:
        inputFileName, outputFileName, operation = sys.argv[1:4]
    except:
        print("Usage: python hw0.py inputFile outputFile operation ")
        exit(1)

    image = cv2.imread(inputFileName)
    result = process(image, operation, sys.argv[4:])
    cv2.imwrite(outputFileName, result)
