#include "filter.h"

int main(int argc, char **argv) {

	path imagesFolderPath(argv[1]);
	path outputFolderPath(argv[2]);

	int orignalImagePixel = 0;
	int thresholdImagePixel = 0;

	directory_iterator end_itr;
	directory_iterator itr(imagesFolderPath);
	for (directory_iterator itr(imagesFolderPath); itr != end_itr; ++itr) {
		string bmpCheck = itr->path().extension().string();
		if (bmpCheck == ".bmp" || bmpCheck == ".BMP") {

			Mat image = imread(itr->path().string(), IMREAD_GRAYSCALE);
			Mat thresholdImage = Mat::zeros(Size(image.cols, image.rows), CV_8UC1);
			// ------------------------- thresholding the orignal image--------------------------------
			thresholdImageProcess(thresholdImage, image);

			// -----------------------creating region index values -------------------------------------
			Mat regionIndexImage = Mat::zeros(Size(thresholdImage.cols, thresholdImage.rows), CV_8UC1);

			int regionIndex = 1;
			int regionIndexPixel;
			vector<Region> vectorOfStructs;

			//---------------------------------storing region index image in vector<Region>---------------------
			thresholdFunction(thresholdImage, regionIndexImage, vectorOfStructs, thresholdImagePixel, regionIndex);

			//-----------------------------------------Threshold image saving part ---------------------------------------
			string thresholdOutputFile = outputFolderPath.string();
			thresholdOutputFile.append("/BIN_");
			thresholdOutputFile.append(itr->path().filename().string());
			imwrite(thresholdOutputFile, thresholdImage);

			// -------------------------------Text file part---------------------------------------------
			string outputText = outputFolderPath.string();
			outputText.append("/");
			outputText.append(itr->path().filename().string());
			outputText.append(".txt");

			std::ofstream outputTextFile;
			outputTextFile.open(outputText);
			outputTextFile << vectorOfStructs.size() << endl;
			std::sort(vectorOfStructs.begin(), vectorOfStructs.end(), compareRegionsCnt);
			for (int i = 0; i < vectorOfStructs.size(); i++) {
				outputTextFile << vectorOfStructs[i].cnt << endl;
			}
			outputTextFile.close();

			// ---------------------------   assign colourIndex values ---------------------------

			assignColourIndex(vectorOfStructs);

			std::sort(vectorOfStructs.begin(), vectorOfStructs.end(), compareRegionIndex);

			//--------------------------------colour image creation-----------------
			Mat colorImage = Mat::zeros(cv::Size(regionIndexImage.cols, regionIndexImage.rows), CV_8UC3);

			//--------------------------------fill colour image ----------------------
			fillColorImage(colorImage, regionIndexImage, vectorOfStructs);

			// -------------------------- save colour image --------------------------------
			string outputColorFile = outputFolderPath.string();
			outputColorFile.append("/SEG_");
			outputColorFile.append(itr->path().filename().string());
			imwrite(outputColorFile, colorImage);
		}
	}
	return 0;
}