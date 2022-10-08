#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>

using namespace std;
int *map;
// https://blog.csdn.net/weixin_42715287/article/details/106024620
int file_mmap(string filename, int capacity) {

    int fd = open(filename.c_str(), O_RDWR | O_CREAT | O_TRUNC , (mode_t) 0600);
    if (fd == -1) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
        return -1;
    }
    /* Stretch the file size to the size of the (mmapped) array of KVPairs
         */
    size_t filesize = capacity * sizeof(int);
    // 这段代码有啥含义
    int result = lseek(fd, filesize - 1, SEEK_SET);
    if (result == -1) {
        close(fd);
        perror("Error calling lseek() to 'stretch' the file");
        exit(EXIT_FAILURE);
    }
    result = write(fd, "", 1);
    if (result != 1) {
        close(fd);
        perror("Error writing last byte of the file");
        exit(EXIT_FAILURE);
    }
    map = (int*) mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        close(fd);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }
    return fd;
}


int file_mmap_r(string filename, int capacity) {

    int fd = open(filename.c_str(), O_RDONLY);
    if (fd == -1) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
        return -1;
    }
    /* Stretch the file size to the size of the (mmapped) array of KVPairs
         */
    size_t filesize = lseek(fd,0,SEEK_END);

    // size_t filesize = capacity * sizeof(int);
    // // 这段代码有啥含义
    // int result = lseek(fd, filesize - 1, SEEK_SET);
    // if (result == -1) {
    //     close(fd);
    //     perror("Error calling lseek() to 'stretch' the file");
    //     exit(EXIT_FAILURE);
    // }
    // result = write(fd, "", 1);
    // if (result != 1) {
    //     close(fd);
    //     perror("Error writing last byte of the file");
    //     exit(EXIT_FAILURE);
    // }
    map = (int*) mmap(NULL, filesize, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        close(fd);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }
    return fd;
}

// 这个是可以的
int test_write(string data) {
    int len = data.length();
    // 打开文件
    int fd=open("output.txt", O_RDWR|O_CREAT, 00777);
    // lseek将文件指针往后移动file_size-1位
    lseek(fd,len-1,SEEK_END);  
    // 从指针处写入一个空字符；mmap不能扩展文件长度，这里相当于预先给文件长度，准备一个空架子
    write(fd, "", 1);
    // 使用mmap函数建立内存映射
    char* addr = (char*)mmap(NULL, len, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
    // 内存映射建立好了，此时可以关闭文件了
    close(fd);
    // 把data复制到addr里
    memcpy(addr, data.c_str(), len);
    // 解除映射
    munmap(addr, len);
    return 0;
}

int test_read() {
    // 打开文件
    int fd = open("output.txt", O_RDONLY);  
    // 读取文件长度
    int len = lseek(fd,0,SEEK_END);
    cout << "len:" << len << endl;
    // 建立内存映射
    // 这个已经满了 所以不能再继续写了
    char* addr = (char*)mmap(NULL, len, PROT_READ,MAP_SHARED, fd, 0);

    // char *addr = (char *) mmap(NULL, len, PROT_READ, MAP_PRIVATE,fd, 0);      
    close(fd);
    // data用于保存读取的数据
    char* data = new char[len]; 
    // 复制过来
    memcpy(data, addr, len);
    cout << data << endl;
    // 解除映射
    munmap(addr, len);
    return 0;
}


int read_file_mmap(int len) {
    int *run = new int[len];
    memcpy(run, map, len * sizeof(int));
    for (int i = 0; i < len; i ++) {
        cout << i << "\t" << run[i] << endl;
    }
    return 0;
}

int write_file_mmap(int len) {
    int *run = new int[len];
    for (int i = 0; i < len; i ++) {
        run[i] = i + 2;
    }
    memcpy(map, run, len * sizeof(int));
    return 0;
}

int test_write_vector_to_file(string filename, int capacity) {
    size_t filesize = capacity * sizeof(int);
    int fd = file_mmap(filename, capacity);
    if (fd == -1) {
        return -1;
    }
    write_file_mmap(capacity);
    if (munmap(map, filesize) == -1) {
        perror("Error un-mmapping the file");
    }    
    close(fd);
    return 0;
}

int test_read_vector_to_file(string filename, int capacity) {
    size_t filesize = capacity * sizeof(int);
    int fd = file_mmap_r(filename, capacity);
    if (fd == -1) {
        return -1;
    }
    read_file_mmap( capacity); // 读取的数据为啥都是空的, 数据是没写入进去吗

    if (munmap(map, filesize) == -1) {
        perror("Error un-mmapping the file");
    }    
    close(fd);
    return 0;
}

int main() {
    // test_write("beauty");
    // test_read();
    // return 0;
    string filename = "data_mmap_file3";
    int capacity = 10;
    test_write_vector_to_file(filename, capacity);
    // test_read_vector_to_file(filename, capacity);


    // fsync(fd); // 这行代码没啥用
    // msync(map, filesize, MS_SYNC); // 这行代码没啥用
    // https://blog.csdn.net/weixin_39541632/article/details/123002848
    
    
    

    return 0;
}
