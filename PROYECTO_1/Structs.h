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
    char mbr_fecha_creacion[20];
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

struct SuperBloque
{
    int s_filesystem_type;
    int s_inodes_count;
    int s_blocks_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    char s_mtime[20];
    char s_umtime[20];
    int s_mnt_count = 0;
    int s_magic;
    int s_inode_size;
    int s_block_size;
    int s_first_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;
};

struct TablaInodos
{
    int i_uid;
    int i_gid;
    int i_size;
    char i_atime[20];
    char i_ctime[20];
    char i_mtime[20];
    int i_block[15];
    char i_type;
};

struct Content
{
    char b_name[12];
    int b_inodo;
};

struct BloqueCarpeta
{
    Content b_content[4];
};

struct BloqueArchivo
{
    char name[14];
    char content[50];
};

struct BloqueApuntadores
{
    int b_pointers[16];
};

struct Journal
{
    char journal_tipo_op;
    char journal_tipo;
    char journal_nombre[15];
    char journal_contenido;
    char journal_fecha[20];
    int next;
};


// ------------------- STRUCTS AUXILIARES --------------------------------------------------------
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
    char mountDate[20];
};

struct TypeBlock
{
    int start;
    char type;
};

enum Operation
{
    NEW_CARPET = '1',
    NEW_FILE = '2'
};