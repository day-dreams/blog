FROM ubuntu:16.04

RUN apt update && apt upgrade -y && apt install nginx tcpdump curl -y  

ENV APP_DIR "/root/www/blog.zhangnan.xyz"

ADD . ${APP_DIR}

RUN mkdir /root/blog.zhangnan.xyz

RUN cd ${APP_DIR} && ls && cp blog.zhangnan.xyz.conf /etc/nginx/conf.d/

ENTRYPOINT   tail -f /dev/null

EXPOSE 80