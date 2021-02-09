#include "http_custom_client.h"

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            printf("HTTP_EVENT_ERROR\n");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            printf("HTTP_EVENT_ON_CONNECTED\n");
            break;
        case HTTP_EVENT_HEADER_SENT:
            printf("HTTP_EVENT_HEADER_SENT\n");
            break;
        case HTTP_EVENT_ON_HEADER:
            printf("HTTP_EVENT_ON_HEADER\n");
            printf("%.*s", evt->data_len, (char*)evt->data);
            break;
        case HTTP_EVENT_ON_DATA:
            printf("HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            if (!esp_http_client_is_chunked_response(evt->client)) {
                printf("%.*s", evt->data_len, (char*)evt->data);
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            printf("HTTP_EVENT_ON_FINISH\n");
            break;
        case HTTP_EVENT_DISCONNECTED:
            printf("HTTP_EVENT_DISCONNECTED\n");
            break;
    }
    return ESP_OK;
}

esp_err_t http_rest_with_url(const char *url, const char *post_data, const char *influx_token)
{
    esp_http_client_config_t config = {
        .url = url,
        .event_handler = _http_event_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // POST
    esp_http_client_set_url(client, url);
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_http_client_set_header(client, "Authorization", influx_token);
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        printf("HTTP POST Status = %d, content_length = %d\n",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        printf("HTTP POST request failed: %s\n", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
    return err;
}