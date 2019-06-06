#ifndef kputils_h__
#define kputils_h__

int strcicmp(char const *a, char const *b);
void hello(void);
size_t strlcpy(char* dst, const char* src, size_t bufsize);
char *strsep(char **stringp, const char *delim);
char *strdup (const char *s);

#endif  // kputils_h_
