# image-threshold-and-flood-fill

This project will implement code :

- load images from a directory
- thresholds each image with T = 30
- perform flood-filling (8-neighbors) to determine the regions in the image
- sorts the regions by the number of pixels in each region
- creates an image that shows different colors for each region (color determined by sorting the regions by pixel count)

Then saves the following files into another directory

- thresholded image
- text file with total number of regions and pixel counts per region (sorted)
- recolored region image


This project will be entirely with UCHAR images which are grayscale and color images.
