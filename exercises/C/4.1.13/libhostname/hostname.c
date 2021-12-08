/*
 * libhostname.so 
 *
 * Replaces the following functions defined in unistd.h
 *     int gethostname(char *name, size_t len);
 *     int sethostname(const char *name, size_t len);
 *
 * Uses environment variables to store and retrieve the name.
 *
 * When a student creates code demonstrating their ability to use the 
 * sethostname() function, the program will fail as students do not have
 * priivileged access on the system in which their code executes.
 *
 * Load the function using:
 *     export LD_PRELOAD=path/libhostname.so 
 *
 * and it will be given precidence over the system sethostname() function.
 *
 * Because sethostname() and gethostname() are companion functions, they 
 * both have to be replaced for sanity.
*/

#define _GNU_SOURCE
#include "hostname.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

#define HOST_NAME_MAX 63 // Hostname max length (man -s7 hostname)

int sethostname(const char *name, size_t len)
{
    int set_hostname_error = -1;

    if(HOST_NAME_MAX >= strlen(name))
    {
        if (setenv("HOST_NAME", name, 1)) // 1 will overwrite if already set
        {
            perror("Unable to temporarily change hostname");
        }
        else
        {
            set_hostname_error = 0;
        }
   }
   return set_hostname_error;
}

int gethostname(char *name, size_t len)
{
    int get_hostname_error = -1;
    char hostnamebuffer[HOST_NAME_MAX + 1] = {0};
    char *hostname;

    hostname = getenv("HOST_NAME");

    if(hostname != NULL)
    {
        strncpy(name, hostname, len);
        get_hostname_error = 0;
    }
    else
    {
        /*
         * Need to use the non-preloaded gethostname() to get system hostname
         * and set the environment variable to that
        */
        int(*original_gethostname)(char *name, size_t len);
        original_gethostname = dlsym(RTLD_NEXT, "gethostname");

        if (original_gethostname != NULL)
        {
            if (!(*original_gethostname)(hostnamebuffer, HOST_NAME_MAX))
            {
                puts(hostnamebuffer);

                if(!setenv("HOST_NAME", hostnamebuffer, 0))
                {
                    strncpy(name, hostnamebuffer, len);
                    get_hostname_error = 0;
                }
            }
        }
    }

    return get_hostname_error;
}
