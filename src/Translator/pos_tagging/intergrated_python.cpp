#include <iostream>
#include <Python.h>

std::string pos_tag(const std::string& sentence) {
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    std::string result;

    // Initialize the Python Interpreter
    Py_Initialize();

    // Load your Python script
    pName = PyUnicode_DecodeFSDefault("your_python_script_name_without_extension");
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, "your_function_name_in_python_script");
        
        if (PyCallable_Check(pFunc)) {
            pArgs = PyTuple_Pack(1, PyUnicode_FromString(sentence.c_str()));
            pValue = PyObject_CallObject(pFunc, pArgs);
            result = PyUnicode_AsUTF8(pValue);
            Py_DECREF(pArgs);
            Py_DECREF(pValue);
        } else {
            std::cerr << "Cannot find function" << std::endl;
        }
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        std::cerr << "Failed to load Python script." << std::endl;
    }

    Py_Finalize();

    return result;
}

int main() {
    std::string sentence = "John's big idea isn't all that bad.";
    std::string tagged_sentence = pos_tag(sentence);
    std::cout << tagged_sentence << std::endl;

    return 0;
}
