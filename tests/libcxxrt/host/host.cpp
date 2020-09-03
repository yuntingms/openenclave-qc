// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

/*
  Since most of the libcxxrt tests are tested based on the
  comparison between log file generated by the enclave with
  log generated by system, one can't add any debug prints
  to this file. If added, the tests will fail because of
  additional prints in enclave log, which won't be there in
  system generated log. For more details please refer
  README.md file
*/

#include <openenclave/host.h>
#include <openenclave/internal/calls.h>
#include <openenclave/internal/error.h>
#include <openenclave/internal/tests.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "libcxxrt_u.h"

void Test(oe_enclave_t* enclave)
{
    int ret = 1;
    char* name = NULL;
    oe_result_t result = test(enclave, &ret, &name);
    OE_TEST(result == OE_OK);

    if (ret != 0)
    {
        printf("FAILED: %s (ret=%d)\n", name, ret);
        abort();
    }
}

void ocall_exit(uint64_t arg)
{
    exit(static_cast<int>(arg));
}

static int _get_opt(
    int& argc,
    const char* argv[],
    const char* name,
    const char** arg = NULL)
{
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], name) == 0)
        {
            if (!arg)
            {
                memmove(
                    (void*)&argv[i],
                    &argv[i + 1],
                    static_cast<size_t>(argc - i) * sizeof(char*));
                argc--;
                return 1;
            }

            if (i + 1 == argc)
                return -1;

            *arg = argv[i + 1];
            memmove(
                (char**)&argv[i],
                &argv[i + 2],
                static_cast<size_t>(argc - i - 1) * sizeof(char*));
            argc -= 2;
            return 1;
        }
    }

    return 0;
}

int main(int argc, const char* argv[])
{
    oe_result_t result;
    oe_enclave_t* enclave = NULL;
    uint32_t flags = OE_ENCLAVE_FLAG_DEBUG;

    // Check for the --sim option:
    if (_get_opt(argc, argv, "--simulate") == 1)
        flags |= OE_ENCLAVE_FLAG_SIMULATE;
    else
        flags = oe_get_create_flags();

    // Check argument count:
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s ENCLAVE\n", argv[0]);
        exit(1);
    }

    // Create the enclave:
    result = oe_create_libcxxrt_enclave(
        argv[1], OE_ENCLAVE_TYPE_SGX, flags, NULL, 0, &enclave);
    if (result != OE_OK)
    {
        oe_put_err("oe_create_enclave(): result=%u", result);
    }

    // Invoke "Test()" in the enclave.
    Test(enclave);

    // Shutdown the enclave.
    if ((result = oe_terminate_enclave(enclave)) != OE_OK)
        oe_put_err("oe_terminate_enclave(): result=%u", result);

    return 0;
}
