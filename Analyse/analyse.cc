#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

void getResMat(std::string Path, cv::Mat &resMat, cv::Mat &gtMat);

void getNameList(std::string NamePath, std::vector<cv::String> &orgNameListmkz, std::vector<cv::String> &orgNameListseg,
                 std::vector<cv::String> &semNameListmkz, std::vector<cv::String> &semNameListseg,
                 std::vector<cv::String> &vecNameListmkz, std::vector<cv::String> &vecNameListseg);

void sumResNum(int *sum, int sizeSum, cv::Mat &resMat);

void sumGtNum(int *sum, int sizeSum, cv::Mat &gtMat);

void watchImg(std::vector<cv::String> &orgNameListmkz, std::vector<cv::String> &orgNameListseg,
              std::vector<cv::String> &semNameListmkz, std::vector<cv::String> &semNameListseg,
              std::vector<cv::String> &vecNameListmkz, std::vector<cv::String> &vecNameListseg);


int main(int argc, char const *argv[])
{
    std::ofstream Analysefout("/home/yujr/openslam_openseqslam/Analyse/Analyse.txt");

    std::string ResMatPath = "/home/yujr/openslam_openseqslam/results/";
    std::string NamePath = "/home/yujr/openslam_openseqslam/datasets/";
    
    cv::Mat resMat = cv::Mat::zeros(203,4,CV_32FC1);
    cv::Mat gtMat = cv::Mat::zeros(203,4,CV_32FC1);
    
    getResMat(ResMatPath,resMat,gtMat);

    std::vector<cv::String> orgNameListmkz;
    std::vector<cv::String> semNameListmkz;
    std::vector<cv::String> vecNameListmkz;
    std::vector<cv::String> orgNameListseg;
    std::vector<cv::String> semNameListseg;
    std::vector<cv::String> vecNameListseg;

    getNameList(NamePath, orgNameListmkz, semNameListmkz, vecNameListmkz, orgNameListseg, semNameListseg, vecNameListseg);
 
    if (!Analysefout) 
    { 
        std::cout << "File Not Opened" << std::endl; 
    } 
    
    int sum[4] = {0,0,0,0};
    int sizeSum = sizeof(sum)/sizeof(int);
    sumResNum(sum,sizeSum,resMat);

    Analysefout << "sum of all res" << std::endl;
    for (size_t i = 0; i < sizeSum; i++)
    {
        Analysefout << sum[i] << "\t";
    }
    Analysefout << std::endl << std::endl; 

    int gt[4] = {0,0,0,0};
    int sizeGt = sizeof(gt)/sizeof(int);
    sumGtNum(gt,sizeGt,gtMat);

    Analysefout << "sum after gt check" << std::endl;
    for (size_t i = 0; i < sizeSum; i++)
    {
        Analysefout << gt[i] << "\t";
    }
    Analysefout << std::endl << std::endl; 

    double avg[4] = {0.0,0.0,0.0,0.0};
    for (size_t i = 0; i < 4; i++)
    {
        avg[i] = gt[i] / sum[i];
        Analysefout << avg[i] << " a-g " << gt[i] << " " << sum[i] << std::endl;
    }

    Analysefout << "on average" << std::endl;
    for (size_t i = 0; i < sizeSum; i++)
    {
        Analysefout << avg[i] << "\t";
    }
    Analysefout << std::endl; 
    
     
    Analysefout  << "ABB: " << std::endl;
    int all = 0;
    for (size_t i = 0; i < 203; i++)
    {
        if (gtMat.at<float>(i,1) > 2 && gtMat.at<float>(i,2) < 2 && gtMat.at<float>(i,3) < 2)
        {
            Analysefout << resMat.at<float>(i,0) << " " << resMat.at<float>(i,1) << " "
                        << resMat.at<float>(i,2) << " " << resMat.at<float>(i,2) << std::endl;
            Analysefout << gtMat.at<float>(i,0) << " " << gtMat.at<float>(i,1) << " "
                        << gtMat.at<float>(i,2) << " " << gtMat.at<float>(i,2) << std::endl;
            all++;
        }
    }

    Analysefout << std::endl << "Num of ABB is: "<< all << " and BAB is: " << std::endl;
    all = 0;
    for (size_t i = 0; i < 203; i++)
    {
        if (gtMat.at<float>(i,1) < 2 && gtMat.at<float>(i,2) > 2 && gtMat.at<float>(i,3) < 2)
        {
            Analysefout << resMat.at<float>(i,0) << " " << resMat.at<float>(i,1) << " "
                        << resMat.at<float>(i,2) << " " << resMat.at<float>(i,2) << std::endl;
            Analysefout << gtMat.at<float>(i,0) << " " << gtMat.at<float>(i,1) << " "
                        << gtMat.at<float>(i,2) << " " << gtMat.at<float>(i,2) << std::endl;
            all++;
        }
    }
    
    Analysefout << std::endl << "Num of BAB is: "<< all << " and BBA is: " << std::endl;
    all = 0;
    for (size_t i = 0; i < 203; i++)
    {
        if (gtMat.at<float>(i,1) < 2 && gtMat.at<float>(i,2) > 2 && gtMat.at<float>(i,3) < 2)
        {
            Analysefout << resMat.at<float>(i,0) << " " << resMat.at<float>(i,1) << " "
                        << resMat.at<float>(i,2) << " " << resMat.at<float>(i,2) << std::endl;
            Analysefout << gtMat.at<float>(i,0) << " " << gtMat.at<float>(i,1) << " "
                        << gtMat.at<float>(i,2) << " " << gtMat.at<float>(i,2) << std::endl;
            all++;
        }
    }

    Analysefout << std::endl << "Num of BBA is: "<< all << " and AAB is: " << std::endl;
    all = 0;
    for (size_t i = 0; i < 203; i++)
    {
        if (gtMat.at<float>(i,1) > 2 && gtMat.at<float>(i,2) > 2 && gtMat.at<float>(i,3) < 2)
        {
            Analysefout << resMat.at<float>(i,0) << " " << resMat.at<float>(i,1) << " "
                        << resMat.at<float>(i,2) << " " << resMat.at<float>(i,2) << std::endl;
            Analysefout << gtMat.at<float>(i,0) << " " << gtMat.at<float>(i,1) << " "
                        << gtMat.at<float>(i,2) << " " << gtMat.at<float>(i,2) << std::endl;
            all++;
        }
    }
    
    Analysefout << std::endl << "Num of AAB is: "<< all << " and ABA is: " << std::endl;
    all = 0;
    for (size_t i = 0; i < 203; i++)
    {
        if (gtMat.at<float>(i,1) > 2 && gtMat.at<float>(i,2) < 2 && gtMat.at<float>(i,3) > 2)
        {
            Analysefout << resMat.at<float>(i,0) << " " << resMat.at<float>(i,1) << " "
                        << resMat.at<float>(i,2) << " " << resMat.at<float>(i,2) << std::endl;
            Analysefout << gtMat.at<float>(i,0) << " " << gtMat.at<float>(i,1) << " "
                        << gtMat.at<float>(i,2) << " " << gtMat.at<float>(i,2) << std::endl;
            all++;
        }
    }

    Analysefout << std::endl << "Num of ABA is: "<< all << " and BAA is: " << std::endl;
    all = 0;
    for (size_t i = 0; i < 203; i++)
    {
        if (gtMat.at<float>(i,1) < 2 && gtMat.at<float>(i,2) > 2 && gtMat.at<float>(i,3) > 2)
        {
            Analysefout << resMat.at<float>(i,0) << " " << resMat.at<float>(i,1) << " "
                        << resMat.at<float>(i,2) << " " << resMat.at<float>(i,2) << std::endl;
            Analysefout << gtMat.at<float>(i,0) << " " << gtMat.at<float>(i,1) << " "
                        << gtMat.at<float>(i,2) << " " << gtMat.at<float>(i,2) << std::endl;
            all++;
        }
    }
    
    Analysefout << std::endl << "Num of BAA is: "<< all << " and AAA is: " << std::endl;
    all = 0;
    for (size_t i = 0; i < 203; i++)
    {
        if (gtMat.at<float>(i,1) > 2 && gtMat.at<float>(i,2) > 2 && gtMat.at<float>(i,3) > 2)
        {
            Analysefout << resMat.at<float>(i,0) << " " << resMat.at<float>(i,1) << " "
                        << resMat.at<float>(i,2) << " " << resMat.at<float>(i,2) << std::endl;
            all++;
        }
    }

    Analysefout << std::endl << "Num of AAA is: "<< all << " and BBB is: " << std::endl;
    all = 0;
    for (size_t i = 0; i < 203; i++)
    {
        if (gtMat.at<float>(i,1) < 2 && gtMat.at<float>(i,2) < 2 && gtMat.at<float>(i,3) < 2)
        {
            Analysefout << resMat.at<float>(i,0) << " " << resMat.at<float>(i,1) << " "
                        << resMat.at<float>(i,2) << " " << resMat.at<float>(i,2) << std::endl;
            all++;
        }
    }
    Analysefout << std::endl << "Num of AAA is: "<< all << std::endl;

    Analysefout.close();
    //watchImg(orgNameListmkz, semNameListmkz, vecNameListmkz, orgNameListseg, semNameListseg, vecNameListseg);
    
    return 0;
}

void getResMat(std::string Path, cv::Mat &resMat, cv::Mat &gtMat)
{
    std::stringstream ResPath;
    ResPath << Path << "X.txt";
    std::string ResMatPath = ResPath.str();
    std::ifstream res(ResMatPath.c_str());
    for (size_t i = 0; i < 203; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            int t;
            res >> t;
            resMat.at<float>(i,j) = (float)t;
        } 
    }

    std::stringstream gtPath;
    gtPath << Path << "SumResT.txt";
    std::string gtMatPath = gtPath.str();
    std::ifstream gt(gtMatPath.c_str());
    for (size_t i = 0; i < 203; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            int t;
            gt >> t;
            gtMat.at<float>(i,j) = (float)t;
        } 
    }
}

void getNameList(std::string NamePath, std::vector<cv::String> &orgNameListmkz, std::vector<cv::String> &orgNameListseg,
                 std::vector<cv::String> &semNameListmkz, std::vector<cv::String> &semNameListseg,
                 std::vector<cv::String> &vecNameListmkz, std::vector<cv::String> &vecNameListseg)
{
    for (size_t i = 1; i < 629; i++)
    {
        std::stringstream ImgName;
        ImgName << NamePath << "OrgLarg/mkz/images-" << std::setfill('0') << std::setw(5) << i << ".bmp";
        orgNameListmkz.push_back(ImgName.str());
    }
    
    for (size_t i = 1; i < 629; i++)
    {
        std::stringstream ImgName;
        ImgName << NamePath << "SemColOrgLag/mkz/images-" << std::setfill('0') << std::setw(5) << i << ".bmp";
        semNameListmkz.push_back(ImgName.str());
    }

    for (size_t i = 1; i < 629; i++)
    {
        std::stringstream ImgName;
        ImgName << NamePath << "ManVec/mkz/images-" << std::setfill('0') << std::setw(5) << i << ".bmp";
        vecNameListmkz.push_back(ImgName.str());
    }

    for (size_t i = 1; i < 204; i++)
    {
        std::stringstream ImgName;
        ImgName << NamePath << "OrgLarg/seg/images-" << std::setfill('0') << std::setw(5) << i << ".bmp";
        orgNameListseg.push_back(ImgName.str());
    }
    
    for (size_t i = 1; i < 204; i++)
    {
        std::stringstream ImgName;
        ImgName << NamePath << "SemColOrgLag/seg/images-" << std::setfill('0') << std::setw(5) << i << ".bmp";
        semNameListseg.push_back(ImgName.str());
    }

    for (size_t i = 1; i < 204; i++)
    {
        std::stringstream ImgName;
        ImgName << NamePath << "ManVec/seg/images-" << std::setfill('0') << std::setw(5) << i << ".bmp";
        vecNameListseg.push_back(ImgName.str());
    }
}

void sumResNum(int *sum, int sizeSum, cv::Mat &resMat)
{
    for (size_t j = 0; j < 4; j++)
    {
        int rowsum = 0;
        for (size_t i = 0; i < 203; i++)
        {
            if (resMat.at<float>(i,j) > 0)
            {
                rowsum++;
            }            
        }
        *(sum+j) = rowsum; 
    }
}

void sumGtNum(int *sum, int sizeSum, cv::Mat &gtMat)
{
    for (size_t j = 0; j < 4; j++)
    {
        int rowsum = 0;
        for (size_t i = 0; i < 203; i++)
        {
            if (gtMat.at<float>(i,j) > 2)
            {
                rowsum++;
            }            
        }
        *(sum+j) = rowsum; 
    }
}

void watchImg(std::vector<cv::String> &orgNameListmkz, std::vector<cv::String> &orgNameListseg,
              std::vector<cv::String> &semNameListmkz, std::vector<cv::String> &semNameListseg,
              std::vector<cv::String> &vecNameListmkz, std::vector<cv::String> &vecNameListseg)
{
    /* cv::Mat SumT = cv::Mat::zeros(203,4,CV_32FC1);
    for (size_t i = 0; i < 203; i++)
    {
        int segIdx = (int)ResMat.at<float>(i,0);
        for (size_t j = 1; j < 4; j++)
        {
            int mkzIdx = (int)ResMat.at<float>(i,j);

            std::cout << "segIdx: " << segIdx << std::endl;
            std::cout << "mkzIdx: " << mkzIdx << std::endl;

            if (mkzIdx != 0)
            {
                cv::Mat segImg = cv::imread(segNameList[segIdx],CV_LOAD_IMAGE_UNCHANGED);
                
                cv::Mat mkzImg = cv::imread(mkzNameList[mkzIdx],CV_LOAD_IMAGE_UNCHANGED);

                cv::Mat segImg2;
                cv::Size dstSize = cv::Size(700,700);
                cv::resize(segImg,segImg2,dstSize,0,0,cv::INTER_LINEAR);

                cv::Mat mkzImg2;
                cv::resize(mkzImg,mkzImg2,dstSize,0,0,cv::INTER_LINEAR);

                cv::imshow("seg",segImg2);
                cv::imshow("mkz",mkzImg2);
                cv::waitKey(60);

                std::cout << "Is this two image same? y/n. " << i << std::endl;
                int ch;
                std::cin >> ch;
                if(ch > 0)
                {
                    SumT.at<float>(i,j) = ch;
                } 
            }
        }
    }

    cv::Mat SumRow = cv::Mat::zeros(1,4,CV_32FC1);
    cv::reduce(SumT, SumRow, 0, CV_REDUCE_SUM);
    
    std::cout << "sumrow is" << std::endl << SumRow << std::endl;  */
}