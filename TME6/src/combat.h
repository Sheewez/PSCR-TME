#ifndef __COMBAT_H__
#define __COMBAT_H__
#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

void attaquer(pid_t adversaire);
void defense();
void combat(pid_t adversaire);
int wait_till_pid(pid_t pid);

#endif
