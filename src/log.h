//
// Created by ahmad on 6/4/20.
//

#ifndef QUARANTINE_LOG_H
#define QUARANTINE_LOG_H

#include <stdio.h>
#include "http/request.h"

#define LOG(X) printf("%s\n", X)
#define LOGF(F, X) printf(F, X)

void log_request(request);

#endif //QUARANTINE_LOG_H
