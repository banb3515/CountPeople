//
//  Sender.cpp
//  Sender를 이용하여 데이터를 서버에 전송합니다.
//
//  Created by Printf("HelloWorld!") on 2021/07/29.
//

#include "Sender.hpp"

// Sender를 생성 후 초기화합니다.
Sender::Sender(SenderParam param)
{
    // 매개 변수 유효성 확인
    if (param.server == "")
    {
        cout << "# [Sender] server 값이 잘못되었습니다.\n";
        exit(-1);
    }
    
    this->param = param;
}

// 데이터를 전송합니다.
void Sender::Send(string data)
{
    CURL* curl;
    struct curl_slist* headers = NULL;
    
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    
    if (!curl) {
        cout << "# CURL을 초기화 할 수 없습니다.\n";
        exit(-1);
    }
    
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
    headers = curl_slist_append(headers, "Accept:application/json");
    
    curl_easy_setopt(curl, CURLOPT_URL, param.server.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
    
#if DEBUG == 1
    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK)
        cout << "# 전송 실패 - " << data << "\n";
    else
        cout << "# 전송 성공 - " << data << "\n";
#endif
    
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}
