#include <microhttpd.h>
#include <stdio.h>
#include <string.h>

#define PORT 8080

static int handle_request(
	void *cls, struct MHD_Connection *connection,
  const char *url, const char *method,
  const char *version, const char *upload_data,
  size_t *upload_data_size, void **con_cls)
{
	const char *response = "Hello, World!";
	struct MHD_Response *http_response;
	int ret;

	// Create a response with the "Hello, World!" message
	http_response = MHD_create_response_from_buffer(strlen(response), (void *)response, MHD_RESPMEM_PERSISTENT);
	ret = MHD_queue_response(connection, MHD_HTTP_OK, http_response);
	MHD_destroy_response(http_response);

	return (ret);
}

int main()
{
	struct MHD_Daemon *daemon;

	// Start the HTTP server
	daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL, &handle_request, NULL, MHD_OPTION_END);
	if (daemon == NULL)
	{
			printf("Failed to start the server\n");
			return 1;
	}

	printf("Server running on port %d\n", PORT);

	// Wait for the user to terminate the program
	getchar();

	// Stop the HTTP server 
	MHD_stop_daemon(daemon);

	return (0);
}
