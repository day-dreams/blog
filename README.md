* url 
  * [https://blog.zhangnan.xyz](https://blog.zhangnan.xyz)
* files
  * nginx.conf docker宿主机上的nginx配置
  * blog.zhangnan.xyz.conf docker容器内的nginx配置
* deploy
    ```bash
    cp nignx.conf /etc/nginc/conf.d/blog.zhangnan.xyz.conf
    service nginx reload
    sh start.sh
    ```