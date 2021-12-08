#include <stdlib.h>
#include <CUnit/CUnit.h>

#include "../include/set_hostname.h"

// Each element of the hostname must be from 1 to 63 characters long
#define HOST_NAME_MAX 63

void jqr_test_set_hostname(void)
{
    char input[HOST_NAME_MAX + 1];
    char hostname_buffer[HOST_NAME_MAX + 1] = {0};
    
    static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234567890-";
    ulong i;

    // Create random hostname of HOST_NAME_MAX chars

    for (i = 0; i < HOST_NAME_MAX; ++i)
    {
        input[i] = charset[rand() % (sizeof(charset) - 1)];
    }

    input[i] = 0;  // Terminate string

    set_hostname(input);
    
    gethostname(hostname_buffer, HOST_NAME_MAX);

    CU_ASSERT_TRUE(memcmp(hostname_buffer, input, sizeof input) == 0);
}
