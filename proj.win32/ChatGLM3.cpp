/******************************************************/
/*                  �ļ�����ChatGLM3.cpp              */
/*          ���ܣ�����AI����������Ʋ���������        */
/******************************************************/

#include"ChatGLM3.h"

//-----------------------------------------------------//
// ������������                                        //
// �������ܣ����캯������ʼ��CurlWrapper����           //
// ��������ֵ���ޣ����캯����                          //
//-----------------------------------------------------//
CurlWrapper::CurlWrapper()
{
    // ��ʼ��cURL��
    curl_global_init(CURL_GLOBAL_DEFAULT);
    // ��ʼ��cURL easy handle
    curl = curl_easy_init();
}

//-----------------------------------------------------//
// ������������                                        //
// �������ܣ���������������CurlWrapper����             //
// ��������ֵ���ޣ�����������                          //
//-----------------------------------------------------//
CurlWrapper::~CurlWrapper()
{
    // ����cURL easy handle
    if (curl)
    {
        curl_easy_cleanup(curl);
    }
    // ����cURL��
    curl_global_cleanup();
}

//-----------------------------------------------------//
// ����������url������URL����userMessage���û���Ϣ��   //
// �������ܣ�ִ��HTTP����                              //
// ��������ֵ��std::string��������Ӧ��                 //
//-----------------------------------------------------//
std::string  CurlWrapper::performRequest(const std::string& url, const std::string& userMessage)
{
    if (curl) 
    {
        std::string readBuffer;

        // ��֪AI��Ϸ�����淨
        std::string prompt =
            "Now that you are a master of AutoChess games, I will give you structured information"
            "about the current situation and ask you to provide valuable suggestions to players "
            "based on the current situation."
            "No need to reply with any polite words, just reply with short suggestions directly."
            "Here are some situations: "
            "When a player has a tank hero, you should suggest him try to place the tank in front of other heroes. "
            "When players have too few hero types, you should suggest him buy more different heroes. "
            "When a player's hero level is too low, you should advise him buy the same hero to level up. "
            "When a player has significantly fewer heroes of a certain type than other heroes, you should advise him to buy more of this type of hero."
            "When a player has a mage and a marksman hero, you should recommend that he place them as far back as possible. "
            "Three heroes of the same profession can trigger the strengthening effect. "
            "When a player's level is too low, he should be advised to increase his level. "
            "Please consider the player's real situation and only reply with a few suggestions each time. "
            "The reply must be kept in only one sentence, concise and clear!"
            "Please speak in English always! Please speak in English always!Please speak in English always!"
            "The following is my true situation:";


        // ���� JSON ����
        std::string json_data = "{\"model\":\"chatglm3-6b\",\"messages\":[{\"role\":\"user\",\"content\":\"" + prompt + userMessage + "\"}]}";

        // ���� URL ������ѡ��
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)json_data.size());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // ����ͷ��Ϊ JSON
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // ִ�����󲢴�����Ӧ
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) 
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        else 
        {
            // ����
            curl_slist_free_all(headers);
            return readBuffer;
        }

        // ����
        curl_slist_free_all(headers);
    }
    return "";
}

//-----------------------------------------------------//
// ����������response����Ӧ�ַ�����                    //
// �������ܣ�����Ӧ����ȡ��Ч����                      //
// ��������ֵ��std::string����ȡ�����ݣ�               //
//-----------------------------------------------------//
std::string CurlWrapper::extractContent(const std::string& response)
{
    try
    {
        // ����JSON��Ӧ����ȡ����
        auto j = json::parse(response);
        return j["choices"][0]["message"]["content"];
    }
    catch (json::parse_error& e)
    {
        // �������������Ϣ
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return "";
    }
}

