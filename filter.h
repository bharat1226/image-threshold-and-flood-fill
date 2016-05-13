#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


using namespace boost::filesystem;
using namespace cv;
using namespace std;

struct Region {
	int regionIndex;
	int cnt;
	int colorIndex;
};

void floodFill(int i, int j, Mat& regionIndexImage, Mat thresholdImage, Region& regionStruct);
bool compareRegionsCnt(Region &a, Region &b);
bool compareRegionIndex(Region &a, Region &b);
void thresholdFunction(Mat thresholdImage, Mat& regionIndexImage, vector<Region>& vectorOfStructs, int thresholdImagePixel, int regionIndex);
void thresholdImageProcess(Mat& thresholdImage, Mat image);
void assignColourIndex(vector<Region>& vectorOfStructs);
void fillColorImage(Mat& colorImage, Mat regionIndexImage, vector<Region> vectorOfStructs);

bool compareRegionsCnt(Region &a, Region &b) {
	return(a.cnt > b.cnt);
}

bool compareRegionIndex(Region &a, Region &b) {
	return(a.regionIndex < b.regionIndex);
}

void thresholdFunction(Mat thresholdImage, Mat& regionIndexImage, vector<Region>& vectorOfStructs, int thresholdImagePixel, int regionIndex) {
	for (int i = 1; i < thresholdImage.rows - 1; i++) {
		for (int j = 1; j < thresholdImage.cols - 1; j++) {
			thresholdImagePixel = thresholdImage.at<uchar>(i, j);
			int regionIndexPixel = regionIndexImage.at<uchar>(i, j);
			if (thresholdImagePixel == 255 && regionIndexPixel == 0) {
				Region regionStruct;
				regionStruct.regionIndex = regionIndex;
				regionStruct.cnt = 0;
				regionStruct.colorIndex = 0;
				floodFill(i, j, regionIndexImage, thresholdImage, regionStruct);
				regionIndex++;
				vectorOfStructs.push_back(regionStruct);
			}
		}
	}
}

void floodFill(int i, int j, Mat& regionIndexImage, Mat thresholdImage, Region& regionStruct) {
	int regionIndex = regionStruct.regionIndex;
	regionIndexImage.at<uchar>(i, j) = regionIndex;
	int regionIndexPixel, thresholdImagePixel;
	int count = regionStruct.cnt;
	count = count + 1;
	regionStruct.cnt = count;

	// ------------- WHEN I USED FUNCTION FOR THIS PART, THERE IS STACK OVERFLOW-------- SO I DID NOT USE FUNCTION HERE

	if ((i - 1 >= 0) && (j - 1 >= 0) && (i - 1  < thresholdImage.rows) && (j - 1  < thresholdImage.cols)) {
		regionIndexPixel = regionIndexImage.at<uchar>(i - 1, j - 1);
		thresholdImagePixel = thresholdImage.at<uchar>(i - 1, j - 1);
		if ((regionIndexPixel == 0) && (thresholdImagePixel == 255)) {
			regionIndexImage.at<uchar>(i - 1, j - 1) = regionIndex;
			floodFill(i - 1, j - 1, regionIndexImage, thresholdImage, regionStruct);
		}
	}

	if ((i - 1 >= 0) && (j >= 0) && (i - 1  < thresholdImage.rows) && (j  < thresholdImage.cols)) {
		regionIndexPixel = regionIndexImage.at<uchar>(i - 1, j);
		thresholdImagePixel = thresholdImage.at<uchar>(i - 1, j);
		if ((regionIndexPixel == 0) && (thresholdImagePixel == 255)) {
			regionIndexImage.at<uchar>(i - 1, j) = regionIndex;
			floodFill(i - 1, j, regionIndexImage, thresholdImage, regionStruct);
		}
	}

	if ((i - 1 >= 0) && (j + 1 >= 0) && (i - 1  < thresholdImage.rows) && (j + 1  < thresholdImage.cols)) {
		regionIndexPixel = regionIndexImage.at<uchar>(i - 1, j + 1);
		thresholdImagePixel = thresholdImage.at<uchar>(i - 1, j + 1);
		if ((regionIndexPixel == 0) && (thresholdImagePixel == 255)) {
			regionIndexImage.at<uchar>(i - 1, j + 1) = regionIndex;
			floodFill(i - 1, j + 1, regionIndexImage, thresholdImage, regionStruct);
		}
	}

	if ((i >= 0) && (j - 1 >= 0) && (i   < thresholdImage.rows) && (j - 1  < thresholdImage.cols)) {
		regionIndexPixel = regionIndexImage.at<uchar>(i, j - 1);
		thresholdImagePixel = thresholdImage.at<uchar>(i, j - 1);
		if ((regionIndexPixel == 0) && (thresholdImagePixel == 255)) {
			regionIndexImage.at<uchar>(i, j - 1) = regionIndex;
			floodFill(i, j - 1, regionIndexImage, thresholdImage, regionStruct);
		}
	}

	if ((i >= 0) && (j + 1 >= 0) && (i   < thresholdImage.rows) && (j + 1  < thresholdImage.cols)) {
		regionIndexPixel = regionIndexImage.at<uchar>(i, j + 1);
		thresholdImagePixel = thresholdImage.at<uchar>(i, j + 1);
		if ((regionIndexPixel == 0) && (thresholdImagePixel == 255)) {
			regionIndexImage.at<uchar>(i, j + 1) = regionIndex;
			floodFill(i, j + 1, regionIndexImage, thresholdImage, regionStruct);
		}
	}

	if ((i + 1 >= 0) && (j - 1 >= 0) && (i + 1  < thresholdImage.rows) && (j - 1  < thresholdImage.cols)) {
		regionIndexPixel = regionIndexImage.at<uchar>(i + 1, j - 1);
		thresholdImagePixel = thresholdImage.at<uchar>(i + 1, j - 1);
		if ((regionIndexPixel == 0) && (thresholdImagePixel == 255)) {
			regionIndexImage.at<uchar>(i + 1, j - 1) = regionIndex;
			floodFill(i + 1, j - 1, regionIndexImage, thresholdImage, regionStruct);
		}
	}

	if ((i + 1 >= 0) && (j >= 0) && (i + 1  < thresholdImage.rows) && (j  < thresholdImage.cols)) {
		regionIndexPixel = regionIndexImage.at<uchar>(i + 1, j);
		thresholdImagePixel = thresholdImage.at<uchar>(i + 1, j);
		if ((regionIndexPixel == 0) && (thresholdImagePixel == 255)) {
			regionIndexImage.at<uchar>(i + 1, j) = regionIndex;
			floodFill(i + 1, j, regionIndexImage, thresholdImage, regionStruct);
		}
	}

	if ((i + 1 >= 0) && (j + 1 >= 0) && (i + 1  < thresholdImage.rows) && (j + 1  < thresholdImage.cols)) {
		regionIndexPixel = regionIndexImage.at<uchar>(i + 1, j + 1);
		thresholdImagePixel = thresholdImage.at<uchar>(i + 1, j + 1);
		if ((regionIndexPixel == 0) && (thresholdImagePixel == 255)) {
			regionIndexImage.at<uchar>(i + 1, j + 1) = regionIndex;
			floodFill(i + 1, j + 1, regionIndexImage, thresholdImage, regionStruct);
		}
	}
}

void thresholdImageProcess(Mat& thresholdImage, Mat image) {
	int orignalImagePixel = 0;
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			orignalImagePixel = image.at<uchar>(i, j);
			if (orignalImagePixel >= 130) {
				thresholdImage.at<uchar>(i, j) = 255;
			}
			else {
				thresholdImage.at<uchar>(i, j) = 0;
			}
		}
	}
}

void assignColourIndex(vector<Region>& vectorOfStructs) {
	int temp = 0;
	for (int i = 0; i < vectorOfStructs.size(); i++) {
		if (temp == 6) {
			temp = 0;
		}
		if (temp == 0) {
			vectorOfStructs[i].colorIndex = temp;
		}
		else if (temp == 1) {
			vectorOfStructs[i].colorIndex = temp;
		}
		else if (temp == 2) {
			vectorOfStructs[i].colorIndex = temp;
		}
		else if (temp == 3) {
			vectorOfStructs[i].colorIndex = temp;
		}
		else if (temp == 4) {
			vectorOfStructs[i].colorIndex = temp;
		}
		else if (temp == 5) {
			vectorOfStructs[i].colorIndex = temp;
		}
		temp++;
	}
}

void fillColorImage(Mat& colorImage, Mat regionIndexImage, vector<Region> vectorOfStructs) {
	int colorIndexValue;
	int regionIndexPixel;
	for (int i = 0; i < regionIndexImage.rows - 1; i++) {
		for (int j = 0; j < regionIndexImage.cols - 1; j++) {
			regionIndexPixel = regionIndexImage.at<uchar>(i, j);
			if (regionIndexPixel != 0) {
				for (int k = 0; k < vectorOfStructs.size(); k++) {
					if ((k + 1) == regionIndexPixel) {
						if (vectorOfStructs[k].colorIndex == 0) {
							colorImage.at<Vec3b>(i, j) = Vec3b(0, 0, 255);
						}
						if (vectorOfStructs[k].colorIndex == 1) {
							colorImage.at<Vec3b>(i, j) = Vec3b(0, 255, 0);
						}
						if (vectorOfStructs[k].colorIndex == 2) {
							colorImage.at<Vec3b>(i, j) = Vec3b(255, 0, 0);
						}
						if (vectorOfStructs[k].colorIndex == 3) {
							colorImage.at<Vec3b>(i, j) = Vec3b(0, 255, 255);
						}
						if (vectorOfStructs[k].colorIndex == 4) {
							colorImage.at<Vec3b>(i, j) = Vec3b(255, 0, 255);
						}
						if (vectorOfStructs[k].colorIndex == 5) {
							colorImage.at<Vec3b>(i, j) = Vec3b(255, 255, 0);
						}
					}
				}
			}
		}
	}
}
