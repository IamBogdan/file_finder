#include "file_finder.h"
#include <errno.h>
#include <stdio.h>

static pthread_mutex_t g_mutex_current_running_threads;
static pthread_mutex_t g_mutex_lookup;

static int8_t g_current_running_threads;
static int8_t g_search_status;
static int8_t g_max_threads;

static const char* g_target_file;
static char g_target_file_directory[FF_MAX_PATH_SIZE];

static struct ff_error_item* g_list;

static void* calc(void*);

static int8_t get_current_running_threads()
{
  pthread_mutex_lock(&g_mutex_current_running_threads);
  int8_t tmp = g_current_running_threads;
  pthread_mutex_unlock(&g_mutex_current_running_threads);
  return tmp;
}

static void set_found_file(const char* path)
{
  pthread_mutex_lock(&g_mutex_lookup);
  g_search_status = FF_SUCCESSFULLY_FOUND;
  strcpy(g_target_file_directory, path);
  pthread_mutex_unlock(&g_mutex_lookup);
}

static int8_t get_search_status()
{
  pthread_mutex_lock(&g_mutex_lookup);
  int8_t tmp = g_search_status;
  pthread_mutex_unlock(&g_mutex_lookup);
  return tmp;
}

static void read_directory(const char* path)
{
  DIR* p_directory;
  struct dirent *p_dirent;
  char next_path[FF_MAX_PATH_SIZE];

  /* opens dir */
  p_directory = opendir(path);
  if (p_directory == NULL) {
    pthread_mutex_lock(&g_mutex_lookup);
    if (ff_push_front_error_list(&g_list, path, errno)) {
      g_search_status = FF_MEMORY_ALLOCATION_ERROR;
    }
    pthread_mutex_unlock(&g_mutex_lookup);
    return;
  }

  /* reads dirs and files in dir */
  while ((p_dirent = readdir(p_directory)) != NULL && get_search_status() == FF_NOT_FOUND) {
    if (p_dirent->d_type == DT_DIR) { /* for dirs */
      if (strcmp(p_dirent->d_name, ".") == 0 || strcmp(p_dirent->d_name, "..") == 0)
        continue;

      snprintf(next_path, sizeof(next_path), "%s/%s", path, p_dirent->d_name);
      
      if (get_current_running_threads() < g_max_threads) {
          pthread_t thread;
          pthread_create(&thread, NULL, calc, next_path);
          pthread_join(thread, NULL);
      }
      else {
        read_directory(next_path);
      }
    }
    else if (p_dirent->d_type == DT_REG && strcmp(p_dirent->d_name, g_target_file) == 0) { /* for files */
      snprintf(next_path, sizeof(next_path), "%s/%s", path, p_dirent->d_name);
      set_found_file(next_path);

      goto cleaning;
    }
  }

cleaning:
  closedir(p_directory);
}


static void* calc(void* p_path)
{
  pthread_mutex_lock(&g_mutex_current_running_threads);
  g_current_running_threads++;
  pthread_mutex_unlock(&g_mutex_current_running_threads);

  if (p_path != NULL) {
    read_directory((const char*)p_path);
  }

  pthread_mutex_lock(&g_mutex_current_running_threads);
  g_current_running_threads--;
  pthread_mutex_unlock(&g_mutex_current_running_threads);

  pthread_exit(NULL);
}

int8_t ff_start_search(const char* file_name, const char* start_point, int8_t thread_count,
                       char** res_path, struct ff_error_item** error_list)
{
  g_max_threads = thread_count;
  g_current_running_threads = 0;
  g_search_status = FF_NOT_FOUND;
  g_target_file = file_name;
  ff_init_error_list(&g_list);

  read_directory(start_point);
  
  /* result: */

  *error_list = g_list;

  if (g_search_status == FF_SUCCESSFULLY_FOUND) {
    *res_path = g_target_file_directory;
    return FF_SUCCESSFULLY_FOUND;
  }

  return g_search_status;
}

/* list: */

void ff_init_error_list(struct ff_error_item** list)
{
  *list = NULL;
}

int8_t ff_push_front_error_list(struct ff_error_item** list,
                                const char* path, int error_code)
{
  struct ff_error_item* item;

  item = malloc(sizeof(struct ff_error_item));
  if (item == NULL) {
    return 1;
  }

  strcpy(item->path, path);
  item->error_code = error_code;
  item->next = *list;

  *list = item;

  return 0;
}

void ff_print_error_list(struct ff_error_item* list)
{
  for (; list; list = list->next) {
    printf("%s - %s\n", list->path, strerror(list->error_code));
  }
}

void ff_delete_error_list(struct ff_error_item* list)
{
  struct ff_error_item* current = list;
  struct ff_error_item* next;
  
  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
}