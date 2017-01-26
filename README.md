# ProjetNINA

## Building the project

Using autotools (install `autoconf` if you don't have it), run the following commands:

  ```
  aclocal
  autoconf
  automake
  ./configure
  make
  ```

## Coding style

- Only C-style comments

  ```
  /* This is okay */
  // This is not okay
  ```

- Opening curly brackets don't go under if statements, they go after it.

  Do this:

  ```
  if (true) {   
  ```

  Don't do that:

  ```
  if (true)
  {
  ```

- Functions work the opposite way (curly brackets under the function name)

## Documentation (Doxygen)

Python :
  ```
  def my_method(x,y):
  """
    my_method description

    @type x: int
    @param x: An integer

    @type y: int|string
    @param y: An integer or string

    @rtype: string
    @return: Returns a sentence with your variables in it
  """
  ```

C++:

  ```
  /** my_function description
   * \fn returntype my_function(argtype arg)
   * \param arg argtype utility
   * \return returntype what is returned
   * \brief brief description -> this is optional
   *
   * More description, and even even
   * even even even even
   * even More -> this is optional
   */
  returntype my_function(argtype arg);


  /**
   * description
   * \class classname header.h(pp)
   */
  class Classname{};
  ```

## Python installation

- Install python:
    ```
    sudo apt-get install python-dev
    ```

- Install pip:
    ```
    sudo apt-get install python-pip
    ```

- Homemade module installation:
  ProjetNINA/src/uintput/ python setup.py install
  PATH : /usr/local/lib/python2.7/dist-packages

  works for import main.py in navigator.cpp too
