stocksim
========
# Introduction
This is a simple Stock data simulator.
Three parts:

    1)Server: DataSim.
    2)Connecter: PushPublish.
    3)Client: SubscribeClient.

   The DataSim generate stock data and push to the PushPublish.
   The PushPublish send message to SubscribeClient.

# Setup
###Install Node.js

Download from website:
http://nodejs.org/dist/v0.10.29/node-v0.10.29-linux-x64.tar.gz

    $ tar -zxf node-v0.10.29-linux-x64.tar.gz
    $ cd node-v0.10.29-linux-x64
    $ ./configure --prefix=/node_installdir
    $ make
    $ make install

Set environment path:
$vim /etc/profile
Add these parameters to the end of /etc/profile:

    export NODE_HOME=/node_installdir
    export PATH=$PATH:$NODE_HOME/bin
    export NODE_PATH=$NODE_HOME/lib/node_modules


###Install ZeroMQ
Download from website:
http://download.zeromq.org/zeromq-3.2.4.tar.gz

    $ tar -zxf zeromq-3.2.4.tar.gz
    $ cd zeromq-3.2.4
    $ ./configure
    $ make
    $ make check
    $ make install
    $ ldconfig


###Install ProtoBuf
Download from website:
https://code.google.com/p/protobuf/downloads/detail?name=protobuf-2.5.0.tar.gz

    $ tar -zxf protobuf-2.5.0.tar.gz
    $ cd protobuf-2.5.0
    $ ./configure --prefix=/protobuf_installdir
    $ make
    $ make check
    $ make install

Set environment path:

    $ vim /etc/profile
    
Add these parameters to the end of /etc/profile:

    export PROTOBUF_HOME=/protobuf_installdir
    export PATH=$PATH:$PROTOBUF_HOME/bin
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PROTOBUF_HOME/lib
    export PKG_CONFIG_PATH=$PROTOBUF_HOME/lib/pkgconfig


###Binding ZeroMQ with Node.js
Install ZeroMQ.Node:

    $ npm install zmq -g

Notice: npm install package depend on the internet connection. Another install method is:
Install from the source code:
https://github.com/JustinTulloss/zeromq.node

    $ cd zeromq.node-master
    $ npm install
    $ make 
    $ make test

if ok.
then cp this whole directory to $NODE_HOME/lib/node_modules


# Installation
###Get StockSim source code
Clone StockSim repository from GitHub:
git@github.com:yeahzzz/stocksim.git
or
Download from website:
https://github.com/yeahzzz/stocksim

###Generate ProtoBuf message format files

    $ cd stocksim/MessageFormat
    $ protoc -I=./ --cpp_out=../SubscribeClient ./message.proto

###Compile out .so

    $ cd stocksim/SubscribeClient
    $ g++ libsub.cpp message.pb.cc -I$PROTOBUF_HOME/include -L$PROTOBUF_HOME/lib -lprotobuf -lzmq -pthread -fPIC -shared -o libsub.so

###Build Client

    $ cd stocksim/SubscribeClient
    $ g++ client.cpp -L. -lsub -o client


# Running
###Start Server

    $ cd stocksim/DataSim
    $ python DataSim.py

###Start Pusher

    $ cd stocksim/PushPublish
    $ node PushPublish.js

###Start Client

    $ cd stocksim/SubscribeClient
    $ ./client
