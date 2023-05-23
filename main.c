#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>


#define PORT 8080
#define NUM_THREADS 4

struct connection_info {
    struct MHD_Connection *connection;
    const char *url;
};

int handle_request(void *cls, struct MHD_Connection *connection, const char *url,
                   const char *method, const char *version, const char *upload_data,
                   size_t *upload_data_size, void **con_cls)
{
    // Handle the request logic here
    const char *response = "Hello, World!";
    struct MHD_Response *http_response;
    // Create a response with the "Hello, World!" message
    http_response = MHD_create_response_from_buffer(strlen(response), (void *)response, MHD_RESPMEM_PERSISTENT);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, http_response);
    MHD_destroy_response(http_response);

    return (ret);
}

void *worker_thread(void *arg)
{
    struct connection_info *info = (struct connection_info *)arg;
    handle_request(NULL, info->connection, info->url, NULL, NULL, NULL, NULL, NULL);
    free(info);
    pthread_exit(NULL);
}

int main()
{
    struct MHD_Daemon *daemon;
    pthread_t threads[NUM_THREADS];
    // Start the HTTP server
    daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, PORT, NULL, NULL,
                              &handle_request, NULL, MHD_OPTION_END);
    if (daemon == NULL) {
        printf("Failed to start the server\n");
        return 1;
    }

    printf("Server running on port %d\n", PORT);

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, worker_thread, NULL) != 0) {
            printf("Failed to create worker thread %d\n", i);
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    // Stop the HTTP server    
    MHD_stop_daemon(daemon);

    return (0);
}
