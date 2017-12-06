#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H
#include<sys/msg.h>
#include<cstring>
#include"Exception.h"

template<typename DataType>
class Message<DataType>{
private:
    long int msgType_;
    char msgData_[sizeof(DataType)];
public:
    Message(){

    }
    Message(DataType &data,long int msgType=0)
        :msgType_(msgType)
    {
        memcpy(static_cast<void *>(msgData_),static_cast<void *>(&data),sizeof(DataType));
    }
    ~Message(){

    }
public:
    void setType(long int msgType){
        msgType_=msgType;
    }
    void setData(DataType &data){
        memcpy(static_cast<void *>(msgData_),static_cast<void *>(&data),sizeof(DataType));
    }
    long int getType(){
        return msgType_;
    }
    DataType getData(){
        return *(static_cast<DataType *>(msgData_));
    }
    int getDataSize(){
        return sizeof(DataType);
    }
};

template<typename DataType>
class MessageQueue<DataType>{
private:
    key_t msgKey_;
    int msgID_;
public:
    MessageQueue(key_t key,int msgflg=0666|IPC_CREAT)
        :msgKey_(key),msgID_(-1)
    {
        msgID_=msgget(msgKey_ , msgflg);
        CHECK<MessageQueueException>(-1!=msgID_,"msgget exception");
    }
    int push(DataType &data,long int msgType=0,int msgflg=0){
          Message<DataType> msg(data,msgType);
         return msgsnd(msgID_,static_cast<void *>(&msg),msg.getDataSize(),msgflg);
    }
    int pop(Message<DataType> &msg,long int msgType=0,int msgflg=0){
        return msgrcv(msgID_,static_cast<void *>(&msg),msg.getDataSize(),msgType,msgflg);
    }
    int getStatus(struct msgid_ds *status){
        return msgctl(msgID_,IPC_STAT,status);
    }
    int setStatus(struct msgid_ds *status){
        return msgctl(msgID_,IPC_SET,status);
    }
    int deleteQueue(){
        return msgctl(msgID_,IPC_RMID,0);
    }
};

#endif // MESSAGEQUEUE_H
