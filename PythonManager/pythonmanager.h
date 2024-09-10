//
// Created by Monika on 9/8/2024.
//

#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/eval.h>


namespace py = pybind11;

class PythonInterpreterManager {
public:
    static PythonInterpreterManager& getInstance() {
        static PythonInterpreterManager instance; // Guaranteed to be destroyed and instantiated on first use
        return instance;
    }

    // Delete copy constructor and assignment operator
    PythonInterpreterManager(PythonInterpreterManager const&) = delete;
    void operator=(PythonInterpreterManager const&) = delete;

    pybind11::scoped_interpreter& getInterpreter() { return *interpreter; }

private:
    PythonInterpreterManager() : interpreter(std::make_unique<pybind11::scoped_interpreter>()) {}
    ~PythonInterpreterManager() {}

    std::unique_ptr<pybind11::scoped_interpreter> interpreter;
};