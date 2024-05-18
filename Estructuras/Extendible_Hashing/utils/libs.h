#include <iostream>
#include <utility>
#include <vector>
#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
#include <functional>
#include <stdio.h>
#include <chrono>
#include <sys/time.h>
#include <ctime>

using namespace std;

typedef long long pos_t;
typedef int depth_t;
typedef int sufix_t;
typedef long long bsize_t;
typedef string filename_t;

typedef int T;

T stringToKey(string k) {
    return stoi(k);
}