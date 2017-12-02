#!/usr/bin/env python
import sys

import numpy as np
import cv2
"""
    Removes the given seam from the image.
    
    image : an n x m array (may have multiple channels)
    seam : an n x 1 array of X-coordinates defining the seam pixels in top-down order.
    
    Thus, seam[0] means remove pixel (0, seam[0]) from the input image.
    
    returns: an n x (m - 1) image with the seam removed.
"""
def remove_vertical_seam(image, seam):
	result = np.zeros((image.shape[0], image.shape[1]-1, image.shape[2]))
	for i in range(0,image.shape[0]):
		result[i] = np.delete(image[i], seam[i],0)
	return result
   
"""
    Returns the L1 gradient magnitude of the given image.
    The result is an n x m numpy array of floating point values,
    where n is the number of rows in the image and m is the number of columns.
"""
def gradient_magnitude(image):
   # First, convert the input image to a 32-bit floating point grayscale.
   # Be sure to scale it such that the intensity varies between 0 and 1.
	gray_image = cv2.cvtColor(image.astype('float32'), cv2.COLOR_BGR2GRAY)
	gray_image = cv2.normalize(gray_image, alpha=0, beta=1, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_32F)
	 # Next, compute the graient in the x direction using the sobel operator with a kernel size of 3.
	ddepth = cv2.CV_32F
	grad_x = cv2.Sobel(gray_image, ddepth,1, 0,ksize=3)
	# Be sure to normalize the image so it's values are between -1 and 1
	cv2.normalize(grad_x, alpha=-1, beta=1, norm_type=cv2.NORM_MINMAX,
dtype=cv2.CV_32F)
	 # Compute the graient in the y direction using the same kernel size and normalization
	grad_y = cv2.Sobel(gray_image, ddepth, 0, 1, ksize=3)
	cv2.normalize(grad_y, alpha=-1, beta=1, norm_type=cv2.NORM_MINMAX,
dtype=cv2.CV_32F) 
	# Finally, compute the l1 norm of the x and y gradients at each pixel value.
    # The l1 norm is the sum of their absolute values.
	result = np.zeros((gray_image.shape[0], gray_image.shape[1]))
	for i in range(0, gray_image.shape[0]):
		for j in range(0, gray_image.shape[1]):
			result[i][j] = (np.absolute(grad_y[i][j]) + np.absolute(grad_x[i][j])) / 4.0
	# convert the final image from a double-precision floating point to single.
	result = result.astype(np.float32)
	# and return the result
	return result

"""
    Computes the cumulative minimum energy of every possible seam in the provided energy image.
    You can do this using the dynamic programming rule:
         M(i, j) = e(i, j) + min( M(i-1, j-1), M(i-1, j), M(i-1, j+1) 
    
    energy : an n x m single channel image defining the energy at each pixel.
    returns : an n x m image containing the cumulative minimum energy cost of all seams through each pixel.
"""
def compute_seam_costs(energy):
   # Create M, an n x m matrix with the first row equal to energy.
	n = energy.shape[0]
	m = energy.shape[1]
	M = np.zeros((n,m))
	M[0] = energy[0] 
	# Iterate over the rows, starting at row 1
      # Iterate over the column 1 to m - 1
	for i in range(1,n):
		for j in range(0, m):
      # Compute M(i, j) = e(i, j) + min( M(i-1, j-1), M(i-1, j), M(i-1, j+1)
			if (j==0):
				M[i][j] = energy[i][j] + min(M[i-1][j],M[i-1][j+1])
			elif(j == m-1):
				M[i][j] = energy[i][j] + min(M[i-1][j], M[i-1][j-1])
			else:
				M[i][j] = energy[i][j] + min(M[i-1][j-1], M[i-1][j],M[i-1][j+1])        
	# be sure to handle the special cases where j = 0 and j = m -1
	# return the result!
	return M

"""
    Find the seam with minimal energy cost given the provided seam cost
    matrix M. Returns the X-coordinates of the minimal-cost vertical seam in
    top-down order.
    
    M: the output from compute_seam_costs.
    
    return: a list of x-coordinates starting at row 0 of M containing the ones to remove.
"""
def minimal_seam(M):
   # Compute the bottom-up path of pixel X-coordinates for the seam with
   # minimal cost.
	n = M.shape[0]
	m = M.shape[1]
	l = []
	l.append(np.where(M[n-1] == min(M[n-1]))[0][0])
	for i in range(n-1,0,-1):
		j = l[0]
		if (j==0):
			l.insert(0,np.where(M[i-1] == min(M[i-1][j], M[i-1][j+1]))[0][0])
		elif (j== m-1):
			l.insert(0,np.where(M[i-1] == min(M[i-1][j],M[i-1][j-1]))[0][0])
		else:
			l.insert(0,np.where(M[i-1] == min(M[i-1][j], M[i-1][j-1],
M[i-1][j+1]))[0][0])
	# Return the top-down seam X-coordinates and the total energy cost of
   # removing that seam.
	energy_cost = 0
	for i in range(0,n):
		energy_cost += M[i][l[i]]
	return l, energy_cost

"""
    Compute the optimal order of horizontal and vertical seam removals to
    achieve the given target image size. Order should be returned as a list of
    0 or 1 values corresponding to horizontal and vertical seams
    respectively.
"""
def compute_ordering(image, target_size):    
	r = image.shape[0] - target_size[0] + 1
	c = image.shape[1] - target_size[1] + 1
	if r < 0 or c < 0:
		raise ValueError("Target size must be smaller than the input size.")
	return [0,1] * min(r-1, c-1) + [0] * max(r-c, 0) + [1] * max(c-r, 0)

def resize(image, target_size):
	output = image.copy()
	order = compute_ordering(output, target_size)
	for i, seam_type in enumerate(order):
		print "Removing seam {} / {} ".format(i, len(order))
        
        # check if order = 0, if so, transpose the image!
		if(order[i] == 0):
			(h, w) = output.shape[:2]
			center = (w/2, h/2)
			t = cv2.getRotationMatrix2D(center, 90, 1.0)
			output = cv2.warpAffine(output, t, (w,h))
        # compute the energy using gradient_magnitude
		grad = gradient_magnitude(output)
        # Compute M using compute_seam_costs
		M = compute_seam_costs(grad)
        # get the minimal seam using 'minimal_seam'
		min_seam,cost = minimal_seam(M)
        # remove it using 'remove_vertical_seam'
		output = remove_vertical_seam(output, min_seam)
        # check if order = 0, if so, transpose the image back!
		if(order[i]==0):
			(h, w) = output.shape[:2]
			center = (w/2, h/2)
			t = cv2.getRotationMatrix2D(center, 270, 1.0)
			output = cv2.warpAffine(output, t, (w,h))
	# Sanity check.....
	#assert(output.shape[0] == target_size[0] and output.shape[1] == target_size[1])
	# return results...
	return output

def runDebug(image, target_size):
    # compute and save a gradient image
   energy = gradient_magnitude(image.copy()) / 2.0
   cv2.imwrite("debug-energy.png", energy * 255)
   print "energy min max {} {}".format(np.min(energy.flatten()), np.max(energy.flatten()))
    
    # compute and save the cost image
   cost = np.log(compute_seam_costs(energy) + 1.0)
   cv2.imwrite("debug-cost.png", cost * 255)
    
    # get the minimal seam using 'minimal_seam'
   seam, seamCost = minimal_seam(cost)
   print "The minimal seam has cost {}".format(seamCost)
   print "The first seam is {}".format(seam)
    
    # remove it using 'remove_vertical_seam'
   output = remove_vertical_seam(image.copy(), seam)
   cv2.imwrite("debug-first-seam-gone.png", output)

if __name__ == "__main__":
	debug = (len(sys.argv) > 5 and sys.argv[5] == "-debug")
	try:
		in_fn, w, h, out_fn = sys.argv[1:5]
		h, w = int(h), int(w)
	except:
		print("Usage: python hw1.py FILE TARGET_WIDTH TARGET_HEIGHT OUTPUT")
		exit(1)
	image = cv2.imread(in_fn)
	if (debug):
		runDebug(image, (h, w))
	else:
		resized = resize(image, (h,w))
		cv2.imwrite(out_fn, resized)
