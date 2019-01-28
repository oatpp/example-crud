FROM lganzzzo/alpine-cmake:latest

ADD . /service

WORKDIR /service/build

RUN cmake ..
RUN make

EXPOSE 8000 8000

ENTRYPOINT ["make", "run"]