#ifndef FINDER_H
#define FINDER_H

#include <iostream>
#include <filesystem>
#include <atomic>
#include <future>
#include <list>

namespace finder
{
  /*
    Launches a search for a file by name

    Parameters:
      std::string start_directory - the directory from which you want to start the search
      std::string file_name - is a string the file name with the suffix
      size_t max_threads - the number of threads that will search
      std::string& result - string in which the path will be written, if the file is found
    
    Returns "true" if the search is successful, otherwise returns "false"
  */
  bool searchFileByName(std::string start_directory, std::string file_name, size_t max_threads, std::string& result);
  /*
    Launches a search for a file by name with standard parameters (initial directory "/", 8 threads to search)
    Outputs the file path if the search was successful, otherwise outputs "File not found: *file_name*"

    Parameters:
      std::string file_name - is a string the file name with the suffix

    Returns "true" if the search is successful, otherwise returns "false".
  */
  bool printPathFile(std::string file_name);
};

#endif