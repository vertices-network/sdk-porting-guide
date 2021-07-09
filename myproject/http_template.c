#include "vertices_http.h"
#include "string.h"
#include <stdio.h>
#include "vertices_errors.h"

static size_t
(*m_response_payload_cb)(char *chunk,
                         size_t chunk_size);
static void * m_http_client_handle = NULL;

static void
parse_headers(const char *headers, size_t len)
{
    char header_copy[len + 1];
    memcpy(header_copy, headers, len);
    header_copy[len] = 0;

    int key_idx = 0;
    int value_idx = 0;
    for (int i = 0; header_copy[i] != 0;)
    {
        key_idx = i;

        while (header_copy[i] != ':')
        {
            ++i;
        }
        header_copy[i] = 0;
        while (header_copy[i] == ' ')
        {
            ++i;
        };
        value_idx = i;
        while (header_copy[i] != '\n' && header_copy[i] != '\r' && header_copy[i] != 0)
        {
            ++i;
        }

        while (header_copy[i] == '\n' || header_copy[i] == '\r')
        {
            header_copy[i] = 0;
            ++i;
        }

        printf("Header: key:%s, value:%s", &header_copy[key_idx], &header_copy[value_idx]);
    }
}

ret_code_t
http_init(const provider_info_t *provider,
          size_t (*response_payload_cb)(char *chunk,
                                        size_t chunk_size))
{
    // init HTTP client using base URL, port, headers and the optional certificate
    // and keep a copy of the client handle locally

    // TODO your implementation
    // m_http_client_handle = http_client_init();
    m_http_client_handle = (void *) 0x1; // TODO to remove

    // keep a copy of the response callback: any reponse will be passed to that callback
    // so that the Vertices SDK can parse it
    VTC_ASSERT_BOOL(response_payload_cb != NULL);

    m_response_payload_cb = response_payload_cb;

    return VTC_SUCCESS;
}

ret_code_t
http_get(const provider_info_t *provider,
         const char *relative_path,
         const char *headers,
         uint32_t *response_code)
{
    VTC_ASSERT_BOOL(m_http_client_handle != NULL);

    // the full URL is a concatenation of
    // provider->url + relative_path
    printf("GET %s%s\n", provider->url, relative_path);

    // headers are separated using \n, \r or both
    // "key1:value1\n\rkey2:value2\n\r"
    // a function is provided above to parse the keys and associated values
    parse_headers(headers, strlen(headers));

    // GET
    // TODO your implementation + remove fake implementation below

    // Below implementation is faking data
    // based on testnet
    char * response_data = NULL;
    size_t response_length = 0;
    if (strcmp("/health", relative_path) == 0)
    {
        const char * health_response = "null";
        response_data = (char *)health_response;
        response_length = strlen(health_response);

        // set response code
        *response_code = 200;
    }
    else if (strcmp("/versions", relative_path) == 0)
    {
        const char * versions_response = "{\"versions\":[\"v1\",\"v2\"],\"genesis_id\":\"testnet-v1.0\",\"genesis_hash_b64\":\"SGO1GKSzyE7IEPItTxCByw9x8FmnrCDexi9/cOUJOiI=\",\"build\":{\"major\":2,\"minor\":7,\"build_number\":1,\"commit_hash\":\"5e00bcd6\",\"branch\":\"rel/stable\",\"channel\":\"stable\"}}";
        
        response_data = (char *) versions_response;
        response_length = strlen(versions_response);

        // set response code
        *response_code = 200;
    }
    else
    {
        // Not found
        *response_code = 404; 
    }

    if (response_data != NULL && response_length != 0)
    {
        printf("/!\\ Responding fake data\n");
        m_response_payload_cb((char *)response_data, response_length);
    }

    if (*response_code >= 300)
    {
        return VTC_HTTP_ERROR;
    }

   return VTC_SUCCESS;
}

ret_code_t
http_post(const provider_info_t *provider,
          const char *relative_path,
          char *headers,
          const char *body,
          size_t body_size,
          uint32_t *response_code)
{
    VTC_ASSERT_BOOL(m_http_client_handle != NULL);

    // the full URL is a concatenation of
    // provider->url + relative_path
    printf("POST %s%s\n", provider->url, relative_path);

    // POST headers and body
    // TODO your implementation

    // set response code 
    // TODO to remove
    *response_code = 200;

    if (*response_code >= 300)
    {
        return VTC_HTTP_ERROR;
    }

    return VTC_SUCCESS;
}

void
http_close(void)
{
    // uninit/close HTTP client
    // TODO your implementation

    // reset local variables
    m_response_payload_cb = NULL;
    m_http_client_handle = NULL;
}
