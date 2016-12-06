#include "navigator.hpp"

Navigator::Navigator(void) {
    Py_Initialize();
    if(PyRun_SimpleString("import sys;sys.path.insert(0, './html_work/')")) {
        std::cout << "path expansion failed" << std::endl;
    }

    module = PyImport_ImportModule("main");
    if(module == NULL) {
        std::cout << "import failed" << std::endl;
        PyErr_Print();
    }
}

Navigator::~Navigator(void) {
    Py_Finalize();
}

std::string Navigator::call_python_function(std::string function,std::string arg) {
    char *resultat;
    PyObject *retour, *fonction, *arguments;

    fonction = PyObject_GetAttrString(module, function.c_str());
    if(fonction == NULL) {
        std::string error;
        error = "could not find function\n";
        return error;
    }

    arguments = Py_BuildValue("(s)", arg.c_str());
    if(arguments == NULL) {
        std::string error;
        error = "arg parsing failed\n";
        return error;
    }

    //std::cout << "Calling" << std::endl;
    retour = PyEval_CallObject(fonction, arguments);
    //std::cout << "Returned" << std::endl;

    // note: need to release arguments
    Py_DECREF(arguments);
    Py_DECREF(fonction);


    if(retour == NULL) {
        printf("It all went wrong\n");
        PyErr_Print();
        return arg;
    }
    PyArg_Parse(retour, "s", &resultat);

    std::string cpp_str = resultat;
    Py_DECREF(retour);
    return cpp_str;
}



std::string Navigator::get_body_html(std::string url)
{
    return this->call_python_function("get_body_html",url);
}

std::string Navigator::navigate(std::string url)
{
    return this->call_python_function("navigate",url);
}

void Navigator::select_hyperlinks_from_html(std::string html,std::vector<HyperLink> &links)
{
    // Just in case it is not empty yet.
    links.clear();

	while(html.find("<a ") != std::string::npos) {
        int c = 0;
        HyperLink lk;

        size_t b_tag_a = html.find("<a ");
        size_t e_tag_a = html.find("</a>");

        std::string tag_a = html.substr(b_tag_a, e_tag_a - b_tag_a);
        html.erase(0,e_tag_a+4);

        size_t b_href = tag_a.find("href=\"");
        size_t e_href = tag_a.substr(b_href+6).find("\"");
        size_t b_txt_a = tag_a.find(">");

        lk.url = tag_a.substr(b_href+6,e_href);
        lk.text = tag_a.substr(b_txt_a+1);
        while(lk.text.find("</") != std::string::npos) {
            size_t b_close = lk.text.find("</");
            size_t e_close = lk.text.find(">",b_close);
            std::string type_tag = lk.text.substr(b_close+2,e_close - 2 - b_close);
            size_t b_open = lk.text.find("<"+type_tag);
            size_t e_open = lk.text.find(">",b_open);
            lk.text = lk.text.erase(b_close,e_close - b_close +1);
            lk.text = lk.text.erase(b_open,e_open - b_open +1);
            std::string::size_type i = 0;
            /* remove \n */
            while(i < lk.text.length()) {
                i = lk.text.find('\n',i);
                if(i == std::string::npos) {
                    break;
                }
                lk.text.erase(i);
            }
        }
        links.push_back(lk);
    }
}
