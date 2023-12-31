/******************************************************/
/*                  �ļ�����ChatGLM3.h                */
/*          ���ܣ�����AI����������Ʋ���������        */
/******************************************************/

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>


using json = nlohmann::json;

class CurlWrapper 
{
public:
    
    CurlWrapper();
    ~CurlWrapper();

    // ��������
    std::string performRequest(const std::string& url, const std::string& userMessage);

    // ��ȡAI�ظ���Ϣ
    std::string extractContent(const std::string& response);

    // �ص�����
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s)
    {
        size_t newLength = size * nmemb;
        try
        {
            s->append((char*)contents, newLength);
            return newLength;
        }
        catch (std::bad_alloc& e) 
        {
            return 0;
        }
    }

private:
    CURL* curl;
};

