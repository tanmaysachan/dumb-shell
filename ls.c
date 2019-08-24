#include "ls.h"

void ls(char** a,int n,char curr[]){
    DIR*p;
    struct dirent *d;
    if(!strcmp(a[0],"ls")){
//only ls is given

        if(n==1){
            p=opendir(curr);
            while(d=readdir(p)){
            if(strcmp(d->d_name,".") && strcmp(d->d_name,".."))
                printf("%s\n",d->d_name);
               // printf("%d\n",strcmp(d->d_name,"."));
            }    
        }
//only ls with flags         
        if(n==2 && a[1][0]=='-'){
            if(!strcmp(a[1],"-a")){
                p=opendir(curr);
                while(d=readdir(p))
                printf("%s\n",d->d_name);
                
            }
       // if(n==2 && sep[1][0]=='-'){
            if(!strcmp(a[1],"-l")){
                p=opendir(curr);
                while(d=readdir(p)){
                        struct stat buff;
                        if(strcmp(d->d_name,".") && strcmp(d->d_name,"..")){

		  				    stat(d->d_name, &buff);
					        printf( (S_ISDIR(buff.st_mode)) ? "d" : "-");
					        printf( (buff.st_mode & S_IRUSR) ? "r" : "-");
					        printf( (buff.st_mode & S_IWUSR) ? "w" : "-");
					        printf( (buff.st_mode & S_IXUSR) ? "x" : "-");
					        printf( (buff.st_mode & S_IRGRP) ? "r" : "-");
					        printf( (buff.st_mode & S_IWGRP) ? "w" : "-");
					        printf( (buff.st_mode & S_IXGRP) ? "x" : "-");
					        printf( (buff.st_mode & S_IROTH) ? "r" : "-");
					        printf( (buff.st_mode & S_IWOTH) ? "w" : "-");
					        printf( (buff.st_mode & S_IXOTH) ? "x" : "-");
					        printf("  %d", (int)buff.st_nlink);
					        struct passwd *pw = getpwuid(buff.st_uid);
						    struct group  *gr = getgrgid(buff.st_gid);
						    char date[20];
						    strftime(date, 20, "%b  %d  %I:%M", gmtime(&(buff.st_ctime)));
					        printf(" %s  %s  %d  %s  %s", pw->pw_name, gr->gr_name, (int)buff.st_size, date, d->d_name);
					        printf("\n");
                        }
                }
            }

            if(!strcmp(a[1],"-la") || !strcmp(a[1],"-al")){
                p=opendir(curr);
                while(d=readdir(p)){
                        struct stat buff;

		  				    stat(d->d_name, &buff);
					        printf( (S_ISDIR(buff.st_mode)) ? "d" : "-");
					        printf( (buff.st_mode & S_IRUSR) ? "r" : "-");
					        printf( (buff.st_mode & S_IWUSR) ? "w" : "-");
					        printf( (buff.st_mode & S_IXUSR) ? "x" : "-");
					        printf( (buff.st_mode & S_IRGRP) ? "r" : "-");
					        printf( (buff.st_mode & S_IWGRP) ? "w" : "-");
					        printf( (buff.st_mode & S_IXGRP) ? "x" : "-");
					        printf( (buff.st_mode & S_IROTH) ? "r" : "-");
					        printf( (buff.st_mode & S_IWOTH) ? "w" : "-");
					        printf( (buff.st_mode & S_IXOTH) ? "x" : "-");
					        printf("  %d", (int)buff.st_nlink);
					        struct passwd *pw = getpwuid(buff.st_uid);
						    struct group  *gr = getgrgid(buff.st_gid);
						    char date[20];
						    strftime(date, 20, "%b  %d  %I:%M", gmtime(&(buff.st_ctime)));
					        printf(" %s  %s  %d  %s  %s", pw->pw_name, gr->gr_name, (int)buff.st_size, date, d->d_name);
					        printf("\n");
                        
                }
            }
        


        }

      if(n==3 && a[1][0]=='-'){
          if(!strcmp(a[1],"-a")){
                p=opendir(a[2]);
                while(d=readdir(p))
                printf("%s\n",d->d_name);
                
            }
       // if(n==2 && sep[1][0]=='-'){
            if(!strcmp(a[1],"-l")){
                p=opendir(a[2]);
                while(d=readdir(p)){
                        struct stat buff;
                        if(strcmp(d->d_name,".") && strcmp(d->d_name,"..")){

		  				    stat(d->d_name, &buff);
					        printf( (S_ISDIR(buff.st_mode)) ? "d" : "-");
					        printf( (buff.st_mode & S_IRUSR) ? "r" : "-");
					        printf( (buff.st_mode & S_IWUSR) ? "w" : "-");
					        printf( (buff.st_mode & S_IXUSR) ? "x" : "-");
					        printf( (buff.st_mode & S_IRGRP) ? "r" : "-");
					        printf( (buff.st_mode & S_IWGRP) ? "w" : "-");
					        printf( (buff.st_mode & S_IXGRP) ? "x" : "-");
					        printf( (buff.st_mode & S_IROTH) ? "r" : "-");
					        printf( (buff.st_mode & S_IWOTH) ? "w" : "-");
					        printf( (buff.st_mode & S_IXOTH) ? "x" : "-");
					        printf("  %d", (int)buff.st_nlink);
					        struct passwd *pw = getpwuid(buff.st_uid);
						    struct group  *gr = getgrgid(buff.st_gid);
						    char date[20];
						    strftime(date, 20, "%b  %d  %I:%M", gmtime(&(buff.st_ctime)));
					        printf(" %s  %s  %d  %s  %s", pw->pw_name, gr->gr_name, (int)buff.st_size, date, d->d_name);
					        printf("\n");
                        }
                }
            }

            if(!strcmp(a[1],"-la") || !strcmp(a[1],"-al")){
                p=opendir(a[2]);
                while(d=readdir(p)){
                        struct stat buff;

		  				    stat(d->d_name, &buff);
					        printf( (S_ISDIR(buff.st_mode)) ? "d" : "-");
					        printf( (buff.st_mode & S_IRUSR) ? "r" : "-");
					        printf( (buff.st_mode & S_IWUSR) ? "w" : "-");
					        printf( (buff.st_mode & S_IXUSR) ? "x" : "-");
					        printf( (buff.st_mode & S_IRGRP) ? "r" : "-");
					        printf( (buff.st_mode & S_IWGRP) ? "w" : "-");
					        printf( (buff.st_mode & S_IXGRP) ? "x" : "-");
					        printf( (buff.st_mode & S_IROTH) ? "r" : "-");
					        printf( (buff.st_mode & S_IWOTH) ? "w" : "-");
					        printf( (buff.st_mode & S_IXOTH) ? "x" : "-");
					        printf("  %d", (int)buff.st_nlink);
					        struct passwd *pw = getpwuid(buff.st_uid);
						    struct group  *gr = getgrgid(buff.st_gid);
						    char date[20];
						    strftime(date, 20, "%b  %d  %I:%M", gmtime(&(buff.st_ctime)));
					        printf(" %s  %s  %d  %s  %s", pw->pw_name, gr->gr_name, (int)buff.st_size, date, d->d_name);
					        printf("\n");
                        
                }
            }
        




      }

//ls with Directory Only
      if(n==2 && a[1][0]!='-'){
           p=opendir(a[1]);
            while(d=readdir(p)){
            if(strcmp(d->d_name,".") && strcmp(d->d_name,".."))
                printf("%s\n",d->d_name);
            }
      }




    }


   







}