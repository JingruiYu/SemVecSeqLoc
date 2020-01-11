#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>


void getResMat( std::string MatPath, cv::Mat &LastRes, cv::Mat &ThisRes, cv::Mat &GTMat,
                std::string DataPath, std::vector<cv::String> &orgNameListmkz,
                std::vector<cv::String> &orgNameListseg)
{
    std::stringstream LastResPath;
    LastResPath << MatPath << "ThisRes.txt";
    std::string LastResPath2 = LastResPath.str();
    std::ifstream res(LastResPath2.c_str());
    for (size_t i = 0; i < 203; i++)
    {
        for (size_t j = 0; j < 6; j++)
        {
            int t;
            res >> t;
            LastRes.at<float>(i,j) = (float)t;
        } 
    }

    std::stringstream ThisResPath;
    ThisResPath << MatPath << "ManVecResultNoNaN.txt";
    std::string ThisResPath2 = ThisResPath.str();
    std::ifstream Tisres(ThisResPath2.c_str());
    for (size_t i = 0; i < 203; i++)
    {
        for (size_t j = 0; j < 2; j++)
        {
            int t;
            Tisres >> t;
            ThisRes.at<float>(i,j) = (float)t;
        } 
    }

    std::stringstream GTPath;
    GTPath << MatPath << "groundTruth.txt";
    std::string GTPath2 = GTPath.str();
    std::ifstream GTres(GTPath2.c_str());
    for (size_t i = 0; i < 203; i++)
    {
        for (size_t j = 0; j < 6; j++)
        {
            int t;
            GTres >> t;
            GTMat.at<float>(i,j) = (float)t;
        } 
    }

    for (size_t i = 1; i < 629; i++)
    {
        std::stringstream ImgName;
        ImgName << DataPath << "OrgLarg/mkz/images-" << std::setfill('0') << std::setw(5) << i << ".bmp";
        orgNameListmkz.push_back(ImgName.str());
    }

    for (size_t i = 1; i < 204; i++)
    {
        std::stringstream ImgName;
        ImgName << DataPath << "OrgLarg/seg/images-" << std::setfill('0') << std::setw(5) << i << ".bmp";
        orgNameListseg.push_back(ImgName.str());
    }
}

int checkManual(int mkzIdx, int segIdx, std::vector<cv::String> &orgNameListmkz,
                std::vector<cv::String> &orgNameListseg)
{
    std::cout << "segIdx: " << segIdx << std::endl;
    std::cout << "mkzIdx: " << mkzIdx << std::endl;

    if (mkzIdx != 0)
    {
        cv::Mat segImg = cv::imread(orgNameListseg[segIdx],CV_LOAD_IMAGE_UNCHANGED);
        
        cv::Mat mkzImg = cv::imread(orgNameListmkz[mkzIdx],CV_LOAD_IMAGE_UNCHANGED);

        cv::Mat segImg2;
        cv::Size dstSize = cv::Size(700,700);
        cv::resize(segImg,segImg2,dstSize,0,0,cv::INTER_LINEAR);

        cv::Mat mkzImg2;
        cv::resize(mkzImg,mkzImg2,dstSize,0,0,cv::INTER_LINEAR);

        cv::imshow("seg",segImg2);
        cv::imshow("mkz",mkzImg2);
        cv::waitKey(60);

        std::cout << "Is this two image same? y/n. " << std::endl;
        int ch;
        std::cin >> ch;
        
        return ch;
    }
    else
    {
        return 0;
    }
}

void getChkMat(cv::Mat &LastRes, cv::Mat &ThisRes, cv::Mat &GTruMat, cv::Mat &ChekMat, 
                std::vector<cv::String> &orgNameListmkz,
                std::vector<cv::String> &orgNameListseg)
{
    for (size_t i = 0; i < 203; i++)
    {
        for (size_t j = 0; j < 2; j++)
        {
            int flag = 1;
            for (size_t k = 0; k < 6; k++)
            {
                if (abs(ThisRes.at<float>(i,j)-GTruMat.at<float>(i,k)) < 3)
                {
                    if (j == 0 && k == 0)
                    {
                        ChekMat.at<float>(i,j) = ThisRes.at<float>(i,j);
                    }
                    else if(j != 0 && k != 0 && GTruMat.at<float>(i,j) != 0)
                    {
                        ChekMat.at<float>(i,j) = 4;
                    }
                    flag = 0;
                }
            }
            if (flag)
            {
                for (size_t k = 0; k < 6; k++)
                {
                    if (abs(ThisRes.at<float>(i,j)-LastRes.at<float>(i,k)) < 3)
                    {
                        ChekMat.at<float>(i,j) = 1;
                        flag = 0;
                    }
                }
                
                if (flag)
                {
                    ChekMat.at<float>(i,j) = checkManual((int)ThisRes.at<float>(i,j),(int)ThisRes.at<float>(i,0),
                                                        orgNameListmkz, orgNameListseg);
                }
            }
        }
    }
}

void getGTMat(cv::Mat &ThisRes, cv::Mat &ThisChk, cv::Mat &GTruMat)
{
    for (size_t i = 0; i < 203; i++)
    {
        for (size_t j = 0; j < 6; j++)
        {
            if (ThisChk.at<float>(i,j) > 2 || j == 0)
            {
                GTruMat.at<float>(i,j) = ThisRes.at<float>(i,j);
            }
        }
    }
}

void saveGT(cv::Mat &GTruMat)
{
    std::ofstream GTfout("/home/yujr/openslam_openseqslam/AnalyseRes/groundTruth.txt");

    if (!GTfout) 
    { 
        std::cout << "File Not Opened" << std::endl; 
    }

    for (size_t i = 0; i < 203; i++)
    {
        for (size_t j = 0; j < 6; j++)
        {
            GTfout << GTruMat.at<float>(i,j) << "\t"; 
        }
        GTfout << std::endl; 
    }
    GTfout.close();
}

void saveChk(cv::Mat &ThisChk)
{
    std::ofstream Chkfout("/home/yujr/openslam_openseqslam/AnalyseRes/check.txt");

    if (!Chkfout) 
    { 
        std::cout << "File Not Opened" << std::endl; 
    }

    for (size_t j = 0; j < 6; j++)
    {
        for (size_t i = 0; i < 203; i++)
        {
            
            Chkfout << ThisChk.at<float>(i,j) << " "; 
             
        }
        Chkfout << std::endl;
    }
    Chkfout << std::endl << std::endl;

    Chkfout.close();
}

void saveChkMat(cv::Mat &ThisRes, cv::Mat &ThisChk)
{
    std::ofstream Chkfout("/home/yujr/openslam_openseqslam/AnalyseRes/saveChkMat.txt");

    if (!Chkfout) 
    { 
        std::cout << "File Not Opened" << std::endl; 
    }

    int correct = 0;
    for (size_t i = 0; i < 203; i++)
    {
        if (ThisChk.at<float>(i,1) > 2)
        {
            correct++;
        }
    }
    
    Chkfout << "correct of each one" << correct << std::endl << std::endl;

    
    Chkfout.close();
}

int main(int argc, char const *argv[])
{
    std::string readMatPath = "/home/yujr/openslam_openseqslam/AnalyseRes/";
    std::string readDataPath = "/home/yujr/openslam_openseqslam/datasets/";

    cv::Mat LastRes = cv::Mat::zeros(203,6,CV_32FC1);
    cv::Mat ThisRes = cv::Mat::zeros(203,2,CV_32FC1);    
    cv::Mat GTruMat = cv::Mat::zeros(203,6,CV_32FC1);
    cv::Mat ChekMat = cv::Mat::zeros(203,2,CV_32FC1);

    
    std::vector<cv::String> orgNameListmkz, semNameListmkz, vecNameListmkz;
    std::vector<cv::String> orgNameListseg, semNameListseg, vecNameListseg;

    getResMat(readMatPath, LastRes, ThisRes, GTruMat, readDataPath, orgNameListmkz, orgNameListseg);

    getChkMat(LastRes, ThisRes, GTruMat, ChekMat, orgNameListmkz, orgNameListseg);
    
    saveChkMat(ThisRes, ChekMat);

    return 0;
}
