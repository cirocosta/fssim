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

int32_t fs_utils_gettime()
{
  struct timeval tv;
  PASSERT(!(gettimeofday(&tv, NULL)), "gettimeofday:");

  return (int32_t)tv.tv_sec;
}

int fs_utils_secs2str(int32_t secs, char* buf, int n)
{
  ASSERT(n >= FS_DATE_FORMAT_SIZE,
         "`buf` must have at least %d available bytes. Has %d.",
         FS_DATE_FORMAT_SIZE, n);

  const time_t tsecs = (time_t)secs;
  struct tm* tm = localtime(&tsecs);
  int written = 0;

  ASSERT(tm != NULL, "locatime: couldn't get localtime given secs = %d", secs);
  PASSERT((written = strftime(buf, n, FS_DATE_FORMAT, tm)) != 0, "strftime: ");

  return written;
}

int fs_utils_filesize2str(int32_t bytes, char* buf, int n)
{
  ASSERT(n >= FS_FILESIZE_FORMAT_SIZE,
         "`buf` must have at least %d available bytes. Has %d.",
         FS_FILESIZE_FORMAT_SIZE, n);

  const float t = 1024;
  uint8_t c = 0;
  float b = bytes;

  while (b >= t)
    b /= t, c++;

  ASSERT(c < 4, "Invalid byte size `%d`", bytes);

  return snprintf(buf, n, "%5.1f%2s", b, FS_FILESIZE_UNITS[c]);
}
