#include<opencv2/opencv.hpp>
#include<fstream>
#include<random>
#include<time.h>
#include<dirent.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<assert.h>
#include<map>
using namespace std;
const string workDir="/home/lyfe667/projects/make_head_pose_dataset/";
const string modelHome="model/";
const string frontFaceModel="haarcascade_frontalface_alt.xml";
const string profileFaceModel="haarcascade_profileface.xml";
const string pathFile="imgPathsWithLabel.txt";
const string outputDir="OFD_detected_DB_labelled/";

cv::Rect flipRect(const cv::Rect &rect,const cv::Size &imgSize,int flipCode){
        cv::Rect flippedRect(rect);
        if(flipCode>=0){
            flippedRect.x=imgSize.width-flippedRect.x-flippedRect.width;
        }else if(0==flipCode){
            flippedRect.y=imgSize.height-flippedRect.y-flippedRect.height;
        }else{
            cout<<"wrong flipCode"<<endl;
        }
        return flippedRect;
}

/*void detectFaces(const cv::Mat &cImg , vector<cv::Rect> &faces,cv::CascadeClassifier &ffcc,cv::CascadeClassifier &pfcc){
    //detecting front face
    double scale=0.2;
     vector<cv::Rect> frontFaces;
    vector<cv::Rect> leftFaces;
    vector<cv::Rect> rightFaces;
    cv::Mat gImg;
    cv::cvtColor(cImg,gImg,CV_BGR2GRAY);
    cv::equalizeHist(gImg,gImg);
    cv::resize(gImg,gImg,cv::Size(gImg.size().width*scale,gImg.size().height*scale));
   ffcc.detectMultiScale(gImg,frontFaces,1.1,2,0|CV_HAAR_SCALE_IMAGE|CV_HAAR_FIND_BIGGEST_OBJECT);
    for(cv::Rect &face:frontFaces){
        faces.push_back(face);
    }

   pfcc.detectMultiScale(gImg,leftFaces,1.1,2,0|CV_HAAR_SCALE_IMAGE|CV_HAAR_FIND_BIGGEST_OBJECT);
    for(cv::Rect &face:leftFaces){
        faces.push_back(face);
    }

    cv::flip(gImg,gImg,1);
    pfcc.detectMultiScale(gImg,rightFaces,1.1,2,0|CV_HAAR_SCALE_IMAGE);
    cv::flip(gImg,gImg,1);
    for(cv::Rect &face:rightFaces){
            face=flipRect(face,cv::Size(gImg.size().width,gImg.size().height),1);
        faces.push_back(face);
    }
    for(cv::Rect &face:faces){
        face.x/=scale;
        face.y/=scale;
        face.width/=scale;
        face.height/=scale;
    }
}*/

cv::Vec3b randomVec3b(uniform_int_distribution<unsigned> &u,default_random_engine &e){
       cv::Vec3b vec3b;
       vec3b[0]=static_cast<unsigned char>(u(e));
       vec3b[1]=static_cast<unsigned char>(u(e));
       vec3b[2]=static_cast<unsigned char>(u(e));
       return vec3b;
}

/*cv::Mat extendImg(const cv::Mat &cImg,const cv::Size &size){
        if(cImg.size().width>=size.width||cImg.size().height>=size.height){
            return cImg;
        }
        cv::Mat extendedImg(size,CV_8UC3);
      int  dx=(size.width-cImg.size().width)/2;
      int  dy=(size.height-cImg.size().height)/2;
        //fill center area with cImg
        for(int w=0;w<cImg.size().width;w++){
            for(int h=0;h<cImg.size().height;h++){
                extendedImg.at<cv::Vec3b>(dy+h,dx+w)=cImg.at<cv::Vec3b>(h,w);
            }
        }
        uniform_int_distribution<unsigned> wu(0,cImg.size().width);
        uniform_int_distribution<unsigned> hu(0,cImg.size().height);
        uniform_int_distribution<unsigned> du(0,3);
        uniform_int_distribution<unsigned> u(0,255);
        default_random_engine e(time(0));
        //fill area around with uniform random pixel
        for(int w=0;w<extendedImg.size().width;w++){
           for(int h=0;h<dy;h++){
              //  extendedImg.at<cv::Vec3b>(h,w)=randomVec3b(u,e);
              // extendedImg.at<cv::Vec3b>(h,w)=cImg.at<cv::Vec3b>(du(e),wu(e));
               extendedImg.at<cv::Vec3b>(h,w)=cv::Vec3b(255,255,255);
            }
        }
       for(int w=0;w<dx;w++){
           for(int h=0;h<extendedImg.size().height;h++){
                //extendedImg.at<cv::Vec3b>(h,w)=randomVec3b(u,e);
               //extendedImg.at<cv::Vec3b>(h,w)=cImg.at<cv::Vec3b>(hu(e),du(e));
                extendedImg.at<cv::Vec3b>(h,w)=cv::Vec3b(255,255,255);
           }
       }
       for(int w=0;w<extendedImg.size().width;w++){
            for(int h=dy+cImg.size().height;h<extendedImg.size().height;h++){
             //  extendedImg.at<cv::Vec3b>(h,w)=randomVec3b(u,e);
                //extendedImg.at<cv::Vec3b>(h,w)=cImg.at<cv::Vec3b>(cImg.size().height-du(e),wu(e));
               extendedImg.at<cv::Vec3b>(h,w)=cv::Vec3b(255,255,255);
            }
       }
       for(int w=dx+cImg.size().width;w<extendedImg.size().width;w++){
            for(int h=0;h<extendedImg.size().height;h++){
                //extendedImg.at<cv::Vec3b>(h,w)=randomVec3b(u,e);
               // extendedImg.at<cv::Vec3b>(h,w)=cImg.at<cv::Vec3b>(hu(e),cImg.size().width-du(e));
                 extendedImg.at<cv::Vec3b>(h,w)=cv::Vec3b(255,255,255);
            }
       }
       return extendedImg;
}*/

struct PathAndLabel{
  string path;
  int label;
};

void addFace(vector<cv::Rect> &facesToAdd,vector<cv::Rect> &faces,int minWidth,int minHeight){
    if(!facesToAdd.empty()){
        sort(facesToAdd.begin(),facesToAdd.end(),
             [](const cv::Rect &face0,const cv::Rect &face1)
             ->bool
        {
            return face0.size().area()>face1.size().area();
        });
        cv::Rect face=facesToAdd[0];
        if(face.size().width>minWidth&&face.size().height>minHeight){
            faces.push_back(face);
        }else{
            faces.push_back(cv::Rect(0,0,0,0));
        }
    }else{
        faces.push_back(cv::Rect(0,0,0,0));
    }
}

int findNearestFace(const vector<cv::Rect> &faces,int index){
    int left=-1,right=-1;
    for(int i=index-1;i>=0;i--){
        if(faces[i].size().area()){
            left=i;
            break;
        }
    }
    for(int i=index+1;i<faces.size();i++){
        if(faces[i].size().area()){
            right=i;
            break;
        }
    }
    if(-1==left&&-1==right){
        return -1;
    }
    if(-1==left){
        return right;
    }
    if(-1==right){
        return left;
    }
    if(index-left<right-index){
        return left;
    }else{
        return right;
    }
}

void detectFaces(vector<PathAndLabel>&pls, vector<cv::Rect> &faces,cv::CascadeClassifier &ffcc,cv::CascadeClassifier &pfcc){
    double scale=0.2;
    for(int i=0;i<pls.size();i++){
        cv::Mat cImg=cv::imread(workDir+pls[i].path);
        cv::Mat gImg;
        cv::cvtColor(cImg,gImg,CV_BGR2GRAY);
        cv::equalizeHist(gImg,gImg);
        cv::resize(gImg,gImg,cv::Size(gImg.size().width*scale,gImg.size().height*scale));
        if(i<=6){
            vector<cv::Rect> leftFaces;
            pfcc.detectMultiScale(gImg,leftFaces,1.1,2,0|CV_HAAR_SCALE_IMAGE|CV_HAAR_FIND_BIGGEST_OBJECT);
            addFace(leftFaces,faces,gImg.size().width/2,gImg.size().height/2);
        }else if(i<=11){
            vector<cv::Rect> frontFaces;
             ffcc.detectMultiScale(gImg,frontFaces,1.1,2,0|CV_HAAR_SCALE_IMAGE|CV_HAAR_FIND_BIGGEST_OBJECT);
            addFace(frontFaces,faces,gImg.size().width/2,gImg.size().height/2);
        }else{
            vector<cv::Rect> rightFaces;
            cv::flip(gImg,gImg,1);
            pfcc.detectMultiScale(gImg,rightFaces,1.1,2,0|CV_HAAR_SCALE_IMAGE);
            cv::flip(gImg,gImg,1);
            for(cv::Rect &face:rightFaces){
                    face=flipRect(face,cv::Size(gImg.size().width,gImg.size().height),1);
            }
            addFace(rightFaces,faces,gImg.size().width/2,gImg.size().height/2);
        }
    }
    for(int i=0;i<faces.size();i++){
        if(faces[i].size().area()==0){
            int nearest=findNearestFace(faces,i);
            if(-1==nearest){
                faces.clear();
                break;
            }else{
              faces[i]=faces[nearest];
            }
        }
    }
    for(cv::Rect &face:faces){
        face.x/=scale;
        face.y/=scale;
        face.width/=scale;
        face.height/=scale;
    }
}

inline string getPersionNumStr(const string &path){
        string::size_type j=path.find_last_of('_');
        string::size_type i=path.find_last_of('_',j-1);
        string numStr=path.substr(i+1,j-i-1);
        return numStr;
}
//current directory is the directory you run the command
int main(){
    if(0==access((workDir+outputDir).c_str(),F_OK)){
        //if output directory already exist,remove the directory
       system(("rm -rf "+workDir+outputDir).c_str());
    }
    //make the output directory
        mkdir((workDir+outputDir).c_str(),S_IRWXU);
       cv::CascadeClassifier ffcc;
       ffcc.load(workDir+modelHome+frontFaceModel);
       cv::CascadeClassifier pfcc;
       pfcc.load(workDir+modelHome+profileFaceModel);
    ifstream fin(workDir+pathFile);
    assert(fin);
    map<string,vector<PathAndLabel>> all_pls;
    while(true){
        PathAndLabel pl;
        fin>>pl.path>>pl.label;
        if(fin.eof()){
                break;
        }
        string numStr=getPersionNumStr(pl.path);
        if(all_pls.find(numStr)==all_pls.end()){
            all_pls.insert({numStr,vector<PathAndLabel>()});
        }
            all_pls[numStr].push_back(pl);
    }
    for(map<string,vector<PathAndLabel>>::iterator it=all_pls.begin();it!=all_pls.end();it++ ){
        //make one directory for one persion
           mkdir((workDir+outputDir+it->first).c_str(),S_IRWXU);
           vector<cv::Rect> faces;
           detectFaces(it->second,faces,ffcc,pfcc);
           if(!faces.empty()){
               //cut faces
               for(int i=0;i<it->second.size();i++){
                       cv::Mat cImg=cv::imread(workDir+it->second[i].path);
                       char fileName[16];
                       sprintf(fileName,"D_%s_%02d.jpg",it->first.c_str(),it->second[i].label);
                       string filePath=workDir+outputDir+it->first+"/"+fileName;
                       cv::imwrite(filePath,cImg(faces[i]));
                       cout<<filePath<<" writed."<<endl;
                       cv::rectangle(cImg,faces[i],cv::Scalar(0,255,0),2);
                       cv::imshow("cImg",cImg);
                       cv::waitKey(1);
               }
           }
    }
    return 0;
}
