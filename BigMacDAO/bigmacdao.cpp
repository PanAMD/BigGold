//
// Created by Monika on 9/7/2024.
//

#include "bigmacdao.h"


void BigMacDao::loadData()
{
    auto & interpreter = PythonInterpreterManager::getInstance();
    try {
        py::module_ pd = py::module_::import("pandas");
        data = loadRawData("Resources/SourceData/big-mac-source-data-v2.csv");
        py::print(data);
    }catch (py::error_already_set& ex) {
        std::cout<<ex.what()<<std::endl;
    }
}

void BigMacDao::calculateBmIndex()
{
    data = calculateBigMacIndex(data);
    py::print(data);
}

BigMacDao::BigMacDao()
{
    auto & interpreter = PythonInterpreterManager::getInstance();
    std::cout<<std::filesystem::current_path().string()<<std::endl;
    try {
        py::module_ main_module = py::module_::import("__main__");
        py::object global_dict = main_module.attr("__dict__");
        py::exec("print('Python is running correctly!')");
        py::eval_file("Resources/BigMac.py", global_dict);  // Load the Python script

        // Load Python functions
        loadRawData = global_dict["load_and_preprocess"];
        calculateBigMacIndex = global_dict["calculate_big_mac_index"];

        // Ensure the functions are callable
        if (!loadRawData || !calculateBigMacIndex) {
            std::cerr << "Failed to load Python functions." << std::endl;
        }

    } catch (const py::error_already_set& e) {
        std::cerr << "Python error: " << e.what() << std::endl;
        PyErr_Print();  // This prints the full traceback to stderr
    } catch (const std::exception& ex) {
        std::cerr << "C++ error: " << ex.what() << std::endl;
    }
}

BigMacDao::~BigMacDao()
{
}

void BigMacDao::updateData()
{
    loadData();
    calculateBmIndex();
}
