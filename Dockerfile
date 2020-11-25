FROM anvari/cmake:3.17.3-alpine3.11 AS build

COPY . /app

WORKDIR /app

RUN mkdir /app/build && \
    cd /app/build && \
    cmake .. && \
    make

FROM alpine:3.11

COPY --from=build /app/build/quarantine /usr/local/bin

CMD ["quarantine"]
