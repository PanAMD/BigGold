//
// Created by Monika on 9/7/2024.
//

#pragma once

#include <iostream>
#include <filesystem>

#include "../PythonManager/pythonmanager.h"

class BigMacDao
{
private:
    void loadData();
    void calculateBmIndex();

    py::object loadRawData;
    py::object calculateBigMacIndex;

    py::object data;


public:
    BigMacDao();
    ~BigMacDao();
    void updateData();
};


