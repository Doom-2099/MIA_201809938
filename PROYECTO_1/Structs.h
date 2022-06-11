#pragma once

using namespace std;

struct Particion
{
    int part_start;
    int part_size;
    char part_status;
    char part_type;
    char part_fit;
    char part_name[16];
};

struct MBR
{
    int mbr_size;
    int mbr_disk_signature;
    char mbr_fecha_creacion[19];
    Particion mbr_partition_1;
    Particion mbr_partition_2;
    Particion mbr_partition_3;
    Particion mbr_partition_4;
};

struct EBR
{
    int part_start;
    int part_size;
    int part_next;
    char part_fit;
    char part_name[16];
};

struct BusyBlock
{
    int start;
    int end;
};

struct FreeBlock
{
    int start;
    int end;
    int size;
};

struct PrtMount 
{
    string id;
    string path;
    string name;
    char part_fit;
    int part_start;
    int part_size;
};

struct TypeBlock
{
    int start;
    char type;
};