#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

void getNameList(std::string ResMatPath, std::vector<cv::String> &segNameList,
                 std::vector<cv::String> &mkzNameList, cv::Mat &ResMat);

int main(int argc, char const *argv[])
{
    std::string ResMatPath = "/home/yujr/openslam_openseqslam/results/X.txt";
    std::string GTMatSave = "/home/yujr/openslam_openseqslam/results/GT.bmp";
    std::string SumMatSave = "/home/yujr/openslam_openseqslam/results/SumT.bmp";

    cv::Mat ResMat = cv::Mat::zeros(203,4,CV_32FC1);
    std::vector<cv::String> segNameList;
    std::vector<cv::String> mkzNameList;
    
    getNameList(ResMatPath,segNameList,mkzNameList,ResMat);
 
    cv::Mat SumT = cv::Mat::zeros(203,4,CV_32FC1);
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

            std::ofstream SumTfout("/home/yujr/openslam_openseqslam/results/SumResT.txt");
            if (!SumTfout) 
            { 
                std::cout << "File Not Opened" << std::endl; 
            } 
            
            for (int i = 0; i<SumT.rows; i++) 
            { 
                for (int j = 0; j<SumT.cols; j++) 
                { 
                    SumTfout << SumT.at<float>(i, j) << "\t"; 
                } 
                SumTfout << std::endl; 
            } 
            SumTfout.close(); 

        }
    }

    cv::Mat SumRow = cv::Mat::zeros(1,4,CV_32FC1);
    cv::reduce(SumT, SumRow, 0, CV_REDUCE_SUM);
    
    std::cout << "sumrow is" << std::endl << SumRow << std::endl; 


    return 0;
}


void getNameList(std::string ResMatPath, std::vector<cv::String> &segNameList,
                 std::vector<cv::String> &mkzNameList, cv::Mat &ResMat)
{
	std::ifstream in(ResMatPath.c_str());
    for (size_t i = 0; i < 203; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            int t;
            in >> t;
            ResMat.at<float>(i,j) = (float)t;
        } 
    }
    
    // std::cout << ResMat << std::endl;

    std::string mkzPath = "/home/yujr/openslam_openseqslam/datasets/OrgLarg/mkz/";
    for (size_t i = 1; i < 629; i++)
    {
        std::stringstream ImgName;
        ImgName << mkzPath << "images-" << std::setfill('0') << std::setw(5) << i << ".bmp";
        mkzNameList.push_back(ImgName.str());

        // std::cout << ImgName.str() << std::endl;
    }
    

    std::string segPath = "/home/yujr/openslam_openseqslam/datasets/OrgLarg/seg/";
    for (size_t i = 1; i < 204; i++)
    {
        std::stringstream ImgName;
        ImgName << segPath << "images-" << std::setfill('0') << std::setw(5) << i << ".bmp";
        segNameList.push_back(ImgName.str());

        // std::cout << ImgName.str() << std::endl;
    }

}