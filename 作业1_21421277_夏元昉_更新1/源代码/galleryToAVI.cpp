#include <opencv2\opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <cstring>
#include <stdlib.h>
#include <tchar.h> 
#include <strsafe.h>
#pragma comment(lib, "User32.lib")

using namespace cv;
using namespace std;

/*void TcharToChar (const TCHAR * tchar, char * _char);*/

vector<Mat> read_dir(vector<string> inputFileNameList,string basePath){
	/***
	Read pictures from given dir, resize and return as a vector<mat>
	Input: a vector container of Pictures' absolute path
	Output: a vector container of Pictures
	***/
	vector<Mat> vPic;
	int numberOfPicsInDir=inputFileNameList.size();
	if (numberOfPicsInDir==0){
		cerr<<"Error occurred while reading directory: "<<basePath<<endl;
		cerr<<"Empty directory"<<endl;
		return vPic;
	}
	for (int i=0;i<numberOfPicsInDir;i++){
		Mat img = imread(inputFileNameList[i]);
		if(img.empty()){
			cerr<<"Error occurred while reading picture:"<<inputFileNameList[i]<<endl;
			cerr<<"Picture skipped."<<endl;
		}
		else{
			//cout<<img.cols<<" "<<img.rows<<endl;
		    Mat img_after_resize;
			resize(img,img_after_resize,Size(640,480),1.0,1.0,INTER_AREA);
			vPic.push_back(img_after_resize);
		}
	}
	return vPic;
}

int main(int argc, TCHAR *argv[]){
	WIN32_FIND_DATA ffd;
	//LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind=INVALID_HANDLE_VALUE;
	DWORD dwError=0;

	//Check argument length. If wrong, print usage.
	if(argc !=4){
		_tprintf(TEXT("\nUsage: %s <directory name> <delay> <AVI name.avi>"),argv[0]);
		return (-1);
	}

	//Check that the input path plus 3 is not longer than MAX_PATH
	StringCchLength(argv[1], MAX_PATH, &length_of_arg);
	if (length_of_arg>(MAX_PATH -3)){
		_tprintf(TEXT("\nDirectory path is too long.\n"));
		return (-1);
	}

	//Check that the input delay is a valid integer
	/*
	if (argv[2]){


	}
	*/

	//Check that the input AVI name is a valid string
	/*
	if (argv[3])
	{


	}
	*/


	//Output the parameter input into the system
	_tprintf(TEXT("\nSource directory is %s\n"),argv[1]);
	_tprintf(TEXT("\nDelay is set to %s\n"),argv[2]);
	_tprintf(TEXT("\nTarget AVI is %s\n\n"),argv[3]);

	StringCchCopy(szDir, MAX_PATH, argv[1]);
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
	
	string sPath;
	TCHAR pathBuffer[MAX_PATH];
	StringCchCopy(pathBuffer,MAX_PATH,argv[1]);
	sPath=pathBuffer;

	int delay=1;
	TCHAR delayBuffer[MAX_PATH];
	StringCchCopy(delayBuffer,MAX_PATH,argv[2]);
	string sDelay=delayBuffer;
	delay=atoi(sDelay.c_str());
	if (delay<1){
		cout<<"Delay must be bigger than 1"<<endl;
		return -1;
	}

	hFind = FindFirstFile(szDir, &ffd);
    if (INVALID_HANDLE_VALUE == hFind){
		return -2;
	} 
   
    // List all the files in the directory with some info about them.
	vector<string> allFilesInDir;
    do{
       if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
		  //Which means it is a directory, so we skip here
		   continue;
       }
       else{
		  string sFilename;
		  string sFilenameAndPath;
		  sFilename=ffd.cFileName;
		  sFilenameAndPath=sPath+"\\"+sFilename;
		  allFilesInDir.push_back(sFilenameAndPath);
       }
    }
    while (FindNextFile(hFind, &ffd) != 0);
    FindClose(hFind);

	vector<Mat> pictures=read_dir(allFilesInDir,sPath);

	//Create Video
	//Init VideoWriter Instance
	//C++: VideoWriter::VideoWriter(const string& filename, int fourcc, double fps, Size frameSize, boolisColor=true)
	string sOutputFileName="Sample Output.avi";
	sOutputFileName=argv[3];
	const char* constCOutputFileName=sOutputFileName.c_str();
	Size outputVideoSize=Size(640,480); //Set default otutput Resolution 640*480;
	VideoWriter outputVideo=VideoWriter(constCOutputFileName,CV_FOURCC('M','J','P','G'),60.0,outputVideoSize,TRUE);
	//int count_of_frames=60*(pictures.size()+1);
	int count_of_frames=60*(pictures.size())*delay;
	//cout<< count_of_frames<<endl;

	for (int i=0;i<pictures.size();i++){
		Mat img_at_this_frame=pictures.at(i);
		int picIndex=0;
		while (picIndex<60*delay){
			if ( (i==0) && (picIndex<=10) ){
				putText(img_at_this_frame,"21421277",Point(100,50),FONT_ITALIC,2,Scalar(0,0,0),3);
				putText(img_at_this_frame,"Yuanfang Xia",Point(100,200),FONT_ITALIC,2,Scalar(0,0,0),3);
			}
			//cout<<picIndex/(60*delay)<<endl;
			outputVideo<<img_at_this_frame;
			picIndex=picIndex+1;
		}
		if(i<pictures.size()-1){
			int j=0;
			while(j<60){
				Mat change = pictures.at(i+1)-pictures.at(i);
				Mat thisFrame = pictures.at(i);
				thisFrame=thisFrame + change/60;
				outputVideo<<thisFrame;
				j++;
			}
		}

	}
	cout<<"Writing Finished\n"<<endl;
	////system("pause");
	return 0;
}
