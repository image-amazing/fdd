#include"FatigueDetectionFrameSequenceProcessor.h"
#include<limits.h>
//#include<glog/logging.h>
#include<time.h>
#include<sstream>
#include<fstream>
#include<exception>

using namespace forwarder;

namespace fdd{

FatigueDetectionFrameSequenceProcessor::FatigueDetectionFrameSequenceProcessor(const cv::Ptr<FaceAnalysisModel> & pFaceAnalysisModel,key_t msgKey)
    :pFaceAnalysisModel_(pFaceAnalysisModel) ,face_(cv::Ptr<Frame>(&frame_),pFaceAnalysisModel_)
    ,videoFolder_("./"),eeFolder_("./"),meFolder_("./"),resultFolder_("./"),msgQue_(msgKey)
{

}

FatigueDetectionFrameSequenceProcessor::~FatigueDetectionFrameSequenceProcessor()
{
    vm_.release();
    eevm_.release();
    mevm_.release();
    msgQue_.deleteQueue();
}

inline void FatigueDetectionFrameSequenceProcessor::updateEyeParameters(EyeParameters &eyeParam,FaceComponent::Status eyeStatus)
{
	eyeParam.eyeStatusStr_ = "EYE STATUS:";
	if (FaceComponent::Status::open==eyeStatus)
	{
		eyeParam.eyeStatusStr_ += "OPEN";
		eyeParam.eyeStatusColor_ = cv::Scalar(0,255,0);
		eyeParam.openEyeFrameCount_++;
	}
	else
	{
		eyeParam.eyeStatusStr_ += "CLOSE";
		eyeParam.eyeStatusColor_ = cv::Scalar(0,0,255);
		eyeParam.closeEyeFrameCount_++;
	}
}

inline void FatigueDetectionFrameSequenceProcessor::updateMouthParameters(MouthParameters &mouthParam,FaceComponent::Status mouthStatus)
{
	mouthParam.mouthStatusStr_ = "MOUTH STATUS:";
	if (FaceComponent::Status::open==mouthStatus)
	{
		mouthParam.mouthStatusStr_ += "OPEN";
		mouthParam.mouthStatusColor_ = cv::Scalar(0,0,255);
		mouthParam.openMouthFrameCount_++;
	}
	else
	{
		mouthParam.mouthStatusStr_ += "CLOSE";
		mouthParam.mouthStatusColor_ = cv::Scalar(0,255,0);
		mouthParam.closeMouthFrameCount_++;
	}
}

inline void FatigueDetectionFrameSequenceProcessor::updateFaceParameters(FaceParameters &faceParam,FaceAnalysisModel::FaceType faceType){
    if(FaceAnalysisModel::FaceType::Front==faceType){
        faceParam.directionStr_="Front";
        faceParam.color_=cv::Scalar(0,255,0);
        faceParam.frontFaceFrameCount_++;
    }else if(FaceAnalysisModel::FaceType::Left==faceType){
        faceParam.directionStr_="Left";
        faceParam.color_=cv::Scalar(0,255,255);
        faceParam.leftFaceFrameCount_++;
    }else if(FaceAnalysisModel::FaceType::Right==faceType){
        faceParam.directionStr_="Right";
        faceParam.color_=cv::Scalar(0,0,255);
        faceParam.rightFaceFrameCount_++;
    }else{
        throw std::exception();
    }
}

void FatigueDetectionFrameSequenceProcessor::countYawnFrame(FaceComponent::Status mouthStatus)
{
	if (yawnParam_.lastMouthStatus_!=mouthStatus)
	{
        if (FaceComponent::Status::open==mouthStatus)
		{
            if(0==yawnParam_.interuptCloseMouthFrameCount_){
                //close mouth in last frame,open mouth in current frame,start opening mouth
                //start timing
                yawnParam_.startOpenMouthTime_ = time(0);
                //start counting
                yawnParam_.currentOpenMouthFrameCount_++;
            }else{
                yawnParam_.interuptCloseMouthFrameCount_=0;
            }
		}
		else
		{
            //open mouth in last frame,close mouth in current frame,end opening mouth
            //start counting yawn interrupt  frame
			yawnParam_.interuptCloseMouthFrameCount_++;
		}
		yawnParam_.lastMouthStatus_ = mouthStatus;
	}
	else
	{
        //same mouth status in last and current frame
		if (FaceComponent::Status::open==mouthStatus)
		{
            //opening mouth
			yawnParam_.currentOpenMouthFrameCount_++;
		}
		else
        {//closed mouth
			if (yawnParam_.interuptCloseMouthFrameCount_>0)
            {
                //during yawn interruption frame counting stage
				yawnParam_.interuptCloseMouthFrameCount_++;
				if (yawnParam_.interuptCloseMouthFrameCount_>=3)
				{
					time_t endOpenMouthTime = time(0);
                    if (endOpenMouthTime-yawnParam_.startOpenMouthTime_>yawnParam_.yawnContinueTimeThreshold_)
                    {
                        //open mouth time exceeds 2s,yawn detected
						yawnParam_.yawnFrameCount_ += yawnParam_.currentOpenMouthFrameCount_;
						yawnParam_.yawnCount_++;
                      //  LOG(INFO)<<"yawn";
						std::cout << "yawn" << std::endl;
                        std::string resultFileName= outputResult(DriverStatus::yawn);
                        forwarder:FatigueMessage fmsg;
                        fmsg.setResultFileName(resultFileName);
                        msgQue_.push<FatigueMessage>(fmsg,static_cast<int>(DriverStatus::yawn));
					}
					yawnParam_.currentOpenMouthFrameCount_ = 0;
					yawnParam_.interuptCloseMouthFrameCount_ = 0;
				}
			}
		}
	}
}

bool FatigueDetectionFrameSequenceProcessor::detectDistractionByTimeInterval(FaceAnalysisModel::FaceType faceDirection,int interval){
    if (faceParam_.lastFaceDirection_!=faceDirection)
    {
        if (FaceAnalysisModel::FaceType::Front!=faceDirection)
        {
            if(0==faceParam_.interuptDistractionFrameCount_){
                //abnormal faceDirection
                //start timing
                faceParam_.startDistractionTime_ = time(0);
            }else{
                //reset distraction interruption count
                faceParam_.interuptDistractionFrameCount_=0;
            }
        }
        else
        {
            //distraction in last frame,front face in current frame,
            //distraction interupt frame start counting
            faceParam_.interuptDistractionFrameCount_++;
        }
        //yawnParam_.lastMouthStatus_ = mouthStatus;
        faceParam_.lastFaceDirection_=faceDirection;
    }
    else
    {
        //face directions in last frame and current frame are same
        if (FaceAnalysisModel::FaceType::Front!=faceDirection)
        {
            time_t currentDistractionTime=time(0);
            if(currentDistractionTime-faceParam_.startDistractionTime_>interval){
                   //std::cout<<"distraction detected  "<<faceDirection<<std::endl;
                    faceParam_.startDistractionTime_=time(0);
                    return true;
            }
        }
        else
        {//front face
            if(faceParam_.interuptDistractionFrameCount_>0)
            {
                //distraction interuption frame is counting
                faceParam_.interuptDistractionFrameCount_++;
                if(faceParam_.interuptDistractionFrameCount_>=3)
                {
                    faceParam_.interuptDistractionFrameCount_=0;
                }
            }
        }
    }
    return false;
}

bool FatigueDetectionFrameSequenceProcessor::judgeFatigueByEye(EyeParameters &eyeParams){
    eyeParams.perclos_ = static_cast<float>(eyeParams.closeEyeFrameCount_) / static_cast<float>(eyeParams.closeEyeFrameCount_ + eyeParams.openEyeFrameCount_);
    eyeParams.openEyeFrameCount_ = 0;
    eyeParams.closeEyeFrameCount_ = 0;
    eyeParams.perclosColor_ = cv::Scalar(0, 255, 0);
    if (eyeParams.perclos_>eyeParams.perclosThreshold_)
    {
        eyeParams.perclosColor_ = cv::Scalar(0, 0, 255);
        return true;
    }
    return false;
}

bool FatigueDetectionFrameSequenceProcessor::judgeFatigueByMouth()
{
    yawnParam_.yawnFrameRate_ = static_cast<float>(yawnParam_.yawnFrameCount_) / static_cast<float>(mouthParam_.openMouthFrameCount_ + mouthParam_.closeMouthFrameCount_);
	mouthParam_.openMouthFrameCount_ = 0;
	mouthParam_.closeMouthFrameCount_ = 0;
	yawnParam_.yawnFrameCount_ = 0;
	yawnParam_.yawnCount_ = 0;
	yawnParam_.yawnFrameRateColor_ = cv::Scalar(0,255,0);
	if (yawnParam_.yawnFrameRate_>yawnParam_.yawnFrameRateThreshold_)
	{
		yawnParam_.yawnFrameRateColor_ = cv::Scalar(0, 0, 255);
		return true;
	}
	return false;
}

bool FatigueDetectionFrameSequenceProcessor::detectDistractionByFrameRate(float distractionThreshold){
    float distractionFrameRate=static_cast<float>(faceParam_.leftFaceFrameCount_+faceParam_.rightFaceFrameCount_)
            /static_cast<float>(faceParam_.leftFaceFrameCount_+faceParam_.rightFaceFrameCount_+faceParam_.frontFaceFrameCount_);
    faceParam_.leftFaceFrameCount_=0;
    faceParam_.rightFaceFrameCount_=0;
    faceParam_.frontFaceFrameCount_=0;
    if(distractionFrameRate>distractionThreshold){
        return true;
    }
    return false;
}

void FatigueDetectionFrameSequenceProcessor::updateFPS()
{
	systemParam_.fps_ = systemParam_.faceFrameCount_ - systemParam_.faceFrameCountAtLastSecond_;
    if(0==systemParam_.fps_){
        systemParam_.fps_=8;
    }
}

inline std::string FatigueDetectionFrameSequenceProcessor::outputResult(DriverStatus status){
    std::string strTime=getNowTimeStr();
    std::ostringstream sout;
     sout<<verticalID_<<"_"<<strTime<<"_FATIGUE.txt";
     std::string resultFileName=sout.str();
     std::ofstream fout((resultFolder_+"/"+resultFileName).c_str());
     if(!fout.is_open()){
         std::cout<<"fail to open"<<resultFolder_+"/"<<resultFileName<<std::endl;
         throw std::exception();
     }
     fout<<verticalID_<<std::endl;
     fout<<strTime<<std::endl;
     fout<<driverID_<<std::endl;
     fout<<status<<std::endl;
     fout.close();
     return resultFileName;
}

#ifdef WITH_SCREEN
void FatigueDetectionFrameSequenceProcessor::printParamsToLeft(cv::Mat &colorImg)
{//use relative cooridnate to fit variation of image size
    cv::putText(colorImg, ("RIGHT "+rightEyeParam_.eyeStatusStr_).c_str(), cv::Point(40, 20), 0, 0.5, rightEyeParam_.eyeStatusColor_,2);
    cv::putText(colorImg, ("LEFT "+leftEyeParam_.eyeStatusStr_).c_str(), cv::Point(40, 40),0,0.5,leftEyeParam_.eyeStatusColor_,2);
    cv::putText(colorImg,("TWO "+eyesParam_.eyeStatusStr_).c_str(),cv::Point(40,60),0,0.5,eyesParam_.eyeStatusColor_,2);
    cv::putText(colorImg, mouthParam_.mouthStatusStr_.c_str(), cv::Point(40, 80), 0, 0.5, mouthParam_.mouthStatusColor_,2);
    cv::putText(colorImg, "--------------", cv::Point(40, 100), 0, 0.5, cv::Scalar(255, 255, 255),2);
	char perclos_buf[32];
	sprintf(perclos_buf, "RIGHT PERCLOSE:%.2f", rightEyeParam_.perclos_);
    cv::putText(colorImg, perclos_buf, cv::Point(40, 120), 0, 0.5, rightEyeParam_.perclosColor_,2);
	sprintf(perclos_buf, "LEFT PERCLOSE:%.2f", leftEyeParam_.perclos_);
    cv::putText(colorImg, perclos_buf, cv::Point(40, 140), 0, 0.5, leftEyeParam_.perclosColor_, 2);
    sprintf(perclos_buf,"EYES PERCLOSE:%.2f",eyesParam_.perclos_);
     cv::putText(colorImg, perclos_buf, cv::Point(40, 160), 0, 0.5, eyesParam_.perclosColor_, 2);
	char yawnFrameRate_buf[32];
	sprintf(yawnFrameRate_buf, "YAWN FRAME RATE:%.2f", yawnParam_.yawnFrameRate_);
    cv::putText(colorImg, yawnFrameRate_buf, cv::Point(40, 180), 0, 0.5, yawnParam_.yawnFrameRateColor_,2);
	char yawnCount_buf[32];
	sprintf(yawnCount_buf, "YAWN COUNT:%d", yawnParam_.yawnCount_);
    cv::putText(colorImg, yawnCount_buf, cv::Point(40, 200), 0, 0.5, cv::Scalar(0, 255, 255),2);
}

void FatigueDetectionFrameSequenceProcessor::printParamsToMiddle(cv::Mat &colorImg){
    char faceTypeBuf[32];
    sprintf(faceTypeBuf,"Face Direction: %s",faceParam_.directionStr_.c_str());
    cv::putText(colorImg,faceTypeBuf,cv::Point(240,20),0,0.5,faceParam_.color_,2);
}

void FatigueDetectionFrameSequenceProcessor::printParamsToRight(cv::Mat &colorImg){
    //show system time
    struct tm *lt = localtime(&systemParam_.nowTime_);
    char time_buf[32];
    sprintf(time_buf, "%d/%d/%d:%d:%02d:%d", lt->tm_year + 1900, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
    cv::putText(colorImg, time_buf, cv::Point(450, 20), 0, 0.5, cv::Scalar(255, 0, 0),2);
    char fps_buf[32];
    sprintf(fps_buf, "FPS:%d", systemParam_.fps_);
    cv::putText(colorImg, fps_buf, cv::Point(450, 40), 0, 0.5, cv::Scalar(255, 0, 0),2);
    char faceDetectionTime_buf[32];
    sprintf(faceDetectionTime_buf, "FDT:%.3lf",pFaceAnalysisModel_->faceDetectionTime());
    cv::putText(colorImg, faceDetectionTime_buf, cv::Point(450, 60), 0, 0.5, cv::Scalar(255, 0, 0),2);
    char faceAlignmentTime_buf[32];
    sprintf(faceAlignmentTime_buf, "FAT:%.3lf", pFaceAnalysisModel_->faceAlignmentTime());
    cv::putText(colorImg, faceAlignmentTime_buf, cv::Point(450, 80), 0, 0.5, cv::Scalar(255, 0, 0),2);
}
#endif

void FatigueDetectionFrameSequenceProcessor::initProcessor(){

    face_.rightEye().colorImgScale(1.3);
    face_.leftEye().colorImgScale(1.3);

    systemParam_.aveRawFPSInOneMinute_=8;

    vm_.setFourcc(CV_FOURCC('M','P','4','2'));
    vm_.setFrameSize(cv::Size(getFrameWidth(),getFrameHeight()));
    vm_.setBAdd(true);
    char path[PATH_LENGTH];
    sprintf(path, "%s/%d_%s_FATIGUE.avi",videoFolder_.c_str(), driverID_,getTimeStr(systemParam_.nowTime_).c_str());
    openVideo(vm_,path,systemParam_.aveRawFPSInOneMinute_);
    eevm_.setFourcc(CV_FOURCC('M','P','4','2'));
    eevm_.setFrameSize(cv::Size(getFrameWidth(),getFrameHeight()));
    eevm_.setBAdd(true);
    sprintf(path, "%s/%d_%s_FATIGUE.avi",eeFolder_.c_str(), driverID_,getTimeStr(systemParam_.nowTime_).c_str());
    openVideo(eevm_,path,systemParam_.aveRawFPSInOneMinute_);
    mevm_.setFourcc(CV_FOURCC('M','P','4','2'));
    mevm_.setFrameSize(cv::Size(getFrameWidth(),getFrameHeight()));
    mevm_.setBAdd(true);
    sprintf(path, "%s/%d_%s_FATIGUE.avi",meFolder_.c_str(), driverID_,getTimeStr(systemParam_.nowTime_).c_str());
    openVideo(mevm_,path,systemParam_.aveRawFPSInOneMinute_);
}

void FatigueDetectionFrameSequenceProcessor::beforeProcess(){

}

void FatigueDetectionFrameSequenceProcessor::process(cv::Mat rawFrame)
{
	checkRawFrameCount();
	systemParam_.nowTime_ = time(0);
	systemParam_.rawFrameCount_++;
	if (systemParam_.lastSecond_ != systemParam_.nowTime_)
	{
		if (0 == systemParam_.nowTime_ % 60)
		{
            //calculate average fps in one minute
			updateAveRawFPSInOneMinute();
			systemParam_.rawFrameCountAtLastMinute_ = systemParam_.rawFrameCount_;
		}
        if (0 == systemParam_.nowTime_ % 20)
		{
            //create a new video file per 20s
            vm_.release();
            if (vm_.getVideoPath() != ""&&systemParam_.fatigueFrameCount_<=10)
			{
                //if abnormal frame count less than 10,remove the file
                remove(vm_.getVideoPath().c_str());
			}
            char videoPath[PATH_LENGTH];
            sprintf(videoPath, "%s/%d_%s_FATIGUE.avi",videoFolder_.c_str(), driverID_,getTimeStr(systemParam_.nowTime_).c_str());
            //std::cout<<videoPath<<std::endl;
            /*vm_.setVideoPath(videoPath);
			vm_.setFPS(systemParam_.aveRawFPSInOneMinute_);
            vm_.open();*/
            openVideo(vm_,videoPath,systemParam_.aveRawFPSInOneMinute_);
            //reset abnormal frame count
			systemParam_.fatigueFrameCount_ = 0;
        }
	}
	frame_ = rawFrame;
    face_.detectFaces2();
    if(face_.bContainFace()){
        //face_.analyzeHeadpose();
        systemParam_.faceFrameCount_++;
        if(systemParam_.nowTime_!=systemParam_.lastSecond_
                &&0==systemParam_.nowTime_%faceParam_.distractionDetectionInterval_){
                if(detectDistractionByFrameRate(faceParam_.distractionFrameRateThreshold_)){
                  // LOG(INFO)<<"frequent distraction detected!!!";
                   std::cout<<"frequent distraction detected!!!"<<std::endl;
                   std::string resultFileName=outputResult(DriverStatus::distraction);
                    FatigueMessage fmsg;
                    fmsg.setResultFileName(resultFileName);
                    msgQue_.push<FatigueMessage>(fmsg,static_cast<int>(DriverStatus::distraction));
                }
            }
    }
    if (face_.bContainFrontFace())
    {
        if(detectDistractionByTimeInterval(FaceAnalysisModel::FaceType::Front,faceParam_.distractionLastedThreshold_))
        {
            //LOG(INFO)<<"distraction detected!!!";
            std::cout<<"distraction detected!!!"<<std::endl;
            std::string resultFileName=outputResult(DriverStatus::distraction);
            FatigueMessage fmsg;
            fmsg.setResultFileName(resultFileName);
            msgQue_.push<FatigueMessage>(fmsg,static_cast<int>(DriverStatus::distraction));
        }
        face_.analyzeFrontFace();
        //face_.drawFaceComponentsRect();
        updateFaceParameters(faceParam_,FaceAnalysisModel::FaceType::Front);
		vm_.write(frame_.rawFrame());
        //save eyes evidence
        eevm_.write(frame_.colorImg());
        //save mouth evidence
        mevm_.write(frame_.colorImg());

        //update right eye parameters
        updateEyeParameters(rightEyeParam_,face_.rightEye().status());
#ifdef WITH_SCREEN
        //draw right eye rectangle
        if(FaceComponent::Status::close==face_.rightEye().status()){
            face_.rightEye().drawMinAreaRect(FaceComponent::abnormalColor);
            face_.rightEye().putText("close",FaceComponent::abnormalColor);
        }else{
            face_.rightEye().drawMinAreaRect(FaceComponent::normalColor);
            face_.rightEye().putText("open",FaceComponent::normalColor);
        }
#endif
        //update left eye parameters
        updateEyeParameters(leftEyeParam_,face_.leftEye().status());
#ifdef  WITH_SCREEN
        //draw left eye rectangle
        if(FaceComponent::Status::close==face_.leftEye().status()){
                face_.leftEye().drawMinAreaRect(FaceComponent::abnormalColor);
                face_.leftEye().putText("close",FaceComponent::abnormalColor);
        }else{
               face_.leftEye().drawMinAreaRect(FaceComponent::normalColor);
               face_.leftEye().putText("open",FaceComponent::normalColor);
        }
#endif
        //update two eyes parameters
        if (face_.rightEye().status()==FaceComponent::Status::close&&face_.leftEye().status()==FaceComponent::Status::close) {
			systemParam_.fatigueFrameCount_++;
            updateEyeParameters(eyesParam_,FaceComponent::Status::close);
        }else{
            updateEyeParameters(eyesParam_,FaceComponent::Status::open);
        }
        //update mouth parameters
		updateMouthParameters(mouthParam_,face_.mouth().status());
#ifdef WITH_SCREEN
        //draw mouth rectangle
        if(FaceComponent::Status::close==face_.mouth().status()){
            face_.mouth().drawMinAreaRect(FaceComponent::normalColor);
            face_.mouth().putText("close",FaceComponent::normalColor);
        }else{
            face_.mouth().drawMinAreaRect(FaceComponent::abnormalColor);
            face_.mouth().putText("open",FaceComponent::abnormalColor);
        }
#endif
		if (face_.mouth().status()==FaceComponent::Status::open) {
			systemParam_.fatigueFrameCount_++;
		}
		countYawnFrame(face_.mouth().status());
		if (systemParam_.lastSecond_!=systemParam_.nowTime_)
        {//1s passed
			updateFPS();
			systemParam_.faceFrameCountAtLastSecond_ = systemParam_.faceFrameCount_;
			if (0==systemParam_.nowTime_%rightEyeParam_.eyeJudgingInterval_)
			{
                eevm_.release();
                bool bFatigueRight = judgeFatigueByEye(rightEyeParam_);
                bool bFatigueLeft = judgeFatigueByEye(leftEyeParam_);
                if(judgeFatigueByEye(eyesParam_))
				{
                  //  LOG(INFO)<<"sleepy!!!";
                    std::cout << "************************" <<std:: endl;
                    std::cout << "sleepy!!!" << std::endl;
                    std::cout << "************************" << std::endl;
                   std::string resultFileName = outputResult(DriverStatus::sleepy);
                    FatigueMessage fmsg;
                    fmsg.setResultFileName(resultFileName);
                    fmsg.setEvidenceName(eevm_.getVideoName());
                    msgQue_.push<FatigueMessage>(fmsg,static_cast<int>(DriverStatus::sleepy));
                }else{
                    remove(eevm_.getVideoPath().c_str());
                }
                char eePath[PATH_LENGTH];
                //sprintf(eePath, "%s%ld%d.avi",eeFolder_.c_str() , systemParam_.nowTime_, systemParam_.rawFrameCount_);
                sprintf(eePath, "%s/%d_%s_FATIGUE.avi",eeFolder_.c_str() , driverID_,getNowTimeStr().c_str());
               /* eevm_.setVideoPath(eePath);
                eevm_.setFPS(systemParam_.aveRawFPSInOneMinute_);
                eevm_.open();*/
                openVideo(eevm_,eePath,systemParam_.aveRawFPSInOneMinute_);
			}
			if (0==systemParam_.nowTime_%mouthParam_.mouthJudgingInterval)
			{
                mevm_.release();
                if (judgeFatigueByMouth())
				{
                    //LOG(INFO)<< "frequent yawn!!!";
                    std::cout << "************************" << std::endl;
                    std::cout << "frequent yawn!!!" << std::endl;
                    std::cout << "************************" << std::endl;
                    std::string resultFileName = outputResult(DriverStatus::frequentYawn);
                    FatigueMessage fmsg;
                    fmsg.setResultFileName(resultFileName);
                    fmsg.setEvidenceName(mevm_.getVideoName());
                    msgQue_.push<FatigueMessage>(fmsg,static_cast<int>(DriverStatus::frequentYawn));
                }else{
                    remove(mevm_.getVideoPath().c_str());
                }
                char mePath[PATH_LENGTH];
                sprintf(mePath, "%s/%d_%s_FATIGUE.avi",meFolder_.c_str() , driverID_ ,getNowTimeStr().c_str());
                /*mevm_.setVideoPath(mePath);
                mevm_.setFPS(systemParam_.aveRawFPSInOneMinute_);
                mevm_.open();*/
                openVideo(mevm_,mePath,systemParam_.aveRawFPSInOneMinute_);
			}
		}
#ifdef WITH_SCREEN
        face_.drawFaceRect(Face::frontColor);
        face_.putText("front",Face::frontColor);
        printParamsToLeft(frame_.colorImg());
        printParamsToMiddle(frame_.colorImg());
        printParamsToRight(frame_.colorImg());
#endif
    }else if(face_.bContainLeftFace()){
    if(detectDistractionByTimeInterval(FaceAnalysisModel::FaceType::Left,faceParam_.distractionLastedThreshold_))
    {
       // LOG(INFO)<<"distraction detected!!!  left";
        std::cout<<"distraction detected!!!  left"<<std::endl;
        std::string resultFileName= outputResult(DriverStatus::distraction);
        FatigueMessage fmsg;
         fmsg.setResultFileName(resultFileName);
         msgQue_.push<FatigueMessage>(fmsg,static_cast<int>(DriverStatus::distraction));
    }
    countYawnFrame(FaceComponent::Status::close);
    updateFaceParameters(faceParam_,FaceAnalysisModel::FaceType::Left);
#ifdef WITH_SCREEN
     face_.drawFaceRect(Face::leftColor);
     face_.putText("left",Face::leftColor);
    printParamsToMiddle(frame_.colorImg());
    printParamsToRight(frame_.colorImg());
#endif
    }else if(face_.bContainRightFace()){
    if(detectDistractionByTimeInterval(FaceAnalysisModel::FaceType::Right,faceParam_.distractionLastedThreshold_)){
       // LOG(INFO)<<"distraction detected!!! right";
        std::cout<<"distraction detected!!! right"<<std::endl;
        std::string resultFileName = outputResult(DriverStatus::distraction);
        FatigueMessage fmsg;
        fmsg.setResultFileName(resultFileName);
        msgQue_.push<FatigueMessage>(fmsg,static_cast<int>(DriverStatus::distraction));
    }
    countYawnFrame(FaceComponent::Status::close);
    updateFaceParameters(faceParam_,FaceAnalysisModel::FaceType::Right);
#ifdef  WITH_SCREEN
    face_.drawFaceRect(Face::rightColor);
    face_.putText("right",Face::rightColor);
    printParamsToMiddle(frame_.colorImg());
    printParamsToRight(frame_.colorImg());
#endif
    }
	if (systemParam_.lastSecond_ != systemParam_.nowTime_)
	{
		systemParam_.lastSecond_ = systemParam_.nowTime_;
	}
#ifdef  WITH_SCREEN
	cv::imshow("colorImg", frame_.colorImg());
	cv::waitKey(1);
#endif
}

void FatigueDetectionFrameSequenceProcessor::afterProcess(){

}

inline void FatigueDetectionFrameSequenceProcessor::updateAveRawFPSInOneMinute()
{
	systemParam_.aveRawFPSInOneMinute_ = round(static_cast<float>(systemParam_.rawFrameCount_-systemParam_.rawFrameCountAtLastMinute_)/60.0f);
    if(0==systemParam_.aveRawFPSInOneMinute_){
        systemParam_.aveRawFPSInOneMinute_=8;
    }
}

inline void FatigueDetectionFrameSequenceProcessor::resetParameters()
{
    rightEyeParam_.reset();
    leftEyeParam_.reset();
    eyesParam_.reset();
    mouthParam_.reset();
    yawnParam_.reset();
    systemParam_.reset();
    faceParam_.reset();
}
inline void FatigueDetectionFrameSequenceProcessor::checkRawFrameCount()
{
	if (systemParam_.rawFrameCount_>UINT_MAX-10)
	{
		resetParameters();
	}
}
}
