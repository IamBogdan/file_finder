#ifndef FILE_FINDER_
#define FILE_FINDER_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>

#define FF_MAX_PATH_SIZE 2048

enum
{
  FF_SUCCESSFULLY_FOUND = 0,
  FF_NOT_FOUND,
  FF_MEMORY_ALLOCATION_ERROR
};

struct ff_error_item
{
  char path[FF_MAX_PATH_SIZE];
  int error_code;
  struct ff_error_item* next;
};

#ifdef __cplusplus
extern "C" {
#endif

void ff_init_error_list(struct ff_error_item** list);
void ff_print_error_list(struct ff_error_item* list);
void ff_delete_error_list(struct ff_error_item* list);

/*
  Returns: on success - 0 and on error - 1
*/
int8_t ff_push_front_error_list(struct ff_error_item** list,
                                const char* path, int error_code);
/*
  Launches a search for a file by name
  inits error_list

  Parameters:
    const char* file_name - is the string the file name with the suffix
    const char* start_point - the directory from which you want to start the search
    int8_t thread_count - the number of threads that will search (if it is zero, only the main thread works)
    char** res_path - the string in which the path will be written, if the file is found
    struct ff_error_item** error_list - the list of errors that can occur when reading a directory
  
  Returns: FF_SUCCESSFULLY_FOUND (0) or FF_NOT_FOUND (1) or FF_MEMORY_ALLOCATION_ERROR (2)
*/
int8_t ff_start_search(const char* file_name, const char* start_point, int8_t thread_count,
                       char** res_path, struct ff_error_item** error_list);

#ifdef __cplusplus
}
#endif

#endif /* FILE_FINDER_ */