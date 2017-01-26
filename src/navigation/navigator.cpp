#include "navigator.hpp"

Navigator::Navigator(void)
{
  logging::vout("Creating Navigator object");
  Py_Initialize();
  logging::vout("Python initialized");
  /* we'll remove this action later and place main.py directly in the path */
  /* PATH : /usr/local/lib/python2.7/dist-packages */
  if(PyRun_SimpleString("import sys;sys.path.insert(0, './src/navigation/')")) {
    logging::verr("path expansion failed");
  }

  module = PyImport_ImportModule("main");
  if(module == NULL) {
    logging::verr("import failed");
    PyErr_Print();
  }
}

Navigator::~Navigator(void)
{
  logging::vout("Destroying Navigator object");
  end_python();
  Py_Finalize();
  logging::vout("Python context finalized");
}

void Navigator::call_python_function_void_nargs(std::string function)
{
  PyObject *fonction;
  fonction = PyObject_GetAttrString(module, function.c_str());
  if(fonction == NULL) {
    std::string error = "could not find function\n";
    logging::verr(error);
  }
  PyEval_CallObject(fonction,NULL);
  Py_DECREF(fonction);
}

std::string Navigator::call_python_function_nargs(std::string function)
{
  char *resultat;
  PyObject *fonction, *retour;
  fonction = PyObject_GetAttrString(module, function.c_str());
  if(fonction == NULL) {
    std::string error = "could not find function\n";
    logging::verr(error);
  }
  retour = PyEval_CallObject(fonction,NULL);
  Py_DECREF(fonction);
  if(retour == NULL) {
    printf("It all went wrong\n");
    PyErr_Print();
  }
  PyArg_Parse(retour, "s", &resultat);

  std::string cpp_str = resultat;
  Py_DECREF(retour);
  return cpp_str;
}


std::string Navigator::call_python_function(std::string function,std::string arg)
{
  char *resultat;
  PyObject *retour, *fonction, *arguments;

  fonction = PyObject_GetAttrString(module, function.c_str());
  if(fonction == NULL) {
    std::string error = "could not find function\n";
    logging::verr(error);
    return error;
  }

  arguments = Py_BuildValue("(s)", arg.c_str());
  if(arguments == NULL) {
    std::string error = "arg parsing failed\n";
    logging::verr(error);
    return error;
  }
  retour = PyEval_CallObject(fonction, arguments);
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

std::string Navigator::get_body_html()
{
  return this->call_python_function_nargs("get_body_html");
}

std::string Navigator::navigate(std::string url)
{
  return this->call_python_function("navigate",url);
}

void Navigator::end_python()
{
  this->call_python_function_void_nargs("end_python");
}

std::string Navigator::write_search(std::string keyword)
{
  return this->call_python_function("write_search",keyword);
}

void Navigator::select_hyperlinks_from_html(std::string html, std::vector<HyperLink> &links, std::vector<std::string> rubbish)
{
  // Just in case it is not empty yet.
  links.clear();
	while(html.find("<a ") != std::string::npos) {
    HyperLink lk;
    size_t b_tag_a = html.find("<a ");
    size_t e_tag_a = html.find("</a>");
    std::string tag_a = html.substr(b_tag_a, e_tag_a - b_tag_a);
    if((e_tag_a+4) <= html.size())
      html.erase(0,e_tag_a+4);
    else
      break;
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
      if((e_close - b_close +1) <= lk.text.size())
        lk.text = lk.text.erase(b_close,e_close - b_close +1);
      else
        break;
      if((e_open - b_open +1) <= lk.text.size())
        lk.text = lk.text.erase(b_open,e_open - b_open +1);
      else
        break;
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
    /* remove links like the ones in rubbish list */
    if(!rubbish.empty()){
      bool insert = true;
      for(auto const& rub: rubbish) {
        if(lk.url.find(rub) != std::string::npos){
          insert = false;
        }
      }
      if(!insert)
        continue;
    }
    /* remove urls with extensions other than .php or .html such as .pdf, .png and so on... */
    if(lk.url.length()>5){
      std::string ext = lk.url.substr(lk.url.length()-6);
      if((ext.find('.') != std::string::npos && (lk.url.substr(lk.url.length()-4) != ".php" || lk.url.substr(lk.url.length()-5) != ".html"))){
        continue;
      }
    }
    links.push_back(lk);
  }
}

void Navigator::select_hyperlinks_from_html(std::string html, std::vector<HyperLink> &links)
{
  // Just in case it is not empty yet.
  links.clear();
	while(html.find("<a ") != std::string::npos) {
    HyperLink lk;
    size_t b_tag_a = html.find("<a ");
    size_t e_tag_a = html.find("</a>");
    std::string tag_a = html.substr(b_tag_a, e_tag_a - b_tag_a);
    if((e_tag_a+4) <= html.size())
      html.erase(0,e_tag_a+4);
    else
      break;
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
      if((e_close - b_close +1) <= lk.text.size())
        lk.text = lk.text.erase(b_close,e_close - b_close +1);
      else
        break;
      if((e_open - b_open +1) <= lk.text.size())
        lk.text = lk.text.erase(b_open,e_open - b_open +1);
      else
        break;
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
    /* remove urls with extensions other than .php or .html such as .pdf, .png and so on... */
    if(lk.url.length()>5){
      std::string ext = lk.url.substr(lk.url.length()-6);
      if((ext.find('.') != std::string::npos && (lk.url.substr(lk.url.length()-4) != ".php" || lk.url.substr(lk.url.length()-5) != ".html"))){
        continue;
      }
    }
    links.push_back(lk);
  }
}
