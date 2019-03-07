#! /bin/bash

docker stop my-blog-instance
docker container rm my-blog-instance
docker build -t my-blog-image .
docker run -d --name my-blog-instance -p 9394:80 my-blog-image
