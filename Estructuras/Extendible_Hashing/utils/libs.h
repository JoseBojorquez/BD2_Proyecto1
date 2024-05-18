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

typedef long long position_t;
typedef int depth_t;
typedef int suffix_t;
typedef long long bucketSize_t;
typedef string filename_t;

typedef int T;

T stringToKey(string k) {
    return stoi(k);
}