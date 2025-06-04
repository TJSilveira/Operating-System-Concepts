#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define SIZE 4096
#define SHM_NAME "/OS"  // Note: added leading slash - required on some systems

// Debug version with extensive error checking
int write_and_read_debug() {
    int fd;
    char *ptr;
    const char *message = "Hello from shared memory!";
    
    printf("=== DEBUG: Write and Read Process ===\n");
    
    // Step 1: Create shared memory
    printf("Step 1: Creating shared memory '%s'\n", SHM_NAME);
    fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0666);
    if (fd == -1) {
        if (errno == EEXIST) {
            printf("Shared memory exists, trying to unlink and recreate...\n");
            shm_unlink(SHM_NAME);
            fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0666);
        }
        if (fd == -1) {
            perror("shm_open failed");
            return 1;
        }
    }
    printf("✓ Shared memory created, fd = %d\n", fd);
    
    // Step 2: Set size
    printf("Step 2: Setting size to %d bytes\n", SIZE);
    if (ftruncate(fd, SIZE) == -1) {
        perror("ftruncate failed");
        close(fd);
        shm_unlink(SHM_NAME);
        return 1;
    }
    printf("✓ Size set successfully\n");
    
    // Step 3: Map memory
    printf("Step 3: Mapping memory\n");
    ptr = (char *) mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        shm_unlink(SHM_NAME);
        return 1;
    }
    printf("✓ Memory mapped at address %p\n", (void*)ptr);
    
    // Step 4: Initialize memory (important!)
    printf("Step 4: Initializing memory\n");
    memset(ptr, 0, SIZE);  // Clear the memory first
    printf("✓ Memory initialized\n");
    
    // Step 5: Write message
    printf("Step 5: Writing message\n");
    strncpy(ptr, message, SIZE - 1);  // Use strncpy for safety
    ptr[SIZE - 1] = '\0';  // Ensure null termination
    printf("✓ Written to shared memory: '%s'\n", ptr);
    
    // Step 6: Read back
    printf("Step 6: Reading back from shared memory\n");
    printf("✓ Read from shared memory: '%s'\n", ptr);
    
    // Step 7: Cleanup
    printf("Step 7: Cleaning up\n");
    if (munmap(ptr, SIZE) == -1) {
        perror("munmap failed");
    }
    if (close(fd) == -1) {
        perror("close failed");
    }
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink failed");
    }
    printf("✓ Cleanup complete\n");
    
    return 0;
}

// Simplified version
int write_and_read_simple() {
    int fd;
    char *ptr;
    const char *message = "Hello World!";
    
    // Clean up any existing shared memory first
    shm_unlink(SHM_NAME);
    
    // Create shared memory
    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open failed");
        return 1;
    }
    
    // Set size
    if (ftruncate(fd, SIZE) == -1) {
        perror("ftruncate failed");
        close(fd);
        shm_unlink(SHM_NAME);
        return 1;
    }
    
    // Map memory
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        shm_unlink(SHM_NAME);
        return 1;
    }
    
    // Clear and write
    memset(ptr, 0, SIZE);
    strcpy(ptr, message);
    
    // Read and display
    printf("Message written and read: %s\n", ptr);
    
    // Cleanup
    munmap(ptr, SIZE);
    close(fd);
    shm_unlink(SHM_NAME);
    
    return 0;
}

// Original separate writer process
int writer_process() {
    int fd;
    char *ptr;
    const char *message = "Hello from writer process!";
    
    printf("=== WRITER PROCESS ===\n");
    
    // Clean up any existing shared memory
    shm_unlink(SHM_NAME);
    
    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open (writer) failed");
        return 1;
    }
    
    if (ftruncate(fd, SIZE) == -1) {
        perror("ftruncate failed");
        close(fd);
        shm_unlink(SHM_NAME);
        return 1;
    }
    
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap (writer) failed");
        close(fd);
        shm_unlink(SHM_NAME);
        return 1;
    }
    
    memset(ptr, 0, SIZE);
    strcpy(ptr, message);
    printf("Written to shared memory: %s\n", message);
    
    munmap(ptr, SIZE);
    close(fd);
    
    printf("Writer finished. Run reader now.\n");
    return 0;
}

// Reader process
int reader_process() {
    int fd;
    char *ptr;
    
    printf("=== READER PROCESS ===\n");
    
    fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (fd == -1) {
        perror("shm_open (reader) failed - make sure writer ran first");
        return 1;
    }
    
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap (reader) failed");
        close(fd);
        return 1;
    }
    
    printf("Read from shared memory: '%s'\n", ptr);
    
    munmap(ptr, SIZE);
    close(fd);
    shm_unlink(SHM_NAME);
    
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Choose version:\n");
        printf("Using simple write-and-read version...\n");
        return write_and_read_simple();
    }
    
    if (strcmp(argv[1], "debug") == 0) {
        return write_and_read_debug();
    } else if (strcmp(argv[1], "writer") == 0) {
        return writer_process();
    } else if (strcmp(argv[1], "reader") == 0) {
        return reader_process();
    } else {
        printf("Usage: %s [debug|writer|reader]\n", argv[0]);
        printf("  No args: simple write and read\n");
        printf("  debug: detailed debugging output\n");
        printf("  writer: create and write\n");
        printf("  reader: read existing\n");
        return 1;
    }
}
