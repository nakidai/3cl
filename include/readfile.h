#ifndef __READFILE_H__
#define __READFILE_H__

/**
 * Read file by filename, die on error
 * @param name Path to file
 * @return Contents of file with additional '\0' at the end
 */
char *readfile(const char *name);

#endif /* __READFILE_H__ */
