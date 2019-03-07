FROM ubuntu:16.04

RUN apt update && apt upgrade -y && apt install nginx

ENV APP_DIR "/root/blog.zhangnan.xyz"

ADD . ${APP_DIR}

RUN cd ${APP_DIR} && cp blog.zhangnan.xyz.conf /etc/nignx/conf.d/blog.zhangnan.xyz.conf && service nginx restart

EXPOSE 80