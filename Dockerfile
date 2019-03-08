FROM ubuntu:16.04

RUN apt update && apt upgrade -y && apt install nginx tcpdump curl -y  

ENV APP_DIR "/var/www/blog.zhangnan.xyz"

ADD . ${APP_DIR}

RUN cd ${APP_DIR} &&  \
    mv blog.zhangnan.xyz.conf /etc/nginx/conf.d/ && \
    rm Dockerfile LISENCE nginx.conf README.md start.sh 

ENTRYPOINT service nginx restart &&  tail -f /dev/null

EXPOSE 80
