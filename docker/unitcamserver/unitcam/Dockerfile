FROM raspv6:latest

ENV PREREQUISITES "bash curl"

RUN apt-get update && \
    apt-get install -y $PREREQUISITES && \
    mkdir -p /work && \
    mkdir -p /images && \
    #chmod a+x ./get-image.sh && \
    chmod 777 /images

COPY ./get-image.sh /work/get-image.sh

WORKDIR /work

ENTRYPOINT ["bash"]
CMD ["/work/get-image.sh"]
