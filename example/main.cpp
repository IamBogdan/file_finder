#include "finder.hpp"

int main(int argc, const char* argv[])
{

  if(argc == 1){
    std::cout << "Enter the name of the file you want to find.\n";
    return 1;
  }
  else if(argc == 2){
    std::cout << "Enter the number of threads.\n";
    return 1;
  }
  
  std::string file_name = argv[1];
  int thread_count = atoi(argv[2]);
  std::string result;

  if(finder::searchFileByName("/", file_name, thread_count, result))
    std::cout << result << '\n';
  else
    std::cout << "File not found\n";

  return 0;
}
