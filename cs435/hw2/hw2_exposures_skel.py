#!/usr/bin/env python

"""combine_exposures -- Combine exposures into a single HDR image.

Output an HDR image by computing the inverse response function for each input image,
using the inverse repsonse function to compute the irradiance of each image,
and perform a weighted average to merge them.
"""

import argparse
import os
import sys

import numpy as np
import scipy.ndimage
import scipy.sparse
import scipy.sparse.linalg

import OpenEXR


# constants for pixel values
ncolors = 256
zmin = 0
zmax = ncolors - 1
zmid = (zmax + zmin) / 2

# compute weight array
weights = np.array([z - zmin if z <= zmid else zmax - z for z in xrange(ncolors)])


def compute_response(imagelist, exposuretimes, channel, npixels, smoothweight, debugFile):
    """Compute and return inverse response function and irradiance values
    
    Uses the method of Debevec and Malik, "Recovering High Dynamic Range Radiance Maps from Photographs"
    """

    # check parameter
	assert len(imagelist) == len(exposuretimes)
	nimages = len(imagelist)

    # initialize matrix and array
	A = scipy.sparse.lil_matrix((nimages * npixels + ncolors - 1, ncolors + npixels))
	b = np.zeros((nimages * npixels + ncolors - 1,))

    # randomize pixels of channel 'channel'
	imageindices = np.random.permutation(np.arange(imagelist[0].size)[channel::3])
    
	# This whole function:
    
	if (debugFile != None):
		dout = open(debugFile, 'w+')
		dout.write("\nimageindices are " + str(imageindices))

    # Add data terms to A and b
    # We'll use k to keep track of which row in A we are on.
	k = 0
    # First iterate over the number of images
        # and then the number of pixels
	for i in range(0, imagelist.shape[1]:
		for j in range(0, imagelist.shape[0]:
			# Extract the pixel value from the image.
			pvalue = imagelist[j,i]	
			# Get the weight for that value from the {{weights}} array
			weight = weights[pvalue]
			# Pupulate the row in A for this pixel value/sample
			A[k, pvalue] = weight
			A[k, ncolors + i] = -weight
			# Populate B with the weight times the log of the exposure time.
			b[k, 0] = weight * np.log(exposuretimes[j])
			# Increment k
			k += 1
    
	if (debugFile != None):
		dout.write("\nb is: " + str(b))
		dout.write("\nAfter Data Term, A is: " + str(A))

	# add smoothness constraint to A, increnent over the number of colors
	for i in range(1, ncolors - 1):    
		# Set the three entries on row k for the smoothness term.
        weight = w[i]
		A[k, i-1] = smoothweight*weight
		A[k, i] = 2*smootheweight*weight
		A[k, i+1] = l*smoothweight
		# increment k
		k += 1

    if (debugFile != None):
        dout.write("\nAfter smoothness constraint, A is: " + str(A))

    # Finally, add constraint g(z_mid) = 1 to A at row k
    A[k, zmid] = 1 
    
    if (debugFile != None):
        dout.write("Final A is: " + str(A))
        dout.close()

    # solve least square system
	result = np.linalg.lstsq(A, b)[0]

	return result[:ncolors], result[ncolors:]


def combine_exposures(imagelist, exposuretimes, npixels, smoothweight, debug):
    """Combine a series of expsures into a single HDR photo"""

    # check parameters
    assert len(imagelist) == len(exposuretimes)
    nimages = len(imagelist)

    final = np.zeros((imagelist[0].shape), dtype=np.float32)
    weightsum = np.zeros((imagelist[0].shape), dtype=np.float32)

    # do each color channel separately
    for c in xrange(3):

        debugFile = None
        if (debug):
            debugFile = "debug-" + str(c) + ".txt"
        
        # compute inverse response function
        responsefunc, radiance = compute_response(imagelist, exposuretimes, c, npixels, smoothweight, debugFile)
        
        # optionally plot the response function (helpful for debugging)
        if (debug):
            import matplotlib.pyplot as plt
            plt.plot(np.arange(len(responsefunc)), responsefunc)
            plt.savefig('channel-' + str(c) + '-response.pdf')
        
        # combine exposures
		for j in xrange(nimages):
            # TODO!!!!
            # now that we have the response function and radiance, compute the final image.
			 
			# Compute the intensity by the weighted average
            # And sum up the weights while you're at 

    # normalize the exposures by dividing by weightsum
    final /= weightsum
    return np.exp(final)


def writeexr(path, image):
    """Write image to path as an EXR file"""
    header = OpenEXR.Header(image.shape[1], image.shape[0])
    out = OpenEXR.OutputFile(path, header)
    out.writePixels({'R': image[:, :, 0].tostring(),
                     'G': image[:, :, 1].tostring(),
                     'B': image[:, :, 2].tostring()})


if __name__ == "__main__":
    description = '\n\n'.join(__doc__.split('\n\n'))
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('imagelist_path', type=str,
                        help="path to a file that lists each image and corresponding shutter speed")
    parser.add_argument('output_file', type=str)    
    parser.add_argument('--npixels', type=int, default=4096,
                        help="Number of pixels used to compute inverse response function")
    parser.add_argument('--smoothweight', type=float, default=256.,
                        help="Weighting on smoothness of response function (i.e., lambda)")
    parser.add_argument('--seed', type=int, default=None,
                        help="seed the rng with this value ")
    parser.add_argument('--debug', action='store_true',
                        help="dump debug data")
    if len(sys.argv) == 1:
        parser.print_help()

    args = parser.parse_args()
    
    if (args.seed != None):
        np.random.seed(args.seed)

    imageinfo = np.genfromtxt(args.imagelist_path, dtype=None)

    imagelist = [scipy.ndimage.imread(os.path.join(os.path.dirname(args.imagelist_path), x)) for x in imageinfo['f0']]
    exposuretimes = imageinfo['f1']

    hdr = combine_exposures(imagelist, exposuretimes, args.npixels, args.smoothweight, args.debug)
    writeexr(args.output_file, hdr)
