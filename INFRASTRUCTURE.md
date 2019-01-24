# Partons-examples
This short instruction describes Docker infrastructure of this project created to make your experience of using it as smooth as possible. Reading this document does not replace reading full Docker documentation, but is enough to get around the code and start developing bits of functionality on your own.

Quick Docker reference:
- [Mac installation instruction](https://docs.docker.com/docker-for-mac/install/);
- [Docker "Get started" tutorial](https://docs.docker.com/get-started/);
- [Dockerfile syntax](https://docs.docker.com/engine/reference/builder/);
- [docker-compose.yml syntax](https://docs.docker.com/compose/compose-file/compose-file-v2/).

# Docker installation
## Mac installation
Please follow the [official instructions](https://docs.docker.com/docker-for-mac/install/).

## Ubuntu installation
Please follow the [official instructions](https://docs.docker.com/install/linux/docker-ce/ubuntu/) on installing Docker CE.

To finish installation and get `docker-compose` follow the [official instructions](https://docs.docker.com/compose/install/).

# Working with an example project
To start working with the project, the only prerequisites are:
1. Installed Docker (please refer to the section above);
2. Around 2 GB of free space on the drive for the [Debian](https://hub.docker.com/_/debian) and [MySQL](https://hub.docker.com/_/mysql) images upon which we construct the rest of the system required for the project operation;
3. Good internet connection, especially for the first run.
Please be patient on the first runs. Docker automatically acquires images and source code of PARTONS project to compile it in place.

> NOTE: For the further work, images with the compiled code and pre-prepared database can be stored on the [Docker Hub](http://hub.docker.com/), so the first run for the end-user will be much faster, less demanding and fewer files needed to be included into the project which would decrease initial confusion as much as possible.

## Running the examples
Running examples have never been easier! The very first step you need to do is to compile a local version of the code. It can be done using command `./compile.sh`. For more details on this step, please refer to the section below.

Please use `./partons.sh` bash script to run  examples. Under the hood, it runs docker container with the PARTONS code. For example, running an XML scenario can be done in the following way:
```
./partons.sh data/examples/observable/computeManyKinematicsForDVCSObservable.xml
```

## Altering the code and compiling changes
Usually, the pure fun lay in the C++ coding, right? As soon as you played enough with XML scenarios and started changing C++ code you need to compile one. For the purpose, please use ./compile.sh. Your very first run must have an output like:
```
$ ./compile.sh
-- The CXX compiler identification is GNU 4.9.2
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Looking for Q_WS_X11
-- Looking for Q_WS_X11 - found
-- Looking for Q_WS_WIN
-- Looking for Q_WS_WIN - not found
-- Looking for Q_WS_QWS
-- Looking for Q_WS_QWS - not found
-- Looking for Q_WS_MAC
-- Looking for Q_WS_MAC - not found
-- Found Qt4: /usr/bin/qmake (found version "4.8.6")
-- Found SFML .. in /usr/include
-- Found ELEMENTARY_UTILS: /usr/local/include/PARTONS
-- Found NUMA: /usr/local/include/PARTONS
-- Found PARTONS: /usr/local/include/PARTONS
-- Configuring done
-- Generating done
-- Build files have been written to: /root/workspace/partons-example/build
Scanning dependencies of target PARTONS_example_automoc
[ 25%] Automatic moc for target PARTONS_example
[ 25%] Built target PARTONS_example_automoc
Scanning dependencies of target PARTONS_example
[ 50%] Building CXX object CMakeFiles/PARTONS_example.dir/src/examples.cpp.o
[ 75%] Building CXX object CMakeFiles/PARTONS_example.dir/src/main.cpp.o
[100%] Building CXX object CMakeFiles/PARTONS_example.dir/PARTONS_example_automoc.cpp.o
Linking CXX executable ../bin/PARTONS_example
[100%] Built target PARTONS_example
```

Under the hood the `./build/` folder is mounted to the container, where all the compilation happens. A first part of the output of the initial run is a configuration done by cmake. If you changed cmake configuration you might be interested in cleaning up the content of build folder to perform configuration once again.

Later every run of `./compile.sh` performs `make` in the container, therefore compiles the code for you.

## Accessing bash shell at the PARTONS container
If for any reason (you definitely have a good one) you want to access bash shell at the Debian environment where compilation and execution of the code happens you must use `./shell.sh` script for this. It gives you access to the bash shell of the container. Also, this script can be used to run simple bash commands on the container side.

Then you are running `./shell.sh`, here's an expected output:
```
$ ./shell.sh
Starting partons-example_mysql_1 ... done
Building shell
Step 1/5 : FROM debian:jessie
 ---> 51a215bbbb19
Step 2/5 : ENV WORKSPACE="/root/workspace/"
 ---> Using cache
 ---> 04cb0e45aa73
Step 3/5 : WORKDIR ${WORKSPACE}
 ---> Using cache
 ---> bb99f9527cb8
Step 4/5 : RUN apt-get update &&     apt-get install -y libeigen3-dev libcln-dev libsfml-dev libqt4-dev libqt4-sql-mysql cmake git
 ---> Using cache
 ---> 9f09aec537f3
Step 5/5 : RUN cd ${WORKSPACE} &&     git clone https://drf-gitlab.cea.fr/partons/core/elementary-utils.git &&     cd ${WORKSPACE}elementary-utils/build &&     cmake -G"Unix Makefiles" ../ -DCMAKE_BUILD_TYPE=Debug &&     make &&     make install &&     cd ${WORKSPACE} &&     git clone https://drf-gitlab.cea.fr/partons/core/numa.git &&     cd ${WORKSPACE}numa/build &&     cmake -G"Unix Makefiles" ../ -DCMAKE_BUILD_TYPE=Debug &&     make &&     make install &&     cd ${WORKSPACE} &&     git clone https://drf-gitlab.cea.fr/partons/core/partons.git &&     cd ${WORKSPACE}partons/build &&     cmake -G"Unix Makefiles" ../ -DCMAKE_BUILD_TYPE=Debug &&     make &&     make install
 ---> Using cache
 ---> 2c60d6a9d331
Successfully built 2c60d6a9d331
Successfully tagged partons-example_shell:latest
WARNING: Image for service shell was built because it did not already exist. To rebuild this image you must use `docker-compose build` or `docker-compose up --build`.
root@610baf81cafd:~/workspace#
```
Not all parts of it would be the same, nevertheless at the point of `root@610baf81cafd:~/workspace#` you are already there.

### The inner structure of the container
To the container you login as root user, thus you have full administrative permissions to install and modify software, but be aware, that any changes you'll perform will be lost as soon as you stop the container. To persist any of the changes you need for the further work on the project you might need to append your changes to the .docker/Dockerfile.partons file, for detailed information on how to do this, please refer to [official documentation](https://docs.docker.com/engine/reference/builder/).

Working files at the container have the following structure:
```
/root
 /workspace
  /elementary-utils
  /numa
  /partons
  /partons-example  # Your code (current directory) is mounted here. Therefore all the changes on your local machine immediately reflected at the container
```

## Stopping all containers
After the very first run of `./partons.sh` command a MySQL server container will run in the background. You can see it viewing the output of the command `docker ps`:
```
$ docker ps
CONTAINER ID        IMAGE                   COMMAND                  CREATED             STATUS                  PORTS                 NAMES
7c092816e56d        partons-example_mysql   "/entrypoint.sh mysq…"   10 hours ago        Up 10 hours (healthy)   3306/tcp, 33060/tcp   partons-example_mysql_1
```
Where `7c092816e56d` is a unique identifier of the container, this value is changing from run to run, do not rely on it. To stop this last container (which  is not resource consuming, since it is idle), please execute: `docker-compose stop`, expected output might look like:
```
$ docker-compose stop
Stopping partons-example_mysql_1 ... done
```

# More on Docker
Docker is a very powerfull piece of software which makes your infrastructure portable, easy to maintain and release. Here will be covered certain basics applicable for the current project and setup, but for the complete reference it is definitely worth your time to read official [Get Started](https://docs.docker.com/get-started/) guide. To become more advanced user it is recommended to read through pages on the [Dockerfile syntax](https://docs.docker.com/engine/reference/builder/) and [docker-compose.yml syntax](https://docs.docker.com/compose/compose-file/compose-file-v2/).

> NOTE: In this project we still use version 2 for `docker-compose.yml` which, I hope is going to be update to version 3. But there's nothing beyond `docker-compose.yml` does not rely on the currently used version, therefore there's nothing preventing user to use newer versions of this syntax.

## Commands disassembled
Looking closely at `docker-compose.yml` you can see their among services: `shell`, `compile`, `partons`. Looking into corresponding `*.sh` files, you can see that there used following command `docker-compose run --rm compile`. Let's figure out what it is on the example of `compile` command. So, here are required listings with additional comments:

**docker-compose.yml**
```
  compile:
    build:
      context: .
      dockerfile: .docker/Dockerfile.partons
    restart: 'no'
    volumes:
      - ".:/root/workspace/partons-example"
      - ".docker/scripts:/root/scripts"
    environment:
      ENV: Local
    command: /bin/bash /root/scripts/compile_example.sh
```
An now, line by line. This name is going to be used in the `docker-compose run --rm *` command.
```
  compile:
```
Here we're telling to the Docker system what image and context to provision the container for the service.
```
    build:
```
The context for the Docker is the folder against which it will be running since Dockerfile interacts with the filesystem.
```
      context: .
```
We must tell Docker how to construct the system we want to use for further operations. Dockerfile contains all of these instructions. For more details, please refer to the [official documentation](https://docs.docker.com/engine/reference/builder/).
```
      dockerfile: .docker/Dockerfile.partons
```
This line would tell Docker to not start your services automatically on the start. Please read the documentation very well before changing this option, thoughtless manipulation with it can cause you problems as soon as you feel more confident to go switching your other projects to Docker.

```
    restart: 'no'
```
Here we define what and how external volumes would be mounted to the container. We define it as `<local folder>:<folder at the container>`
```
    volumes:
      - ".:/root/workspace/partons-example"
      - ".docker/scripts:/root/scripts"
```
Default command to be executed if no other is defined.

```
    command: /bin/bash /root/scripts/compile_example.sh
```
Using this approach you can extend `docker-compose.yml` (which is done in YAML format) to incorporate other routines and automate the hell out of all tasks you feel bored to repeat again and again.

## Cleanup
During first run Docker would build a new image creating an intermediate layer, so then next rebuilds, if needed, can be performed faster. Such behaviour is a good advantage for the programmers who are rebuilding their images many times a day, but in our case, this is an implausible scenario. More commonly you'll have very limited disk space at you working computer/laptop. In this case to clean up a system perform a simple cleanup using following docker command:
```
$ docker system prune
WARNING! This will remove:
        - all stopped containers
        - all networks not used by at least one container
        - all dangling images
        - all dangling build cache
Are you sure you want to continue? [y/N]
```

This standard output of this command informs you what kind of the objects are going to be removed. Don't worry. You won't remove anything necessary for the future work. If Docker deletes objects necessary for its further operation, it will create them on demand.

## Resetting database
For whatever purposes you would need it, to recreate database anew you must perform following commands:
```
# First you need to stop all containers of the database, or those are using the database. The global shutdown is easily achieved with:
docker-compose stop

# Now we need to remove local volume which is used to store database data between runs of this container:
docker-compose rm -v
```

On the next run removed volume would be recreated again and populated with the fresh database schema. Therefore do not afraid experiment!
