#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

cv::Mat src_gray;
int sltrhresh = 93;
int max_thresh = 255;
cv::RNG rng(12345);

void GetImgNameList(std::string ImgReadPath,std::vector<cv::String> &ImgNameList,
                    std::vector<cv::String> &ArrNameList,std::vector<cv::String> &DasNameList,
                    std::vector<cv::String> &SltNameList,std::vector<cv::String> &ResNameList);

bool useImg(int i,std::vector<cv::String> &ImgNameList,std::vector<cv::String> &ResNameList);

void saveOrgImg(int i,int j,std::string ImgReadPath, std::vector<cv::String> &ImgNameList);

void getSemOrg(int i, int j, std::string ImgReadPath, std::vector<cv::String> &ImgNameList, 
            std::vector<cv::String> &ResNameList, std::vector<cv::String> &ArrNameList, 
            std::vector<cv::String> &DasNameList, std::vector<cv::String> &SltNameList);

void getSemColorOrg(int i, int j, std::string ImgReadPath, std::vector<cv::String> &ImgNameList, 
            std::vector<cv::String> &ResNameList, std::vector<cv::String> &ArrNameList, 
            std::vector<cv::String> &DasNameList, std::vector<cv::String> &SltNameList);


void getArrDesM(int i, std::vector<cv::String> &ArrNameList, cv::Mat &ManDesMat, cv::Mat &ManDesVec);
void getDasDesM(int i, std::vector<cv::String> &DasNameList, cv::Mat &ManDesMat, cv::Mat &ManDesVec);
void getSltDesM(int i, std::vector<cv::String> &SltNameList, cv::Mat &ManDesMat, cv::Mat &ManDesVec);

void findContor(cv::Mat &srcImg, int thresh);

void saveDescrip(int j, std::string ImgReadPath, cv::Mat &ManDesMat, cv::Mat &ManDesVec);

int main(int argc, char const *argv[])
{
    std::string ImgReadPath = "/home/yujr/mkz_11_19/";

    std::vector<cv::String> ImgNameList;
    std::vector<cv::String> ArrNameList;

    std::vector<cv::String> DasNameList;
    std::vector<cv::String> SltNameList;
    std::vector<cv::String> ResNameList;

    GetImgNameList(ImgReadPath,ImgNameList,ArrNameList,DasNameList,SltNameList,ResNameList);
    
    int j=447;
    for (size_t i = 3013; i < ImgNameList.size(); i++)
    {std::cout << ImgNameList[i] << std::endl;
        if (useImg(i,ImgNameList,ResNameList))
        {
            std::cout << "ok" << std::endl;
            cv::Mat ManDesMat = cv::Mat::zeros(41,41,CV_8UC3);
            cv::Mat ManDesVec = cv::Mat::ones(15,1,CV_32FC1);
            ManDesVec = ManDesVec * -1;
            saveOrgImg(i,j,ImgReadPath,ImgNameList);
            getSemOrg(i,j,ImgReadPath,ImgNameList,ResNameList,ArrNameList,DasNameList,SltNameList);
            getSemColorOrg(i,j,ImgReadPath,ImgNameList,ResNameList,ArrNameList,DasNameList,SltNameList);
            
            getArrDesM(i,ArrNameList,ManDesMat,ManDesVec);
            getDasDesM(i,DasNameList,ManDesMat,ManDesVec);
            getSltDesM(i,SltNameList,ManDesMat,ManDesVec);

            saveDescrip(j,ImgReadPath,ManDesMat,ManDesVec);
            
            j++;
            std::cout << "the j is :" << j << std::endl;
        }
    }

    return 0;
}

void GetImgNameList(std::string ImgReadPath,std::vector<cv::String> &ImgNameList,
                    std::vector<cv::String> &ArrNameList,std::vector<cv::String> &DasNameList,
                    std::vector<cv::String> &SltNameList,std::vector<cv::String> &ResNameList)
{
    for (size_t i = 10; i < 3806; i++)
    {
        std::stringstream ImgName;
        ImgName << ImgReadPath << "stitch/" << i << ".bmp";
        ImgNameList.push_back(ImgName.str());
            
        std::stringstream ArrName;
        ArrName << ImgReadPath << "arrow/" << i << "_arrow.bmp";
        ArrNameList.push_back(ArrName.str());

        std::stringstream DasName;
        DasName << ImgReadPath << "dash/" << i << "_dash.bmp";
        DasNameList.push_back(DasName.str());        

        std::stringstream SltName;
        SltName << ImgReadPath << "slot/" << i << "_slot.bmp";
        SltNameList.push_back(SltName.str());

        std::stringstream ResName;
        ResName << ImgReadPath << "result/" << i << ".bmp";
        ResNameList.push_back(ResName.str());  
    }
std::cout << "GetImgNameList is finished" << std::endl; 
}

bool useImg(int i,std::vector<cv::String> &ImgNameList,std::vector<cv::String> &ResNameList)
{
    cv::Mat srcImg = cv::imread(ImgNameList[i],CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat resImg = cv::imread(ResNameList[i],CV_LOAD_IMAGE_UNCHANGED);

    if (srcImg.empty())
    {
        std::cerr << std::endl << "Failed to load image at : " << ImgNameList[i] << std::endl;
    }
    else if (resImg.empty())
    {
        std::cerr << std::endl << "Failed to load image at : " << ResNameList[i] << std::endl;
    }
    else
    {
        
        cv::imshow("src",srcImg);
        cv::waitKey(60);

        std::cout << "the srcImg and resImg are shown, is this used? y/n. " << i << std::endl;

        char ch;
        std::cin >> ch;
        if(ch == 'n')
        {
            return 0;
        }
        else if(ch == 'y')
        {
            return 1; 
        } 
    }
}

void saveOrgImg(int i, int j,std::string ImgReadPath, std::vector<cv::String> &ImgNameList)
{
    cv::Mat srcImg = cv::imread(ImgNameList[i],CV_LOAD_IMAGE_UNCHANGED);
    
    if (srcImg.empty())
    {
        std::cerr << std::endl << "Failed to load image at : " << ImgNameList[i] << std::endl;
    }
    else
    {
        cv::Mat srcImg2;
        cv::Size dstSize = cv::Size(64,32);
        cv::resize(srcImg,srcImg2,dstSize,0,0,cv::INTER_LINEAR);

        std::stringstream OrgPath;
        OrgPath << ImgReadPath << "Org/images-" << std::setfill('0') << std::setw(5) << j << ".bmp";
        cv::imwrite(OrgPath.str(),srcImg2);
std::cout << "original image is saved" << std::endl;
    }
}

void getSemOrg(int i, int j, std::string ImgReadPath, std::vector<cv::String> &ImgNameList,
            std::vector<cv::String> &ResNameList, std::vector<cv::String> &ArrNameList, 
            std::vector<cv::String> &DasNameList, std::vector<cv::String> &SltNameList)
{
    cv::Mat srcImg = cv::imread(ImgNameList[i],CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat arrImg = cv::imread(ArrNameList[i],CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat dasImg = cv::imread(DasNameList[i],CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat sltImg = cv::imread(SltNameList[i],CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat DescrpMat = cv::Mat::zeros(448,448,CV_8UC3);

    std::cout << "ImgNameList[i]" << ImgNameList[i] << std::endl;
    std::cout << "ResNameList[i]" << ResNameList[i] << std::endl;
    std::cout << "ArrNameList[i]" << ArrNameList[i] << std::endl;
    std::cout << "DasNameList[i]" << DasNameList[i] << std::endl;
    std::cout << "SltNameList[i]" << SltNameList[i] << std::endl;

    for (size_t i = 0; i < arrImg.cols; i++)
    {
        for (size_t j = 0; j < arrImg.rows; j++)
        {
            int arrlab = arrImg.ptr<uchar>(j)[i];
            int daslab = dasImg.ptr<uchar>(j)[i];
            int sltlab = sltImg.ptr<uchar>(j)[i];
            
            if (arrlab == 0 && daslab == 0 && sltlab == 0) 
            {
                DescrpMat.at<cv::Vec3b>(j,i) = cv::Vec3b(0.0, 0.0, 0.0);
            }
            else
            {
                DescrpMat.at<cv::Vec3b>(j,i) = srcImg.at<cv::Vec3b>(j,i);
            }                
        }
    } 
    
    cv::Mat srcImg2;
    cv::Size dstSize = cv::Size(64,32);
    cv::resize(DescrpMat,srcImg2,dstSize,0,0,cv::INTER_LINEAR);

    std::stringstream SemOrgPath;
    SemOrgPath << ImgReadPath << "SemOrg/images-" << std::setfill('0') << std::setw(5) << j << ".bmp";
    cv::imwrite(SemOrgPath.str(),srcImg2);
std::cout << "Semantic original is saved" << std::endl;
}


void getSemColorOrg(int i, int j, std::string ImgReadPath, std::vector<cv::String> &ImgNameList,
            std::vector<cv::String> &ResNameList, std::vector<cv::String> &ArrNameList, 
            std::vector<cv::String> &DasNameList, std::vector<cv::String> &SltNameList)
{
    cv::Mat srcImg = cv::imread(ImgNameList[i],CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat arrImg = cv::imread(ArrNameList[i],CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat dasImg = cv::imread(DasNameList[i],CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat sltImg = cv::imread(SltNameList[i],CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat DescrpMat = cv::Mat::zeros(448,448,CV_8UC3);

    std::cout << "ImgNameList[i]" << ImgNameList[i] << std::endl;
    std::cout << "ResNameList[i]" << ResNameList[i] << std::endl;
    std::cout << "ArrNameList[i]" << ArrNameList[i] << std::endl;
    std::cout << "DasNameList[i]" << DasNameList[i] << std::endl;
    std::cout << "SltNameList[i]" << SltNameList[i] << std::endl;

    for (size_t i = 0; i < arrImg.cols; i++)
    {
        for (size_t j = 0; j < arrImg.rows; j++)
        {
            int arrlab = arrImg.ptr<uchar>(j)[i];
            int daslab = dasImg.ptr<uchar>(j)[i];
            int sltlab = sltImg.ptr<uchar>(j)[i];
            
            if (sltlab != 0)
            {
                DescrpMat.at<cv::Vec3b>(j,i) = cv::Vec3b(255.0, 255.0, 255.0);
            }

            if (arrlab != 0)
            {
                DescrpMat.at<cv::Vec3b>(j,i) = cv::Vec3b(0.0, 130.0, 180.0);
            }
            
            if (daslab != 0)
            {
                DescrpMat.at<cv::Vec3b>(j,i) = cv::Vec3b(220.0, 200.0, 0.0);
            }           
        }
    } 
    
    // cv::Mat srcImg2;
    // cv::Size dstSize = cv::Size(64,32);
    // cv::resize(DescrpMat,srcImg2,dstSize,0,0,cv::INTER_LINEAR);

    std::stringstream SemColOrg;
    SemColOrg << ImgReadPath << "SemColOrg/images-" << std::setfill('0') << std::setw(5) << j << ".bmp";
    cv::imwrite(SemColOrg.str(),DescrpMat);
std::cout << "Semantic color original is saved" << std::endl;
}

void getArrDesM(int i, std::vector<cv::String> &ArrNameList, cv::Mat &ManDesMat, cv::Mat &ManDesVec)
{
    cv::Mat arrImg = cv::imread(ArrNameList[i],CV_LOAD_IMAGE_UNCHANGED);
    int arrthresh = 95;

    findContor(arrImg, arrthresh);

    int num;
    std::cout << "cin the num of arr type" << std::endl;
    std::cin >> num;

    for (size_t i = 0; i < num; i++)
    {
        std::cout << "cin the arrow type : " << std::endl;
        int type;
        std::cin >> type;
        int value = ManDesMat.at<cv::Vec3b>(type,type)[0];
        value++;
        ManDesMat.at<cv::Vec3b>(type,type) = cv::Vec3b(value, value, value);
        ManDesVec.at<float>(type,0)= value;
    } 
std::cout << "Arrow is get" << std::endl; 
// std::cout << "ManDesVec is get" << ManDesMat << std::endl;
std::cout << "ManDesVec is get" << ManDesVec.t() << std::endl;
}

void getDasDesM(int i,  std::vector<cv::String> &DasNameList, cv::Mat &ManDesMat, cv::Mat &ManDesVec)
{
    cv::Mat dasImg = cv::imread(DasNameList[i],CV_LOAD_IMAGE_UNCHANGED);
    int dasthresh = 93;

    findContor(dasImg, dasthresh);

    std::cout << ",cin the num of dash type" << std::endl;
    int num;
    std::cin >> num;

    ManDesMat.at<cv::Vec3b>(5,5) = cv::Vec3b(num, num, num);
    ManDesVec.at<float>(5,0)= num;
std::cout << "Dash is get" << std::endl;
// std::cout << "ManDesVec is get" << ManDesMat << std::endl;
std::cout << "ManDesVec is get" << ManDesVec.t() << std::endl;
}

void getSltDesM(int i, std::vector<cv::String> &SltNameList, cv::Mat &ManDesMat, cv::Mat &ManDesVec)
{
    cv::Mat sltImg = cv::imread(SltNameList[i],CV_LOAD_IMAGE_UNCHANGED);
    int sltthresh = 87;

    //findContor(sltImg, sltthresh);

    std::cout << "slt num ? " << ",cin the num of this type: 0~2" << std::endl;
    int num;
    std::cin >> num;
    ManDesMat.at<cv::Vec3b>(40,40) = cv::Vec3b(num, num, num);
    ManDesVec.at<float>(14,0) = (float)num;

    if (num > 2)
    {
        num = 2;
    }
    

    for (size_t i = 0; i < num; i++)
    {
        std::cout << "cin the B~E" << std::endl;
        char BE;
        std::cin >> BE;
        if (BE == 'B')
        {
            int value = ManDesMat.at<cv::Vec3b>(6,6)[0];
            value++;
            ManDesMat.at<cv::Vec3b>(6,6) = cv::Vec3b(value, value, value);
            ManDesVec.at<float>(6+i*4,0) = 2.0f;
        }
        else if (BE == 'C')
        {
            int value = ManDesMat.at<cv::Vec3b>(7,7)[0];
            value++;
            ManDesMat.at<cv::Vec3b>(7,7) = cv::Vec3b(value, value, value);
            ManDesVec.at<float>(6+i*4,0) = 3.0f;
        }
        else if (BE == 'D')
        {
            int value = ManDesMat.at<cv::Vec3b>(8,8)[0];
            value++;
            ManDesMat.at<cv::Vec3b>(8,8) = cv::Vec3b(value, value, value);
            ManDesVec.at<float>(6+i*4,0) = 4.0f;
        }
        else if (BE == 'E')
        {
            int value = ManDesMat.at<cv::Vec3b>(9,9)[0];
            value++;
            ManDesMat.at<cv::Vec3b>(9,9) = cv::Vec3b(value, value, value);
            ManDesVec.at<float>(6+i*4,0) = 5.0f;
        }
        else
        {
            std::cout << "empty" << std::endl;
        }
        
        std::cout << "cin the hund: 0~9" << std::endl;
        int hund;
        std::cin >> hund;
        if (hund <10)
        {
            ManDesVec.at<float>(7+i*4,0)= (float)hund;
            hund += 10;
            int value = ManDesMat.at<cv::Vec3b>(hund,hund)[0];
            value++;
            ManDesMat.at<cv::Vec3b>(hund,hund) = cv::Vec3b(value, value, value);            
        }
        else
        {
            std::cout << "empty" << std::endl;
        }
        
        std::cout << "cin the tend: 0~9" << std::endl;
        int tend;
        std::cin >> tend;
        if (tend<10)
        {
            ManDesVec.at<float>(8+i*4,0)= (float)tend;
            tend += 20;
            int value = ManDesMat.at<cv::Vec3b>(tend,tend)[0];
            value++;
            ManDesMat.at<cv::Vec3b>(tend,tend) = cv::Vec3b(value, value, value); 
        }
        else
        {
            std::cout << "empty" << std::endl;
        }
        
        std::cout << "cin the oned: 0~9" << std::endl;
        int oned;
        std::cin >> oned;
        if (oned < 10)
        {
            ManDesVec.at<float>(9+i*4,0)= (float)oned;
            oned += 30;
            int value = ManDesMat.at<cv::Vec3b>(oned,oned)[0];
            value++;
            ManDesMat.at<cv::Vec3b>(oned,oned) = cv::Vec3b(value, value, value); 
        }
        else
        {
            std::cout << "empty" << std::endl;
        }
    }
std::cout << "slt is get" << std::endl;
// std::cout << "ManDesVec is get" << ManDesMat << std::endl;
std::cout << "ManDesVec is get" << ManDesVec.t() << std::endl;

}

void findContor(cv::Mat &srcImg, int thresh)
{

    cv::Mat dstImg = cv::Mat::zeros(448,448,CV_8UC3);
    for (size_t i = 0; i < srcImg.cols; i++)
    {
        for (size_t j = 0; j < srcImg.rows; j++)
        {
            int tmplab = srcImg.ptr<uchar>(j)[i];
            if (tmplab == 0 )
            {
                dstImg.at<cv::Vec3b>(j,i) = cv::Vec3b(107, 142, 35);
            }
            else
            {
                dstImg.at<cv::Vec3b>(j,i) = cv::Vec3b(153, 153, 153);
            }                    
        }
    }

    cv::cvtColor( dstImg, src_gray, CV_BGR2GRAY );
    cv::blur( src_gray, src_gray, cv::Size(3,3) );

    cv::Mat canny_output;
    std::vector< std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
 
	/// Detect edges using canny
	cv::Canny( src_gray, canny_output, thresh, thresh*2, 3 );
	/// Find contours
	cv::findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
    std::cout << "contours.size : " << contours.size() << std::endl;
    
    for (size_t i = 0; i < contours.size(); i++)
        std::cout << "contours[i].size : " << contours[i].size() << std::endl;
}


void saveDescrip(int j, std::string ImgReadPath, cv::Mat &ManDesMat, cv::Mat &ManDesVec)
{
    cv::Mat srcImg2;
    cv::Size dstSize = cv::Size(64,32);
    cv::resize(ManDesMat,srcImg2,dstSize,0,0,cv::INTER_LINEAR);

    std::stringstream SemOrgPath;
    SemOrgPath << ImgReadPath << "ManOrg/images-" << std::setfill('0') << std::setw(5) << j << ".bmp";
    cv::imwrite(SemOrgPath.str(),ManDesMat);

    std::stringstream SemVecPath;
    SemVecPath << ImgReadPath << "ManVec/images-" << std::setfill('0') << std::setw(5) << j << ".bmp";
    cv::imwrite(SemVecPath.str(),ManDesVec);
}