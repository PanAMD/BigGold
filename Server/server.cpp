//
// Created by Monika on 03/09/24.
//

#include "server.h"

using json = nlohmann::json;

inline std::string repair_json(const std::string& input) {
    std::string fixed = input;

    // Remove trailing commas (in arrays or objects)
    fixed = std::regex_replace(fixed, std::regex(",\\s*([}\\]])"), "$1");

    // Add closing braces if necessary (this is a naive fix and might need to be improved)
    if (std::count(fixed.begin(), fixed.end(), '{') > std::count(fixed.begin(), fixed.end(), '}')) {
        fixed += "}";
    }
    return fixed;
}

inline size_t WriteCallbackFunction(void* contents, size_t size, size_t nmemb, std::string* response) {
    size_t totalSize = size * nmemb;
    response->append((char*)contents, totalSize);
    return totalSize;
}


inline size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    std::ofstream* file = (std::ofstream*)userp;
    size_t totalSize = size * nmemb;
    file->write((char*)contents, totalSize);
    return totalSize;
}

inline std::string http_get(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request and capture the return code
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            // If there is an error, print it out
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Success, print the response (for debugging)
            std::cout << "Response: " << response << std::endl;
        }

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return response;
}

void Server::downloadFile()
{
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        std::ofstream file(outputPath.data(), std::ios::binary);
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "CURL initialization failed!" << std::endl;
    }
}

void Server::fetchChanges()
{
    while (true) {
        if (newCommit()) {
            downloadFile();
            updateData(); // Process the new data
        }
        std::this_thread::sleep_for(std::chrono::minutes(5)); // Check every 5 minutes
    }
}

void Server::updateData()
{

}

Server::Server() : downloadThread(std::make_shared<std::thread>(&Server::fetchChanges, this))
{
    lastCommitHash = getCommitHash();
}

Server::~Server()
{
    if (downloadThread && downloadThread->joinable())
    {
        downloadThread->join();
    }
}

void Server::run()
{
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::minutes(5));\
        std::cout<<"Server running..."<<std::endl;
    }
}

std::string Server::getCommitHash()
{
    std::string response = repair_json(http_get(url.data()));
    std::cout <<"Final Json: "<<response<<std::endl;
    response.erase(std::remove(response.begin(), response.end(), '\n'), response.end());

    try
    {
        auto contenty = json::parse(response);
    }
    catch (std::exception & ex)
    {
        std::cout<<"Exception: "<<ex.what()<<std::endl;
    }

    return "";
}

bool Server::newCommit()
{
    std::string latestCommitHash = getCommitHash();
    if (latestCommitHash != lastCommitHash) {
        lastCommitHash = latestCommitHash;
        return true;
    }
    return false;
}


