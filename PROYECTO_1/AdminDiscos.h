#pragma once

#include <iostream>
#include <iomanip>
#include <ctime>
#include <stdio.h>
#include <cstring>
#include <sys/stat.h>
#include <math.h>
#include "FuncAuxiliar.h"

using namespace std;

void mkdisk(int size, string path, string name, char unit);
void rmdisk(string path);
void fdisk(int size, string path, string name, char unit, char type, char fit);
void fdiskDelete(string path, string name, string borrar);
void fdiskAdd(string path, string name, int add, char unit);
void fdiskMov(string path, string name);
void mount(string path, string name);
void printMount();
void unmount(vector<string> ids);
void mkfs(string id, string type);
vector<PrtMount> getList();