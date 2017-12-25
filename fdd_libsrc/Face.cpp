#include"Face.h"
#include"FDDGlobal.h"

namespace fdd{

const cv::Scalar Face::frontColor=cv::Scalar(0,255,0);
const cv::Scalar Face::leftColor=cv::Scalar(0,255,255);
const cv::Scalar Face::rightColor=cv::Scalar(0,0,255);

Face::Face()
    :pFrame_()
    ,pModel_()
    ,bContainFrontFace_(false)
    ,bContainLeftFace_(false)
    ,bContainRightFace_(false)
    ,faceDetectionRect_(0,0,0,0)
    ,rightEye_(cv::Ptr<Frame>()
    ,cv::Ptr<FaceAnalysisModel>())
    ,mouth_(cv::Ptr<Frame>(),cv::Ptr<FaceAnalysisModel>())
{
}

Face::Face(const cv::Ptr<Frame> &pFrame,const cv::Ptr<FaceAnalysisModel> &pModel)
    :pFrame_(pFrame),pModel_(pModel),rightEye_(pFrame,pModel),leftEye_(pFrame,pModel),mouth_(pFrame,pModel)
    ,bContainFrontFace_(false),bContainLeftFace_(false),bContainRightFace_(false)
{
    faceDetectionRect_ = cv::Rect(0, 0, pFrame_->graySmallImgForFaceDetection().size().width, pFrame_->graySmallImgForFaceDetection().size().height);
}

Face::~Face()
{

}

void Face::pFrame(const cv::Ptr<Frame> &pFrame)
{
	pFrame_ = pFrame;
	rightEye_.pFrame(pFrame);
	leftEye_.pFrame(pFrame);
	mouth_.pFrame(pFrame);
    //reset faceDetectionRect
    faceDetectionRect_ = cv::Rect(0, 0, pFrame_->graySmallImgForFaceDetection().size().width, pFrame_->graySmallImgForFaceDetection().size().height);
    bContainFrontFace_=false;
    bContainLeftFace_=false;
    bContainRightFace_=false;
}

void Face::pModel(const cv::Ptr<FaceAnalysisModel> &pModel)
{
	pModel_ = pModel;
	rightEye_.pModel(pModel);
	leftEye_.pModel(pModel);
	mouth_.pModel(pModel);
}

void Face::setFaceDetectionRect(const cv::Rect &faceDetectionRect)
{
    faceDetectionRect_ = faceDetectionRect;
}

RightEye &Face::rightEye()
{
	return rightEye_;
}

LeftEye &Face::leftEye() {
	return leftEye_;
}

Mouth &Face::mouth()
{
	return mouth_;
}

bool Face::bContainFrontFace()
{
    return bContainFrontFace_;
}

bool Face::bContainLeftFace(){
    return bContainLeftFace_;
}

bool Face::bContainRightFace(){
    return bContainRightFace_;
}

cv::Rect Face::maxFaceOnSmallImgForFaceDetection()
{
    return maxFaceOnSmallImgForFaceDetection_;
}

int Face::headpose(){
    return headpose_;
}

inline cv::Rect Face::flipRect(const cv::Rect &rect,const cv::Size &imgSize,Frame::Direction direction){
    cv::Rect flippedRect(rect);
    if(Frame::Direction::Horizontal==direction){
        flippedRect.x=imgSize.width-flippedRect.x-flippedRect.width;
    }else if(Frame::Direction::Vertical==direction){
        flippedRect.y=imgSize.height-flippedRect.y -flippedRect.height;
    }else {
        std::cout<<"wrong face direction"<<std::endl;
    }
    return flippedRect;
}

bool Face::detectFace(FaceAnalysisModel::FaceType faceType){
    cv::Mat grayImg=pFrame_->graySmallImgForFaceDetection();
    if (0==faceDetectionRect_.area()||FaceAnalysisModel::FaceType::Right==faceType)
    {//If there is no face detected in last frame,reset detection rect to the whole rect of the frame
        faceDetectionRect_ =cv::Rect(0,0,grayImg.size().width,grayImg.size().height);
    }else if(FaceAnalysisModel::FaceType::Right==faceType){
            //flip faceDetectionRect_  by horizontal
            faceDetectionRect_=flipRect(faceDetectionRect_
                                        ,cv::Size(grayImg.size().width ,grayImg.size().height)
                                        ,Frame::Direction::Horizontal);
        }
    pModel_->detectFaces(grayImg(faceDetectionRect_), faces_, faceType,1.1, 2, 0
        //|CV_HAAR_FIND_BIGGEST_OBJECT
        //|CV_HAAR_DO_ROUGH_SEARCH
        | CV_HAAR_SCALE_IMAGE
        ,
        cv::Size(grayImg.size().width / 5, grayImg.size().height / 3));
    if (!faces_.empty())
    {
        std::sort(faces_.begin(), faces_.end(),
            [](const cv::Rect &face0, const cv::Rect &face1)
            -> bool
        {
            return face0.size().area() > face1.size().area();
        });
        //max face rect in faceDetectionRect_
        maxFaceOnSmallImgForFaceDetection_ = faces_[0];
        //face rect on graySmallImgForFaceDetection_
        maxFaceOnSmallImgForFaceDetection_.x += faceDetectionRect_.x;
        maxFaceOnSmallImgForFaceDetection_.y += faceDetectionRect_.y;
        if(FaceAnalysisModel::FaceType::Right==faceType){
                //flip maxFaceOnSmallImgForFaceDetection_ by horizontal
                maxFaceOnSmallImgForFaceDetection_=flipRect( maxFaceOnSmallImgForFaceDetection_
                                                             ,cv::Size(grayImg.size().width,grayImg.size().height)
                                                             ,Frame::Direction::Horizontal);
            }
        //update faceDetectionRect_  to extended current face rect
        faceDetectionRect_= extendRect(maxFaceOnSmallImgForFaceDetection_,1.5);
        //check faceDetectionRect_  to prevent boundary overstepping
        checkBoundary(faceDetectionRect_,cv::Rect(0,0,grayImg.size().width,grayImg.size().height));
        return true;
    }else{
        faceDetectionRect_ = cv::Rect(0, 0, 0, 0);
        return false;
    }
    /*
     * We always keep faceDetectionRect_  and  maxFaceOnSmallImgForFaceDetection_
     * for graySmallImgForFaceDetection with no flpping
*/
}

void Face::generateFaceComponentsColorImage(){
    //regress feature points on front face
    pModel_->regressFeaturePoints(pFrame_->graySmallImgForFaceDetection()
                                  , maxFaceOnSmallImgForFaceDetection_
                                  ,featurePoints_
                                  ,regressedShapeWithoutScale_
                                  ,pFrame_->getScaleForFaceDetection());
    //save right eye feature points
    std::vector<cv::Point> rightEyePoints;
    rightEyePoints.push_back(featurePoints_[4]);
    rightEyePoints.push_back(featurePoints_[5]);
    rightEyePoints.push_back(featurePoints_[6]);
    rightEyePoints.push_back(featurePoints_[7]);
    rightEyePoints.push_back(featurePoints_[8]);
    rightEyePoints.push_back(featurePoints_[9]);
    //set right eye feature points
    rightEye_.featurePoints(rightEyePoints);
    //analyze right eye status
    rightEye_.generateColorImg();
    //save left eye feature points
    std::vector<cv::Point> leftEyePoints;
    leftEyePoints.push_back(featurePoints_[10]);
    leftEyePoints.push_back(featurePoints_[11]);
    leftEyePoints.push_back(featurePoints_[12]);
    leftEyePoints.push_back(featurePoints_[13]);
    leftEyePoints.push_back(featurePoints_[14]);
    leftEyePoints.push_back(featurePoints_[15]);
    //set left eye feature points
    leftEye_.featurePoints(leftEyePoints);
    //analyze left eye status
    leftEye_.generateColorImg();
    //save mouth feature points
    std::vector<cv::Point> mouthChinPoints;
    mouthChinPoints.push_back(featurePoints_[1]);
    mouthChinPoints.push_back(featurePoints_[2]);
    mouthChinPoints.push_back(featurePoints_[16]);
    mouthChinPoints.push_back(featurePoints_[17]);
    mouthChinPoints.push_back(featurePoints_[18]);
    mouthChinPoints.push_back(featurePoints_[19]);
    //set mouth feature points
    mouth_.featurePoints(mouthChinPoints);
    //analyze mouth status
    mouth_.generateColorImg();
}

void Face::analyzeFaceComponentsStatus(){
    rightEye_.analyzeStatus();
    leftEye_.analyzeStatus();
    mouth_.analyzeStatus();
}

void Face::drawFaceComponentsRect(){
    rightEye_.drawMinAreaRect();
    leftEye_.drawMinAreaRect();
    mouth_.drawMinAreaRect();
}

void Face::analyzeFrontFace(){
    generateFaceComponentsColorImage();
    analyzeFaceComponentsStatus();
}
//static int count=0;
void Face::analyzeHeadpose(){
    cv::Mat faceImg=pFrame_->colorImg()(cv::Rect(maxFaceOnSmallImgForFaceDetection_.x/pFrame_->getScaleForFaceDetection()
                                                 ,maxFaceOnSmallImgForFaceDetection_.y/pFrame_->getScaleForFaceDetection()
                                                 ,maxFaceOnSmallImgForFaceDetection_.width/pFrame_->getScaleForFaceDetection()
                                                 ,maxFaceOnSmallImgForFaceDetection_.height/pFrame_->getScaleForFaceDetection()));
    //cv::imshow("faceImg",faceImg);
   /* time_t now=time(0);
    char filename[16];
    sprintf(filename,"temp/%d%d.jpg",now,count++);
    cv::imwrite(filename,faceImg);*/
    headpose_=pModel_->predictHeadposeByDNN(faceImg,cv::Size(64,64));
  // std::cout<<headpose_<<std::endl;
}



void Face::drawFaceRect(const cv::Scalar &color){
    cv::rectangle(pFrame_->colorImg(),cv::Rect(maxFaceOnSmallImgForFaceDetection_.x/pFrame_->getScaleForFaceDetection()
                                               ,maxFaceOnSmallImgForFaceDetection_.y/pFrame_->getScaleForFaceDetection()
                                               ,maxFaceOnSmallImgForFaceDetection_.width/pFrame_->getScaleForFaceDetection()
                                               ,maxFaceOnSmallImgForFaceDetection_.height/pFrame_->getScaleForFaceDetection())
                                               ,color,2);
}

void Face::putText(const std::string &text,const cv::Scalar &color){
    cv::putText(pFrame_->colorImg(),text.c_str(),cv::Point(maxFaceOnSmallImgForFaceDetection_.x/pFrame_->getScaleForFaceDetection()+maxFaceOnSmallImgForFaceDetection_.width/pFrame_->getScaleForFaceDetection()*0.3
                                                           ,maxFaceOnSmallImgForFaceDetection_.y/pFrame_->getScaleForFaceDetection()-3),0,1,color,2);
}

void Face::detectFaces2(){
    pFrame_->equalizeGraySmallImgForFaceDetectionHist();
    resetLastStatus();
    bContainFrontFace_=detectFace(FaceAnalysisModel::FaceType::Front);
    if(!bContainFrontFace_){
        bContainRightFace_=detectFace(FaceAnalysisModel::FaceType::Right);
        if(!bContainRightFace_){
            bContainLeftFace_=detectFace(FaceAnalysisModel::FaceType::Left);
        }
    }
}

void Face::detectFaces()
{
    pFrame_->equalizeGraySmallImgForFaceDetectionHist();
   if(bContainFrontFace_||!bContainFace()){
        //if front face detected or neither front face nor profile face detected,we detect front face
        bContainFrontFace_=detectFace(FaceAnalysisModel::FaceType::Front);
        if(bContainFrontFace_){
           //analyzeFrontFace();
        }
        else
        {
            //no front face detected,detect profile face
            bContainLeftFace_=detectFace(FaceAnalysisModel::FaceType::Left);
            if(!bContainLeftFace_){
               bContainRightFace_=detectFace(FaceAnalysisModel::FaceType::Right);
            }
        }
    }else if(bContainLeftFace_){
            bContainLeftFace_=detectFace(FaceAnalysisModel::FaceType::Left);
            if(!bContainLeftFace_){
                bContainFrontFace_=detectFace(FaceAnalysisModel::FaceType::Front);
                if(bContainFrontFace_){
                    //analyzeFrontFace();
                }else{
                    bContainRightFace_=detectFace(FaceAnalysisModel::FaceType::Right);
                }
            }
    }else if(bContainRightFace_){
        bContainRightFace_=detectFace(FaceAnalysisModel::FaceType::Right);
        if(!bContainRightFace_){
            bContainFrontFace_=detectFace(FaceAnalysisModel::FaceType::Front);
            if(bContainFrontFace_){
                //analyzeFrontFace();
            }else{
                bContainLeftFace_=detectFace(FaceAnalysisModel::FaceType::Left);
            }
        }
    }
}

bool Face::bContainFace(){
    return bContainFrontFace_||bContainLeftFace_||bContainRightFace_;
}
void Face::resetLastStatus(){
    bContainFrontFace_=false;
    bContainLeftFace_=false;
    bContainRightFace_=false;
    faceDetectionRect_=cv::Rect(0,0,0,0);
}
}
