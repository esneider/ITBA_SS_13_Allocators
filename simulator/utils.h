#ifndef __UTILS_H__
#define __UTILS_H__


#define vrealloc(base, nel) trealloc((base), (nel) * sizeof(*(base)))

void error(void *root);


#endif /* __UTILS_H__ */