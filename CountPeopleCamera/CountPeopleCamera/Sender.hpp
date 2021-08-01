//
//  Sender.hpp
//  Sender를 이용하여 데이터를 서버에 전송합니다.
//
//  Created by Printf("HelloWorld!") on 2021/07/29.
//

#ifndef Sender_hpp
#define Sender_hpp

#include <iostream>
#include <curl/curl.h>

using namespace std;

/* Sender의 매개 변수입니다.
 @param server: 전송할 서버의 URL입니다.
 */
struct SenderParam
{
    string server;
};

// Sender를 이용하여 데이터를 서버에 전송합니다.
class Sender
{
public:
    /* Sender를 생성 후 초기화합니다.
     @param param: Sender의 매개 변수입니다.
     */
    Sender(SenderParam param);
    
    /* 데이터를 전송합니다.
     @param data: 전송할 데이터입니다.
     */
    void Send(string data);
    
private:
    SenderParam param;
};

#endif /* Sender_hpp */
