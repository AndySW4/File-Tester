// Andrew Wood


 #include <stdio.h>      // printf(), perror()
 #include <stdlib.h>     // exit(), malloc(), free(), strtol()
 #include <unistd.h>     // read(), write(), lseek(), close()
 #include <fcntl.h>      // open() O_RDWR
 #include <string.h>     // strcmp()
 
 int main(int argc, char *argv[])
 {
     // We expect exactly 1 argument for the filename
     if (argc != 2) {
         fprintf(stderr, "./pa5test error: invalid number of arguments\n");
         // Non-zero exit status
         exit(255);
     }
 
     // Try to open filename for read/write
     int fd = open(argv[1], O_RDWR);
     if (fd < 0) {
         perror("./pa5test error: invalid filename");
         exit(255);
     }
 
     // Our infinite loop to prompt the user
     while (1) {
        
         printf("Option (r for read, w for write, s for seek): ");
         fflush(stdout);
 
         // We'll read the user input into a small buffer
         char option[32];
         if (fgets(option, sizeof(option), stdin) == NULL) {
             // This indicates EOF (Ctrl+D) at the prompt -> exit gracefully
             printf("\nExiting...\n");
             close(fd);
             return 0; 
         }
 
         // Strip trailing newline if present
         size_t len = strlen(option);
         if (len > 0 && option[len - 1] == '\n') {
             option[len - 1] = '\0';
         }
 
         // Compare the user input
         if (strcmp(option, "r") == 0) {
             // READ
             printf("Enter the number of bytes you want to read: ");
             fflush(stdout);
 
             char readSizeBuf[32];
             if (fgets(readSizeBuf, sizeof(readSizeBuf), stdin) == NULL) {
                 // EOF -> exit
                 printf("\nExiting...\n");
                 close(fd);
                 return 0;
             }
 
             long readSize = strtol(readSizeBuf, NULL, 10);
             if (readSize <= 0) {
                 // Ignore negative or 0 reads
                 continue;
             }
 
             // Allocate buffer
             char *buffer = (char *)malloc(readSize + 1);
             if (!buffer) {
                 fprintf(stderr, "Memory allocation error.\n");
                 close(fd);
                 return 1;
             }
 
             // Read from the file
             ssize_t bytesRead = read(fd, buffer, readSize);
             if (bytesRead < 0) {
                 perror("Error reading file");
                 free(buffer);
                 continue;
             }
 
             // Null-terminate for safe print
             buffer[bytesRead] = '\0';
 
             printf("%s\n", buffer);
             free(buffer);
 
         } else if (strcmp(option, "w") == 0) {
             // WRITE
             printf("Enter the data you want to write: ");
             fflush(stdout);
 
             // We'll read the entire line
             char writeBuf[1024];
             if (fgets(writeBuf, sizeof(writeBuf), stdin) == NULL) {
                 // EOF -> exit
                 printf("\nExiting...\n");
                 close(fd);
                 return 0;
             }
 
             // determine length
             size_t writeLen = strlen(writeBuf);
 
             // removing trailing newline
             if (writeLen > 0 && writeBuf[writeLen - 1] == '\n') {
                 writeBuf[writeLen - 1] = '\0';
                 writeLen--;
             }
 
             // Write to the file
             if (writeLen > 0) {
                 ssize_t bytesWritten = write(fd, writeBuf, writeLen);
                 if (bytesWritten < 0) {
                     perror("Error writing to file");
                 }
             }
 
         } else if (strcmp(option, "s") == 0) {
             // SEEK
             printf("Enter an offset value: ");
             fflush(stdout);
 
             char offsetBuf[32];
             if (fgets(offsetBuf, sizeof(offsetBuf), stdin) == NULL) {
                 // EOF -> exit
                 printf("\nExiting...\n");
                 close(fd);
                 return 0;
             }
 
             long offsetVal = strtol(offsetBuf, NULL, 10);
 
             printf("Enter a value for whence: ");
             fflush(stdout);
 
             char whenceBuf[32];
             if (fgets(whenceBuf, sizeof(whenceBuf), stdin) == NULL) {
                 // EOF -> exit
                 printf("\nExiting...\n");
                 close(fd);
                 return 0;
             }
 
             long whenceVal = strtol(whenceBuf, NULL, 10);
 
             // whenceVal should be 0, 1, or 2 for SEEK_SET, SEEK_CUR, SEEK_END
             // We won't validate beyond that
             off_t newPos = lseek(fd, offsetVal, whenceVal);
             if (newPos == (off_t)-1) {
                 perror("Error seeking in file");
             }
 
         } else if (strcmp(option, "") == 0) {
             // If user just pressed enter with no input, ignore
             continue;
         } else {
             // Ignore everything else
             continue;
         }
     }
 
     // just in case
     close(fd);
     return 0;
 }
 