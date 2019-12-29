#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>


void getResMat(std::string MatPath, cv::Mat &LastRes, cv::Mat &LastChk, 
                cv::Mat &ThisRes, std::string DataPath, 
                std::vector<cv::String> &orgNameListmkz,
                std::vector<cv::String> &orgNameListseg)
{
    std::stringstream LastResPath;
    LastResPath << MatPath << "LastRes.txt";
    std::string LastResPath2 = LastResPath.str();
    std::ifstream res(LastResPath2.c_str());
    for (size_t i = 0; i < 203; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            int t;
            res >> t;
            LastRes.at<float>(i,j) = (float)t;
        } 
    }

    std::stringstream LastChkPath;
    LastChkPath << MatPath << "LastChk.txt";
    std::string LastChkPath2 = LastChkPath.str();
    std::ifstream chk(LastChkPath2.c_str());
    for (size_t i = 0; i < 203; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            int t;
            chk >> t;
            LastChk.at<float>(i,j) = (float)t;
        } 
    }

    std::stringstream ThisResPath;
    ThisResPath << MatPath << "ThisRes.txt";
    std::string ThisResPath2 = ThisResPath.str();
    std::ifstream Tisres(ThisResPath2.c_str());
    for (size_t i = 0; i < 203; i++)
    {
        for (size_t j = 0; j < 6; j++)
        {
            int t;
            Tisres >> t;
            ThisRes.at<float>(i,j) = (float)t;
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

void getChkMat(cv::Mat &LastRes, cv::Mat &LastChk, cv::Mat &ThisRes, cv::Mat &ThisChk, 
                std::vector<cv::String> &orgNameListmkz,
                std::vector<cv::String> &orgNameListseg)
{
    for (size_t i = 0; i < 203; i++)
    {
        for (size_t j = 0; j < 6; j++)
        {
            int flag = 1;
            for (size_t k = 0; k < 4; k++)
            {
                if (abs(ThisRes.at<float>(i,j)-LastRes.at<float>(i,k)) < 3)
                {
                    if (j == 0 && k == 0)
                    {
                        ThisChk.at<float>(i,j) = ThisRes.at<float>(i,j);
                    }
                    else if(j != 0 && k != 0 && ThisRes.at<float>(i,j) != 0 && 
                            LastRes.at<float>(i,k) != 0)
                    {
                        if (LastChk.at<float>(i,k) > 0)
                        {
                            ThisChk.at<float>(i,j) = LastChk.at<float>(i,k);
                        }
                    }
                    flag = 0;
                }
            }
            if (flag)
            {
                ThisChk.at<float>(i,j) = checkManual((int)ThisRes.at<float>(i,j),(int)ThisRes.at<float>(i,0),
                                                        orgNameListmkz, orgNameListseg);
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

    Chkfout << "correct of each one" << std::endl;      
    for (size_t j = 0; j < 6; j++)
    { 
        int correct = 0;
        for (size_t i = 0; i < 203; i++)
        {
            if (ThisChk.at<float>(i,j) > 2)
            {
                correct++;
            }  
        }
        Chkfout << " " << correct ; 
    }
    Chkfout << std::endl << std::endl;

    Chkfout << "sum of each one" << std::endl;      
    for (size_t j = 0; j < 6; j++)
    { 
        int sum = 0;
        for (size_t i = 0; i < 203; i++)
        {
            sum += ThisChk.at<float>(i,j);
        }
        Chkfout << "  " << sum ; 
    }
    Chkfout << std::endl << std::endl;

    Chkfout << "1B2B3B4B5A : ";
    int sum = 0;
    for (size_t i = 0; i < 203; i++)
    {
        if (ThisChk.at<float>(i,1) < 3 && ThisChk.at<float>(i,2) < 3 &&
            ThisChk.at<float>(i,3) < 3 && ThisChk.at<float>(i,4) < 3 &&
            ThisChk.at<float>(i,5) > 2)
        {
            for (size_t j = 0; j < 6; j++)
            {
                Chkfout << ThisRes.at<float>(i,j) << " ";
            }
            Chkfout << std::endl;
            for (size_t j = 0; j < 6; j++)
            {
                Chkfout << ThisChk.at<float>(i,j) << " ";
            }
            Chkfout << std::endl;
            sum++;
        }        
    }
    Chkfout << "1B2B3B4B5A : " << sum << std::endl << std::endl;

    Chkfout << "1A2B3B4B5B : ";
    sum = 0;
    for (size_t i = 0; i < 203; i++)
    {
        if (ThisChk.at<float>(i,1) > 2 && ThisChk.at<float>(i,2) < 3 &&
            ThisChk.at<float>(i,3) < 3 && ThisChk.at<float>(i,4) < 3 &&
            ThisChk.at<float>(i,5) < 3)
        {
            for (size_t j = 0; j < 6; j++)
            {
                Chkfout << ThisRes.at<float>(i,j) << " ";
            }
            Chkfout << std::endl;
            for (size_t j = 0; j < 6; j++)
            {
                Chkfout << ThisChk.at<float>(i,j) << " ";
            }
            Chkfout << std::endl;
            sum++;
        }        
    }
    Chkfout << "1A2B3B4B5B : " << sum << std::endl << std::endl;

    Chkfout << "1B2A3A4A5B : ";
    sum = 0;
    for (size_t i = 0; i < 203; i++)
    {
        if (ThisChk.at<float>(i,1) < 3 && ThisChk.at<float>(i,2) > 2 &&
            ThisChk.at<float>(i,3) > 2 && ThisChk.at<float>(i,4) > 2 &&
            ThisChk.at<float>(i,5) < 3)
        {
            for (size_t j = 0; j < 6; j++)
            {
                Chkfout << ThisRes.at<float>(i,j) << " ";
            }
            Chkfout << std::endl;
            for (size_t j = 0; j < 6; j++)
            {
                Chkfout << ThisChk.at<float>(i,j) << " ";
            }
            Chkfout << std::endl;
            sum++;
        }        
    }
    Chkfout << "1B2A3A4A5B : " << sum << std::endl << std::endl;


    Chkfout << "3B4A : ";
    sum = 0;
    for (size_t i = 0; i < 203; i++)
    {
        if (ThisChk.at<float>(i,3) < 3 && ThisChk.at<float>(i,4) > 2)
        {
            for (size_t j = 0; j < 6; j++)
            {
                Chkfout << ThisRes.at<float>(i,j) << " ";
            }
            Chkfout << std::endl;
            for (size_t j = 0; j < 6; j++)
            {
                Chkfout << ThisChk.at<float>(i,j) << " ";
            }
            Chkfout << std::endl;
            sum++;
        }        
    }
    Chkfout << "3B4A : " << sum << std::endl << std::endl;

    Chkfout << "3A4B : ";
    sum = 0;
    for (size_t i = 0; i < 203; i++)
    {
        if (ThisChk.at<float>(i,3) > 2 && ThisChk.at<float>(i,4) < 3)
        {
            for (size_t j = 0; j < 6; j++)
            {
                Chkfout << ThisRes.at<float>(i,j) << " ";
            }
            Chkfout << std::endl;
            for (size_t j = 0; j < 6; j++)
            {
                Chkfout << ThisChk.at<float>(i,j) << " ";
            }
            Chkfout << std::endl;
            sum++;
        }        
    }
    Chkfout << "3A4B : " << sum << std::endl << std::endl;


    Chkfout << "2B3A : ";
    sum = 0;
    for (size_t i = 0; i < 203; i++)
    {
        if (ThisChk.at<float>(i,2) < 3 && ThisChk.at<float>(i,3) > 2)
        {
            for (size_t j = 0; j < 6; j++)
            {
                Chkfout << ThisRes.at<float>(i,j) << " ";
            }
            Chkfout << std::endl;
            for (size_t j = 0; j < 6; j++)
            {
                Chkfout << ThisChk.at<float>(i,j) << " ";
            }
            Chkfout << std::endl;
            sum++;
        }        
    }
    Chkfout << "2B3A : " << sum << std::endl << std::endl;

    Chkfout << "2A3B : ";
    sum = 0;
    for (size_t i = 0; i < 203; i++)
    {
        if (ThisChk.at<float>(i,2) > 2 && ThisChk.at<float>(i,3) < 3)
        {
            for (size_t j = 0; j < 6; j++)
            {
                Chkfout << ThisRes.at<float>(i,j) << " ";
            }
            Chkfout << std::endl;
            for (size_t j = 0; j < 6; j++)
            {
                Chkfout << ThisChk.at<float>(i,j) << " ";
            }
            Chkfout << std::endl;
            sum++;
        }        
    }
    Chkfout << "2A3B : " << sum << std::endl << std::endl;
    
    Chkfout.close();
}

int main(int argc, char const *argv[])
{
    std::string readMatPath = "/home/yujr/openslam_openseqslam/AnalyseRes/";
    std::string readDataPath = "/home/yujr/openslam_openseqslam/datasets/";

    cv::Mat LastRes = cv::Mat::zeros(203,4,CV_32FC1);
    cv::Mat LastChk = cv::Mat::zeros(203,4,CV_32FC1);
    cv::Mat ThisRes = cv::Mat::zeros(203,6,CV_32FC1);
    
    cv::Mat ThisChk = cv::Mat::zeros(203,6,CV_32FC1);
    cv::Mat GTruMat = cv::Mat::zeros(203,6,CV_32FC1);
    
    std::vector<cv::String> orgNameListmkz, semNameListmkz, vecNameListmkz;
    std::vector<cv::String> orgNameListseg, semNameListseg, vecNameListseg;

    getResMat(readMatPath, LastRes, LastChk, ThisRes, readDataPath, 
              orgNameListmkz, orgNameListseg);

    getChkMat(LastRes, LastChk, ThisRes, ThisChk, 
              orgNameListmkz, orgNameListseg);
    
    getGTMat(ThisRes, ThisChk, GTruMat);

    saveGT(GTruMat);

    saveChk(ThisChk);

    saveChkMat(ThisRes, ThisChk);

    return 0;
}
