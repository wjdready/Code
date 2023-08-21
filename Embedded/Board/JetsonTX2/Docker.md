
```sh
docker run -itd --name ubuntu-test ubuntu:22.04
docker ps

docker exec -it ubuntu-test bash
docker cp Jetson_Linux_R32.7.3_aarch64.tbz2 11c2685bcf13:/root
```
