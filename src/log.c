//
// Created by ahmad on 11/25/20.
//

#include "log.h"

void log_request(request r) {
    printf("%s\t%s %s\n", r.protocol , r.method, r.path);
//    for (int i = 0; i < r.header_size; ++i)
//    {
//        printf("|%s:%s|", r.headers[i].name, r.headers[i].value);
//    }

//    if (r.body_size != 0) {
//        printf("body: %s\n", r.body);
//    }
}
