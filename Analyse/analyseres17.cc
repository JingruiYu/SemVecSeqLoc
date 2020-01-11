#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>


void getGt2Res(cv::Mat &gtMat,cv::Mat &resMat,int resNum)
{
    cv::Mat gtAllMat = cv::Mat::zeros(203,6,CV_32FC1);
    std::string gtMatPath = "/home/yujr/openslam_openseqslam/results/groundTruth.txt";
    std::ifstream gt(gtMatPath.c_str());
    for (size_t i = 0; i < gtAllMat.rows; i++)
    {
        for (size_t j = 0; j < gtAllMat.cols; j++)
        {
            int t;
            gt >> t;
            gtAllMat.at<float>(i,j) = (float)t;
        } 
    }

    gtAllMat.col(0).copyTo(gtMat.col(0));
    gtAllMat.col(5).copyTo(gtMat.col(1));       // modify

    std::vector<cv::String> ResMatList;
    std::string resMatsPath = "/home/yujr/openslam_openseqslam/results/res1-10/5_520_410/";      // modify
    cv::String resPath = resMatsPath + "*.txt";
    cv::glob(resPath,ResMatList,true);

    std::ofstream GTfout("/home/yujr/openslam_openseqslam/Analyse/ResMat5_520_410.txt");
    if (!GTfout) 
    { 
        std::cout << "File Not Opened" << std::endl; 
    }

    if (ResMatList.size() == resNum)
    {
        for (size_t k = 0; k < resNum; k++)
        {
            std::string resKPath = ResMatList[k];
            GTfout << k << " : " << resKPath << std::endl;
            std::ifstream res(resKPath.c_str());
            
            size_t j = 0;
            for (size_t i = 0; i < 2*resMat.rows; i++)
            {
                int t;
                res >> t;
                if (i%2 == 1)
                {
                    resMat.at<float>(j++,k) = (float)(521-t); // modify
                }          
            }
        }
    }  

    GTfout << std::endl << std::endl; 

    for (size_t i = 0; i < resMat.rows; i++)
    {
        for (size_t j = 0; j < resMat.cols; j++)
        {
            GTfout << resMat.at<float>(i,j) << " "; 
        }
        GTfout << std::endl; 
    }

    GTfout.close();
}

void chekRes(cv::Mat &chekMat,cv::Mat &gtMat,cv::Mat &resMat)
{
    for (size_t i = 0; i < chekMat.rows; i++)
    {
        int flag = 0;
        for (size_t j = 0; j < chekMat.cols; j++)
        {
            int res = resMat.at<float>(i,j);
                        
            for (size_t k = 1; k < gtMat.cols; k++)
            {
                if (abs(res - gtMat.at<float>(i,k)) < 3 && gtMat.at<float>(i,k) != 0 && res !=0 )
                {
                    chekMat.at<float>(i,j) = 5;
                    flag = 1;
                }
            }
            
            if (flag == 0)
            {
                for (size_t k = 1; k < gtMat.cols; k++)
                {
                    if (i-1 >= 0)
                    {
                        if (abs(res - gtMat.at<float>(i-1,k)) < 3 &&
                            gtMat.at<float>(i-1,k) != 0 && res !=0)
                        {
                            chekMat.at<float>(i,j) = 4;
                            flag = 1;
                        }
                    }

                    if (flag == 0)
                    {
                        if (abs(res - gtMat.at<float>(i+1,k)) < 3 &&
                            gtMat.at<float>(i+1,k) != 0 && res !=0)
                        {
                            chekMat.at<float>(i,j) = 4;
                            flag = 1;
                        }
                    } 
                }
            }

            if (flag == 0)
            {
                for (size_t k = 1; k < gtMat.cols; k++)
                {
                    if (i-2 >= 0)
                    {
                        if (abs(res - gtMat.at<float>(i-2,k)) < 3 && 
                            gtMat.at<float>(i-2,k) != 0 && res !=0)
                        {
                            chekMat.at<float>(i,j) = 3;
                            flag = 1;
                        }
                    }

                    if (flag == 0)
                    {
                        if (abs(res - gtMat.at<float>(i+2,k)) < 3 &&
                            gtMat.at<float>(i+2,k) != 0 && res !=0)
                        {
                            chekMat.at<float>(i,j) = 3;
                            flag = 1;
                        }
                    } 
                }
            }
            
            if (flag == 0)
            {
                chekMat.at<float>(i,j) = -1;
            }
        }  
    }
}

void savechekMat(cv::Mat &chekMat)
{
    std::ofstream GTfout("/home/yujr/openslam_openseqslam/Analyse/checkMat5_520_410.txt");

    if (!GTfout) 
    { 
        std::cout << "File Not Opened" << std::endl; 
    }

    for (size_t i = 0; i < chekMat.rows; i++)
    {
        for (size_t j = 0; j < chekMat.cols; j++)
        {
            GTfout << chekMat.at<float>(i,j) << " "; 
        }
        GTfout << std::endl; 
    }
    GTfout.close();
}

int main(int argc, char const *argv[])
{
    int resNum = 4;     // modify
    cv::Mat gtMat = cv::Mat::zeros(203,2,CV_32FC1);     // modify
    cv::Mat resMat = cv::Mat::zeros(203,resNum,CV_32FC1);

    getGt2Res(gtMat,resMat,resNum);

    cv::Mat chekMat = cv::Mat::zeros(203,resNum,CV_32FC1);

    chekRes(chekMat,gtMat,resMat);

    savechekMat(chekMat);

    return 0;
}
