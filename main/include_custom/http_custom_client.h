#include <string.h>
#include <stdlib.h>

#include "esp_http_client.h"

esp_err_t _http_event_handler(esp_http_client_event_t *evt);

esp_err_t http_rest_with_url(const char *url, const char *post_data, const char *influx_token);