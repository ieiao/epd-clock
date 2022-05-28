idf_component_register(
    SRCS
    "net-manager.c"
    "http-server.c"
    "dns-server.c"
    "https-request.c"
    "weather.c"
    INCLUDE_DIRS
    "include"
    REQUIRES esp-tls zlib json esp_http_server storage main
)
