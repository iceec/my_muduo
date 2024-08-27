#pragma once

#include<memory>
#include<functional>
#include<TimeStamp.hpp>

class TcpConnection;
class Buffer;


using TcpConnectionPtr = std::shared_ptr<TcpConnection> ; // connection是可以多个指向的
using Connection_Call_Back = std::function<void(TcpConnectionPtr &)>;
using Close_Call_Back = std::function<void(TcpConnectionPtr &)>;
using Write_Complete_Call_Back = std::function<void(TcpConnectionPtr &)>;
using High_Water_Mark_Call_Back = std::function<void(TcpConnectionPtr &,size_t)>;  // 还不知道是干什么的

using Message_Call_Back = std::function<void(TcpConnectionPtr &,Buffer &,TimeStamp)>; // 对于接受消息的回调 

