#include "fssim/file_utils.h"

int fs_utils_fsize(FILE* file)
{
  int size;
  int last_pos;

  PASSERT(~fseek(file, 0, SEEK_SET), "");
  last_pos = ftell(file);
  PASSERT(~fseek(file, 0, SEEK_END), "");
  size = ftell(file);
  PASSERT(~fseek(file, last_pos, SEEK_SET), "");

  return size;
}

char** fs_utils_splitpath(const char* input, unsigned* size)
{
  const char delimiter = '/';
  const size_t input_size = strlen(input);
  unsigned count = 0;
  unsigned i = 0;

  char** result = NULL;
  char* tmp = strndup(input, input_size);
  char* tmp_init = tmp;

  char token_delimiter[2] = { delimiter, '\0' };

  while (i < input_size) {
    while (tmp[i++] == delimiter && i < input_size)
      ;
    while (tmp[i++] != delimiter && i < input_size)
      ;
    count++;
  }

  result = malloc((count + 1) * sizeof(*result));
  PASSERT(result, "%s", FS_ERR_MALLOC);
  count = 0;

  tmp = strtok(tmp, token_delimiter);
  while (tmp != NULL) {
    *(result + count) = strdup(tmp);
    PASSERT(result + count, "%s", FS_ERR_MALLOC);
    tmp = strtok(NULL, token_delimiter);
    count++;
  }

  result[count] = NULL;

  if (size)
    *size = count;

  free(tmp_init);
  return result;
}
