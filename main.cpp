#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h> // Everything needed for embedding Python in C++
#include <pybind11/stl.h>

namespace py = pybind11;

int main() {
    py::scoped_interpreter guard{}; // Start the Python interpreter

    try {

        // Execute the Python file
        py::module_ main_module = py::module_::import("__main__");
        py::object global_dict = main_module.attr("__dict__");
        py::eval_file("Resources/BigMac.py", global_dict);

        // Access the function from the script
        py::object load_data = global_dict["load_and_preprocess"];
        py::object calculate_big_mac_index = global_dict["calculate_big_mac_index"];

        // Example data for the DataFrame
        py::module_ pd = py::module_::import("pandas");
        auto dataFrame = load_data("Resources/SourceData/big-mac-source-data-v2.csv");

        py::object big_mac_data = pd.attr("DataFrame")(dataFrame);

        // Call the function from the Python script
        py::object result_df = calculate_big_mac_index(big_mac_data);

        // Print the resulting DataFrame
        std::cout << "Resulting DataFrame:\n" << py::str(result_df) << std::endl;

    } catch (std::exception& e) {
        std::cerr << "Python error: " << e.what() << std::endl;
    }

    return 0;
}