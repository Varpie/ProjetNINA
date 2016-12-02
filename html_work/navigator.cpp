#include "navigator.h"

std::string get_bodyhtml_from_url(std::string url)
{
    char *resultat;
    PyObject *retour, *module, *fonction, *arguments;

    Py_Initialize();
    //PySys_SetPath(".");
    if(PyRun_SimpleString("import sys;sys.path.insert(0, './html_work/')")) {
    	std::string error;
        error = "path expansion failed\n";
        return error;
    }

    module = PyImport_ImportModule("main");
    if(module == NULL) {
    	std::string error;
        error = "import failed\n";
        PyErr_Print();
        return error;
    }

    fonction = PyObject_GetAttrString(module, "get_html_from_url");
    if(fonction == NULL) {
    	std::string error;
        error = "could not find function\n";
        return error;
    }

    arguments = Py_BuildValue("(s)", url.c_str());
    if(arguments == NULL) {
    	std::string error;
        error = "arg parsing failed\n";
        return error;
    }

    printf("Calling\n");
    retour = PyEval_CallObject(fonction, arguments);
    printf("Returned\n");

    // note: need to release arguments
    Py_DECREF(arguments);

    if(retour == NULL) {
        printf("It all went wrong\n");
        PyErr_Print();
        return url;
    }

    PyArg_Parse(retour, "s", &resultat);

    Py_Finalize();
    std::string cpp_str = resultat;
    return cpp_str;
}

void select_hyperlinks_from_html(std::string html,std::vector<HyperLink> &links)
{
	while(html.find("<a ") != std::string::npos) {
        size_t b_tag_a = html.find("<a ");
        HyperLink lk;
        size_t e_tag_a = html.find("</a>");
        std::string tag_a = html.substr(b_tag_a, e_tag_a - b_tag_a);
        html.erase(0,e_tag_a+4);

        size_t b_href = tag_a.find("href=\"");
        size_t e_href = tag_a.substr(b_href+6).find("\"");
        size_t b_txt_a = tag_a.find(">");
        lk.url = tag_a.substr(b_href+6,e_href);
        if(lk.url.find("//") == 0) {
            lk.url = "htpp:"+lk.url;
        }


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