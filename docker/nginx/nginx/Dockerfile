#FROM debian:bullseye-slim
FROM  nginx:latest

ENV PREREQUISITES "bash"

# ------ Install prerequisites
RUN apt-get update && \
    apt-get install -y $PREREQUISITES && \
#
#  ----- PREPARE SUBDIRECTRIES
    mkdir -p /usr/share/nginx/log && \
    mkdir -p /usr/share/nginx/lua && \
    mkdir -p /usr/share/nginx/js && \
    mkdir -p /data/images && \
    chmod 777 /data/images && \
    ls -l /etc/nginx/ && \
    cat /etc/nginx/nginx.conf && \
    ls -l /usr/share/nginx && \
    ls -l /usr/share/nginx/lua

