#define _GNU_SOURCE

#include "libmalloc.h"

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

static void *(*real_malloc)(size_t) = NULL;
static void *(*real_calloc)(size_t, size_t) = NULL;
static void *(*real_realloc)(void *, size_t) = NULL;
static void (*real_free)(void *) = NULL;

static void *(*tmp_malloc)(size_t) = NULL;
static void *(*tmp_calloc)(size_t, size_t) = NULL;
static void *(*tmp_realloc)(void *, size_t) = NULL;
static void (*tmp_free)(void *) = NULL;

int TEST_FAIL;

// https://github.com/jtolio/malloc_instrumentation/blob/master/malloc_instrument.c#L69
char          tmpbuf[1024];
unsigned long tmppos = 0;
unsigned long tmpallocs = 0;

void *
dummy_malloc(size_t size)
{
    if (tmppos + size >= sizeof(tmpbuf))
    {
        exit(1);
    }
    void *retptr = tmpbuf + tmppos;
    tmppos += size;
    ++tmpallocs;
    return retptr;
}

void *
dummy_calloc(size_t nmemb, size_t size)
{
    void *       ptr = dummy_malloc(nmemb * size);
    unsigned int i = 0;
    for (; i < nmemb * size; ++i)
    {
        *((char *)(ptr + i)) = '\0';
    }
    return ptr;
}

void
dummy_free(void *ptr)
{
}

void __attribute__((constructor)) hookfns(void)
{
    real_malloc = dummy_malloc;
    real_calloc = dummy_calloc;
    real_free = dummy_free;

    tmp_malloc = dlsym(RTLD_NEXT, "malloc");
    tmp_calloc = dlsym(RTLD_NEXT, "calloc");
    tmp_free = dlsym(RTLD_NEXT, "free");
    tmp_realloc = dlsym(RTLD_NEXT, "realloc");

    real_malloc = tmp_malloc;
    real_calloc = tmp_calloc;
    real_realloc = tmp_realloc;
    real_free = tmp_free;
}

void *
calloc(size_t nmemb, size_t size)
{
    if (TEST_FAIL)
    {
        return NULL;
    }
    return real_calloc(nmemb, size);
}

void *
malloc(size_t size)
{
    if (TEST_FAIL)
    {
        return NULL;
    }
    return real_malloc(size);
}

void
free(void *ptr)
{
    // This is super hacky
    // but we do this check so that valgrind doesn't complain
    // otherwise we would get an error.
    // dlfcn registers an exit handler that will free the error buf it
    // creates when you call dlsym()
    // Since we've temporarily put that on the stack we will end up calling
    // free on a stack address.
    if (ptr == tmpbuf)
    {
        return;
    }
    real_free(ptr);
}
