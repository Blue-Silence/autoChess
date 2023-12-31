/******************************************************/
/*                  文件名：ChatGLM3.cpp              */
/*          功能：利用AI分析场面局势并给出建议        */
/******************************************************/

#include"ChatGLM3.h"

//-----------------------------------------------------//
// 函数参数：无                                        //
// 函数功能：构造函数，初始化CurlWrapper对象           //
// 函数返回值：无（构造函数）                          //
//-----------------------------------------------------//
CurlWrapper::CurlWrapper()
{
    // 初始化cURL库
    curl_global_init(CURL_GLOBAL_DEFAULT);
    // 初始化cURL easy handle
    curl = curl_easy_init();
}

//-----------------------------------------------------//
// 函数参数：无                                        //
// 函数功能：析构函数，清理CurlWrapper对象             //
// 函数返回值：无（析构函数）                          //
//-----------------------------------------------------//
CurlWrapper::~CurlWrapper()
{
    // 清理cURL easy handle
    if (curl)
    {
        curl_easy_cleanup(curl);
    }
    // 清理cURL库
    curl_global_cleanup();
}

//-----------------------------------------------------//
// 函数参数：url（请求URL），userMessage（用户消息）   //
// 函数功能：执行HTTP请求                              //
// 函数返回值：std::string（请求响应）                 //
//-----------------------------------------------------//
std::string  CurlWrapper::performRequest(const std::string& url, const std::string& userMessage)
{
    if (curl) 
    {
        std::string readBuffer;

        // 告知AI游戏基本玩法
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


        // 构建 JSON 数据
        std::string json_data = "{\"model\":\"chatglm3-6b\",\"messages\":[{\"role\":\"user\",\"content\":\"" + prompt + userMessage + "\"}]}";

        // 设置 URL 和其他选项
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)json_data.size());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // 设置头部为 JSON
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // 执行请求并处理响应
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) 
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        else 
        {
            // 清理
            curl_slist_free_all(headers);
            return readBuffer;
        }

        // 清理
        curl_slist_free_all(headers);
    }
    return "";
}

//-----------------------------------------------------//
// 函数参数：response（响应字符串）                    //
// 函数功能：从响应中提取有效内容                      //
// 函数返回值：std::string（提取的内容）               //
//-----------------------------------------------------//
std::string CurlWrapper::extractContent(const std::string& response)
{
    try
    {
        // 解析JSON响应并提取内容
        auto j = json::parse(response);
        return j["choices"][0]["message"]["content"];
    }
    catch (json::parse_error& e)
    {
        // 输出解析错误信息
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return "";
    }
}

