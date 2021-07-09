#include <stdio.h>
#include "vertices.h"

static provider_info_t algoexplorer_provider =
    {.url = (char *) "https://api.testnet.algoexplorer.io", 
    .port = 0, 
    .header = (char *) ""};

static ret_code_t
vertices_evt_handler(vtc_evt_t *evt)
{
    /* to be implemented in the next chapters */
    
    return VTC_ERROR_INTERNAL;
}

int main(int argc, char *argv[])
{
    printf("Hello world\n");

    // vertex configuration
    vertex_t vertex = {
        .provider = &algoexplorer_provider,
        .vertices_evt_handler = vertices_evt_handler
    };

    // create new vertex
    ret_code_t err_code = vertices_new(&vertex);
    VTC_ASSERT(err_code);

    // ping provider
    err_code = vertices_ping();
    VTC_ASSERT(err_code);

    // get provider version
    provider_version_t version = {0};
    err_code = vertices_version(&version);
    VTC_ASSERT(err_code);

    printf("🏎 Running on %s v.%u.%u.%u\n",
             version.network,
             version.major,
             version.minor,
             version.patch);

    return 0;
}
