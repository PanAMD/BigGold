//
// Created by Monika on 03/09/24.
//

#include "server.h"

using json = nlohmann::json;

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
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void* contents, size_t size, size_t nmemb, std::string* s) {
            s->append((char*)contents, size * nmemb);
            return size * nmemb;
        });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
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
        std::ofstream file(outputPath, std::ios::binary);
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

Server::Server() : downloadThread(std::make_shared<std::thread>([this]{fetchChanges();}))
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
    }
}

std::string Server::getCommitHash()
{
    std::string response = http_get(url.data());
    auto json_response = json::parse(response);
    return json_response["sha"];
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


