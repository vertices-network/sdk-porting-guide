#include <stdio.h>
#include "vertices.h"
#include "string.h"

static provider_info_t algoexplorer_provider =
    {.url = (char *) "https://api.testnet.algoexplorer.io", 
    .port = 0, 
    .header = (char *) ""};

static ret_code_t
vertices_evt_handler(vtc_evt_t *evt)
{
    ret_code_t err_code = VTC_SUCCESS;

    switch (evt->type)
    {
        case VTC_EVT_TX_READY_TO_SIGN:
        {
            // get pending transaction to be signed
            signed_transaction_t *tx = NULL;
            err_code = vertices_transaction_get(evt->bufid, &tx);
            if (err_code == VTC_SUCCESS)
            {
                // create a byte array that will be signed
                unsigned char to_be_signed[tx->payload_body_length + 2];

                // init with value "TX"
                to_be_signed[0] = 'T';
                to_be_signed[1] = 'X';

                // copy the payload body into that array
                memcpy(&to_be_signed[2], &tx->payload[tx->payload_header_length], tx->payload_body_length);

                // `to_be_signed` is now ready to be signed

                // TODO sign `to_be_signed` using account private key and put the signature into tx->signature
                // ...

                // send event to send the signed TX
                vtc_evt_t sched_evt = { .type = VTC_EVT_TX_SENDING, .bufid = evt->bufid };
                err_code = vertices_event_schedule(&sched_evt);
            }
        }
            break;
        
        case VTC_EVT_TX_SENDING:
        {
            // nothing to be done
        }
            break;

        case VTC_EVT_TX_SUCCESS:
        {
            printf("Hoora!");
        }
            break;

        default:break;
    }

    return err_code;
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

    // pass a pointer to a local variable if you want to know the events 
    // waiting to be processed
    size_t queue_size;
    while(1)
    {
        // process while there are events waiting
        do
        {
            err_code = vertices_event_process(&queue_size);
            VTC_ASSERT(err_code);
        } while(queue_size);

        // go into sleep?
    }

    return 0;
}
