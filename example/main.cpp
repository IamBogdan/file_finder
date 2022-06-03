#include "../finder/finder.hpp"

int main(int argc, const char* argv[])
{
  if(argc > 1){
    std::cout << "printPathFile:\n";
    finder::printPathFile(argv[1]);

    std::cout << "\nsearchFileByName:\n";
    std::string res;
    if(finder::searchFileByName("/", argv[1], 8, res))
      std::cout << res << '\n';
    else
      std::cout << "File not found\n";
  }
  else{
    std::cout << "Enter the name of the file you want to find.\n";
  }

  return 0;
}