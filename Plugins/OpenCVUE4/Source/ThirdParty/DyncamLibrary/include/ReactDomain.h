//
// Created by ascadian on 4/20/17.
//

#ifndef DYNCAM_REACTDOMAIN_H
#define DYNCAM_REACTDOMAIN_H

#include <react/Domain.h>
#include <react/Event.h>
#include <react/Observer.h>
#include <react/Signal.h>
#include <react/Algorithm.h>

using namespace react;

//REACTIVE_DOMAIN(D, parallel_concurrent)
//REACTIVE_DOMAIN(D, sequential_concurrent)
REACTIVE_DOMAIN(D, sequential)
USING_REACTIVE_DOMAIN(D)

#endif //DYNCAM_REACTDOMAIN_H
