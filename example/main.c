#include "file_finder.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
  struct ff_error_item* list;
  char* str;

  if (argc < 4) {
    return 1;
  }

  if (ff_start_search(argv[1], argv[2], atoi(argv[3]), &str, &list) == FF_SUCCESSFULLY_FOUND) {
    printf("errors:\n");
    ff_print_error_list(list);

    printf("---------\n");
    printf("%s\n", str);
  }

  ff_delete_error_list(list);

  return 0;
}