FROM centos
RUN yum update -y
RUN yum upgrade -y
RUN yum install -y gcc-c++ 
COPY lab7_client.cpp /home/lab7_client.cpp
COPY lab7_server.cpp /home/lab7_server.cpp
