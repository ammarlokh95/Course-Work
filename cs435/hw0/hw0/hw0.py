#!/usr/bin/env python
import sys

import numpy as np
import cv2

def logImpl(img):
    # return the log of the image
    # Since log(0) is undefined, add 1 before performing the operation.
    # Be sure to normalize such that the maximum value is 1
	temp = img + 1
	cv2.log(temp,temp)	 
	temp *= 1.44269504 # to scale everything to 0 to 1   
	return temp

def powerLaw(img, c, y):
	return (c * cv2.pow(img, y, img))
 
def process(image, operation, args):
    
    # First, determine the input channel.
	if(len(image.shape) == 3 and image.shape[2] == 3):
		# If the image has 3 channels, assume it is BGR, convert it to HSV, and operate on the value channel.
		image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
		inputChannel = image[:,:,2]    
      
        # If the image has 1 channel, just operate on it directly
	else: 
		inputChannel = image      
    # Convert the inputChannel to be 32-bit floating point representation, and scale it such
    # that the values are between 0 and 1
	inputChannel = inputChannel.astype('float32')
	inputChannel /= 255  
	print "input from %f to %f " % (np.min(inputChannel.flatten()), np.max(inputChannel.flatten()))
    
	if (operation == "log"):
		# Finds the log of the inputchannel
		resultChannel = logImpl(inputChannel)	   
     	
	elif (operation == "powerlaw"):
        # Parse c and y from the argument list, and convert them to floats
		c = float(args[0])
		y = float(args[1])
		
	# Call powerLaw applies resultchannel = c * inputchannel ^ y
        	resultChannel = powerLaw(inputChannel,c ,y)
	elif (operation == "ilog"):
        # the inverse log is e^inputchannel (?)
		resultChannel = (cv2.exp(inputChannel)) / 2.718281828 # to scale from 0 to 1     
	elif (operation == "negative"):
	# gets the nagative of the values of the image
		resultChannel = 1 - inputChannel      
	
	elif (operation == "npow"):
      # Parse n from the argument list and convert it to a float.
		n = float(args[0])
		resultChannel = powerLaw(inputChannel, 1.0, n)        
        # compute inputChannel to the power of x ('n'?) 
        
	elif (operation == "nroot"):
        # Nth rooth is the same as npow(1.0/n)
		n = float(args[0])
		resultChannel = powerLaw(inputChannel, 1.0, 1.0/n)		    
	else:
		raise Exception('Unknown operation {}'.format(operation))
    
	print "result from %f to %f " % (np.min(resultChannel.flatten()), np.max(resultChannel.flatten()))
        
	        
    # Convert the resultChannel to be 8-bit unsigned representation, and scale it such
    # that the values are between 0 and 255
	resultChannel *= 255
	np.around(resultChannel) # to ensure the floats are properly rounded off
	resultChannel = resultChannel.astype('uint8') 
    
    # Construct the result image
	if(len(image.shape) == 3 and image.shape[2] == 3 ): 
        # If the image had 3 channels, replace the V channel of the HSV image with the result channel,
        # and convert it back to BGR
		resultImage = image
		resultImage[:,:,2] = resultChannel 
        
		resultImage = cv2.cvtColor(resultImage, cv2.COLOR_HSV2BGR)  
	# If the image has 1 channel, just return it
	else: 
		resultImage = resultChannel
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
