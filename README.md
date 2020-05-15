
Build Status: [![CircleCI](https://circleci.com/gh/homer6/mutable.svg?style=svg)](https://circleci.com/gh/homer6/mutable)

Overview
--------

Mutable allows you to make schema migrations (schema mutations), data mutations (data migrations), and other mutations in a clean and modern way.

Mutable is designed to work with kubernetes and works with postgres, mongodb, mysql, and can support other datastores upon request.



Build Ubuntu 18
---------------

```
sudo apt install git gcc g++ cmake pkg-config libssl-dev libsasl2-dev python ccache libmysqlcppconn-dev libmysqlclient-dev libpq-dev
git clone --recursive https://github.com/homer6/mutable.git
cd mutable

cd lib/libmongoc
git checkout debian/1.15.2-1
python build/calc_release_version.py > VERSION_CURRENT
mkdir cmake-build
cd cmake-build
cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
sudo make -j12 install
cd ../../..

cd lib/mongocxx-driver/build
git checkout debian/3.4.1-1
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
sudo make -j12 install
cd ../../..

cd lib/libpqxx
git checkout 7.0.5
mkdir build
cmake -DPostgreSQL_TYPE_INCLUDE_DIR:STRING=/usr/include/postgresql ..
sudo make -j12 install
cd ../../..
ldconfig

sudo apt install -y libssl-dev libboost-all-dev libsasl2-dev liblz4-dev libzstd-dev
cd lib/librdkafka
git checkout v1.4.2
./configure --install-deps
make -j12
make install
ldconfig
cd ../..

cd lib/cppkafka
git checkout 006642cdb2a871ef3aad517e9148607b859604e0
cmake .
make -j12
make install
ldconfig
cd ../..

cmake .
make -j12
```


Building Docker
---------------

```
apt install -y docker.io

docker build -t mutable_base -f Dockerfile.base .
docker build -t mutable_build -f Dockerfile.build .
docker build -t mutable -f Dockerfile.run .
```

Publishing to dockerhub (after building)
----------------------------------------

```
docker login

docker tag mutable_base:latest homer6/mutable_base:v2
docker tag mutable_build:latest homer6/mutable_build:v2
docker tag mutable:latest homer6/mutable:v0.1.0
docker tag mutable:latest homer6/mutable:latest

docker push homer6/mutable_base:v2
docker push homer6/mutable_build:v2
docker push homer6/mutable:v0.1.0
docker push homer6/mutable:latest
```



Connecting to Buildtime Docker Container
--------------------------------------

```
docker run -it mutable_build
git remote add upstream https://github.com/homer6/mutable.git
git pull upstream master
make -j4
ldd build/mutable
```


Connecting to Runtime Docker Container
--------------------------------------

```
docker run -it --entrypoint /bin/ash mutable
```







Running (Locally)
-----------------

```
export POSTGRES_CONNECTION="postgresql://username@localhost/dbname?connect_timeout=10&application_name=myapp&ssl=true"
export MONGO_CONNECTION="mongodb://localhost/"

export MYSQL_USERNAME=root
export MYSQL_DATABASE=temp

export ENVIRONMENT_PREFIX="testing"

./build/mutable
```



Running (Docker)
----------------

```
export POSTGRES_CONNECTION="postgresql://username@localhost/dbname?connect_timeout=10&application_name=myapp&ssl=true"
export MONGO_CONNECTION="mongodb://localhost/"

export MYSQL_USERNAME=root
export MYSQL_DATABASE=temp

export BROKER_LIST="127.0.0.1:9092"

export ENVIRONMENT_PREFIX="testing"

docker run \
	--env POSTGRES_CONNECTION \
	--env MONGO_CONNECTION \
	--env MYSQL_USERNAME \
	--env MYSQL_DATABASE \
	--env BROKER_LIST \
	--env ENVIRONMENT_PREFIX \
	homer6/mutable
```
