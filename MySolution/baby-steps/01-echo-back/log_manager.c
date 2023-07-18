#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "log_manager.h"

#define LOG_FILE "logs.txt"

#include <stdio.h>

void view_logs(struct log_manager *manager, int num_lines) {
   /* TODO bonus */
   int total_lines = 0;
   char buf[1024];
   int bytes_read = 0;

   off_t pos = lseek(manager->fd, 0, SEEK_CUR);

   lseek(manager->fd, 0, SEEK_SET);
   while ((bytes_read = read(manager->fd, buf, 1024)) > 0){
      for (int i = 0; i < bytes_read; i++)
         if (buf[i] == '\n')
            total_lines++;
   }
   int skip_lines = total_lines - num_lines;

   lseek(manager->fd, 0, SEEK_SET);
   while ((bytes_read = read(manager->fd, buf, 1024)) > 0){
      for (int i = 0; i < bytes_read; i++)
      {
         if (skip_lines > 0)
         {
            if (buf[i] == '\n')
               skip_lines--;
         }
         else
            write(STDOUT_FILENO, &buf[i], sizeof(buf[i]));
      }
   }
   
   lseek(manager->fd, pos, SEEK_CUR); // 此处不太理解为什么是当前位置重定向，而不是文件开头重定向
   // lseek(manager->fd, pos, SEEK_SET);
}

void delete_logs(struct log_manager *manager) 
{
   /* TODO bonus */
   close(manager->fd);
   manager->fd = open(LOG_FILE, O_TRUNC | O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}

void disable_logs(struct log_manager *manager)
{
   /* TODO bonus */
   manager->enabled = 0;
}

void enable_logs(struct log_manager *manager)
{
   /* TODO bonus */
   manager->enabled = 1;
}

struct log_manager *create_manager()
{
   /* TODO bonus */
   static int created = 0;
   static struct log_manager *manager = NULL;

   if (created){
      return manager;
   }

   manager = malloc(sizeof(*manager));
   if (!manager){
      return NULL;
   }

   manager->enabled = 1;
   manager->fd = open(LOG_FILE, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
   if (manager->fd == -1){
      free(manager);
      return NULL;
   }

   created = 1;
   return manager;
}

void destroy_manager(struct log_manager *manager)
{
   /* TODO bonus */
   close(manager->fd);
   free(manager);
}
