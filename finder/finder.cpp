#include "finder.hpp"

using namespace finder;

std::string g_searched_file_name;
bool g_is_found;
std::string g_searched_file_path;

std::mutex g_sf_mutex;
std::mutex g_fp_mutex;

void setFound(bool new_value)
{
  std::lock_guard<std::mutex> locker(g_sf_mutex);
  g_is_found = new_value;
}

bool isFound()
{
  std::lock_guard<std::mutex> locker(g_sf_mutex);
  return g_is_found;
}

void setSearchedFilePath(std::string new_value) 
{
  std::lock_guard<std::mutex> locker(g_fp_mutex);
  g_searched_file_path = new_value; 
}

std::string getSearchedFilePath()
{
  std::lock_guard<std::mutex> locker(g_fp_mutex);
  return g_searched_file_path;
}

void setSearchedFileName(std::string new_value)
{
  g_searched_file_name = new_value;
}

std::string getSearchedFileName()
{
  return g_searched_file_name;
}

bool isRunAsync(std::atomic<size_t>& working, size_t thread_limit)
{
  size_t i = working;
  if(i < thread_limit){
    return working.compare_exchange_strong(i, i + 1);
  }
  return false;
}

bool scanDirectory(std::atomic<size_t>& working, const std::string& path, size_t thread_limit)
{
  if(isFound())
    return true;

  std::list<std::future<bool>> delayed;

  try{
    for(auto const& dir_entry : std::filesystem::directory_iterator(path/*, std::filesystem::directory_options::skip_permission_denied*/)){
      try{
        if(dir_entry.is_directory() && !dir_entry.is_symlink()){ // for dirs
          std::string next(dir_entry.path());
          if(isRunAsync(working, thread_limit)){
            auto thread_func = [&working, next, thread_limit](){
              bool status = scanDirectory(working, next, thread_limit);
              --working;
              return status;
            };
            delayed.emplace_back(std::async(std::launch::async, thread_func));
          }
          else{
            delayed.emplace_back(std::async(std::launch::deferred, scanDirectory, std::ref(working), next, thread_limit));
          }
        }
        else if(dir_entry.is_regular_file() && dir_entry.path().filename() == getSearchedFileName()){ // for files
          //std::cout << "HERE: " << dir_entry.path() << '\n';
          setSearchedFilePath(dir_entry.path());
          setFound(true);
          return true;
        }
      }
      catch(std::filesystem::filesystem_error& err){
        /* pass */
        continue;
      }
    }
  } 
  catch(std::filesystem::filesystem_error& e){/* pass */}

  bool is_found = false;
  for(auto &d: delayed){
    d.wait();
    is_found = d.get();
    if(isFound() || is_found)
      return true;
  }
  return false;
}

bool finder::searchFileByName(std::string start_directory, std::string file_name, size_t max_threads, std::string& result)
{
  std::atomic<size_t> working{0};

  setFound(false);
  setSearchedFileName(file_name);

  if(scanDirectory(working, start_directory, max_threads))
    result = getSearchedFilePath();

  return isFound();
}

bool finder::printPathFile(std::string file_name)
{
  std::string mypath;
  
  if(searchFileByName("/", file_name, 8, mypath)){
    std::cout << mypath << '\n';
  }
  else{
    std::cout << "File not found: " << file_name << '\n';
  }

  return isFound();
}