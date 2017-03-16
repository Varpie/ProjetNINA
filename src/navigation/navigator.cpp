#include "navigator.hpp"

Navigator::Navigator(void)
{
  logging::vout(1,"Creating Navigator object");
  Py_Initialize();
  logging::vout(1,"Python initialized");
  /* not necessary since we moved nina.py to /usr/local/lib/python2.7/dist-packages/ /
  /*if(PyRun_SimpleString("import sys;sys.path.insert(0, './src/navigation/')")) {
    logging::verr("path expansion failed");
  }*/
  module = PyImport_ImportModule("nina");
  if(module == NULL) {
    logging::verr("import failed");
    PyErr_Print();
  }
  this->define_verbose(logging::verbose);
}

Navigator::~Navigator(void)
{
  logging::vout(1,"Destroying Navigator object");
  end_python();
  Py_Finalize();
  logging::vout(1,"Python context finalized");
}

void Navigator::call_python_function_void_nargs(std::string function)
{
  logging::vout(2,"Entering Navigator::call_python_function_void_nargs : "+function);
  PyObject *fonction;
  fonction = PyObject_GetAttrString(module, function.c_str());
  if(fonction == NULL) {
    std::string error = "could not find function\n";
    logging::verr(error);
  }
  PyEval_CallObject(fonction,NULL);
  Py_DECREF(fonction);
  logging::vout(2,"Leaving Navigator::call_python_function_void_nargs : "+function);
}

std::string Navigator::call_python_function_nargs(std::string function)
{
  logging::vout(2,"Entering Navigator::call_python_function_nargs : "+function);
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
    logging::verr(1,"It all went wrong\n");
    PyErr_Print();
  }
  PyArg_Parse(retour, "s", &resultat);

  std::string cpp_str = resultat;
  Py_DECREF(retour);
  logging::vout(2,"Leaving Navigator::call_python_function_nargs : "+function);
  return cpp_str;
}

int Navigator::call_python_function_nargs_i(std::string function)
{
  logging::vout(2,"Entering Navigator::call_python_function_nargs : "+function);
  int result;
  PyObject *fonction, *retour;
  fonction = PyObject_GetAttrString(module, function.c_str());
  if(fonction == NULL) {
    std::string error = "could not find function\n";
    logging::verr(error);
  }
  retour = PyEval_CallObject(fonction,NULL);
  Py_DECREF(fonction);
  if(retour == NULL) {
    logging::verr(1,"It all went wrong\n");
    PyErr_Print();
  }
  PyArg_Parse(retour, "i", &result);

  Py_DECREF(retour);
  logging::vout(2,"Leaving Navigator::call_python_function_nargs : "+function);
  return result;
}


std::string Navigator::call_python_function(std::string function,std::string arg)
{
  logging::vout(2,"Entering Navigator::call_python_function : "+function);
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
  /* note: need to release arguments */
  Py_DECREF(arguments);
  Py_DECREF(fonction);


  if(retour == NULL) {
    logging::verr(1,"It all went wrong\n");
    PyErr_Print();
    return arg;
  }
  PyArg_Parse(retour, "s", &resultat);

  std::string cpp_str = resultat;
  Py_DECREF(retour);
  logging::vout(2,"Leaving Navigator::call_python_function : "+function);
  return cpp_str;
}

void Navigator::call_python_function_void_args(std::string function, std::string arg)
{
  logging::vout(2,"Entering Navigator::call_python_function_args : "+function);
  PyObject *fonction, *arguments;

  fonction = PyObject_GetAttrString(module, function.c_str());
  if(fonction == NULL) {
    std::string error = "could not find function\n";
    logging::verr(error);
  }

  arguments = Py_BuildValue("(s)", arg.c_str());
  if(arguments == NULL) {
    std::string error = "arg parsing failed\n";
    logging::verr(error);
  }
  PyEval_CallObject(fonction, arguments);
  Py_DECREF(arguments);
  Py_DECREF(fonction);
}

void Navigator::call_python_function_void_args(std::string function, int arg)
{
  logging::vout(2,"Entering Navigator::call_python_function_args : "+function);
  PyObject *fonction, *arguments;

  fonction = PyObject_GetAttrString(module, function.c_str());
  if(fonction == NULL) {
    std::string error = "could not find function\n";
    logging::verr(error);
  }

  arguments = Py_BuildValue("(i)", arg);
  if(arguments == NULL) {
    std::string error = "arg parsing failed\n";
    logging::verr(error);
  }
  PyEval_CallObject(fonction, arguments);
  Py_DECREF(arguments);
  Py_DECREF(fonction);
}

void Navigator::define_verbose(int level)
{
  this->call_python_function_void_args("define_verbose",level);
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

int Navigator::get_pid()
{
  return this->call_python_function_nargs_i("get_pid");
}

void Navigator::select_hyperlinks_from_html(std::string html, std::vector<HyperLink> &links)
{
  logging::vout(2,"Entering Navigator::select_hyperlinks_from_html no rubbish");
  /* Just in case it is not empty yet. */
  links.clear();
	while(html.find("<a ") != std::string::npos) {
    HyperLink lk;
    size_t b_tag_a = html.find("<a ");
    size_t e_tag_a = html.find("</a>");
    if(b_tag_a == std::string::npos || e_tag_a == std::string::npos ||
      ((html.size()-b_tag_a) < (e_tag_a - b_tag_a)) || (html.size() < (e_tag_a+4))) {
      break;
    }
    std::string tag_a = html.substr(b_tag_a, e_tag_a - b_tag_a);
    html.erase(0,e_tag_a+4);
    bool a = parse_tag_a(lk,tag_a);
    if(!a)
      continue;
    links.push_back(lk);
  }
  logging::vout(2,"Leaving Navigator::select_hyperlinks_from_html no rubbish");
}

void Navigator::select_hyperlinks_from_html(std::string html, std::vector<HyperLink> &links, std::vector<std::string> rubbish)
{
  logging::vout(2,"Entering Navigator::select_hyperlinks_from_html rubbish");
  /* Just in case it is not empty yet. */
  links.clear();
	while(html.find("<a ") != std::string::npos) {
    HyperLink lk;
    bool pass = false;
    size_t b_tag_a = html.find("<a ");
    size_t e_tag_a = html.find("</a>");
    if(b_tag_a == std::string::npos || e_tag_a == std::string::npos ||
      ((html.size()-b_tag_a) < (e_tag_a - b_tag_a)) || (html.size() < (e_tag_a+4))) {
      break;
    }
    std::string tag_a = html.substr(b_tag_a, e_tag_a - b_tag_a);
    html.erase(0,e_tag_a+4);
    bool a = parse_tag_a(lk,tag_a);
    if(!a)
      continue;
    for(auto const& rub: rubbish) {
      if(lk.url.find(rub) != std::string::npos || lk.url == "#" || lk.url == "="){
        pass = true;
      }
    }
    if(pass){
      continue;
    }
    links.push_back(lk);
  }
  logging::vout(2,"Leaving Navigator::select_hyperlinks_from_html rubbish");
}

bool Navigator::parse_tag_a(HyperLink &lk,std::string &tag_a) 
{
  logging::vout(4,"Entering Navigator::parse_tag_a");
  try {
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
    /* remove urls with extensions other than .php or .html such as .pdf, .png and so on... */
    if(lk.url.length()>5){
      std::string ext = lk.url.substr(lk.url.length()-6);
      if((ext.find('.') != std::string::npos && (lk.url.substr(lk.url.length()-4) != ".php" || lk.url.substr(lk.url.length()-5) != ".html"))){
        return false;
      }
    }
    return true;
  } catch (const std::out_of_range &e) {
    logging::vout(1,"Error : " + (std::string)e.what());
    return false;
  }
  logging::vout(4,"Leaving Navigator::parse_tag_a");
}
