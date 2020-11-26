#pragma once

#ifndef _COINS64F_H
#define _COINS64F_H

#include "secp256k1class.h"
#include "keccak.h"
#include <stdint.h>
#include <memory.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>

using namespace std;

void HashMap();

uint64_t LoadFiles();

uint64_t Add(string line);

void DeleteHashMap();

void printh(uint64_t* a, uint64_t len, bool rmv);

void printkp(uint64_t* k, uint64_t* a);

#endif