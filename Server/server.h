//
// Created by Monika on 03/09/24.
//

#pragma once

#include <thread>
#define CURL_STATICLIB
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <regex>

#include "nlohmann/json.hpp"

constexpr std::string_view url= "https://api.github.com/repos/TheEconomist/big-mac-data/commits/master";
constexpr std::string_view outputPath = "./Resources/SourceData/big-mac-source-data-v2.csv";

class Server
{
private:
    bool newCommit();
    std::string getCommitHash();
    void downloadFile();
    void fetchChanges();
    void updateData();

    std::shared_ptr<std::thread> downloadThread;

    std::string lastCommitHash;

public:
    Server();
    ~Server();
    void run();
};

