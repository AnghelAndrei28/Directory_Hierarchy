#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir {
  char * name;
  struct Dir * parent;
  struct File * head_children_files;
  struct Dir * head_children_dirs;
  struct Dir * next;
}
Dir;

typedef struct File {
  char * name;
  struct Dir * parent;
  struct File * next;
}
File;

//Initializare director
Dir * init_Dir(Dir * parent, char * name) {
  Dir * dir = (Dir * ) malloc(sizeof(Dir));
  dir -> name = (char * ) malloc((strlen(name) + 1) * sizeof(char));
  strcpy(dir -> name, name);
  dir -> parent = parent;
  dir -> head_children_dirs = NULL;
  dir -> head_children_files = NULL;
  dir -> next = NULL;
  return dir;
}

//Initializare fisier
File * init_File(Dir * parent, char * name) {
  File * file = (File * ) malloc(sizeof(File));
  file -> name = (char * ) malloc((strlen(name) + 1) * sizeof(char));
  strcpy(file -> name, name);
  file -> parent = parent;
  file -> next = NULL;
  return file;
}

//Se veerifica existenta unui alt director cu acelasi nume
int check_dir(Dir * head, char * name) {
  Dir * tmp = head;
  while (tmp != NULL) {
    if (strcmp(tmp -> name, name) == 0) {
      printf("Directory already exists\n");
      return 1;
    }
    tmp = tmp -> next;
  }
  return 0;
}

//Se verifica existenta unui alt fisier cu acelasi nume
int check_file(File * head, char * name) {
  File * tmp = head;
  while (tmp != NULL) {
    if (strcmp(tmp -> name, name) == 0) {
      printf("File already exists\n");
      return 1;
    }
    tmp = tmp -> next;
  }
  return 0;
}

//Functie de adaugare a unui director in lista de directoare
Dir * dir_add(Dir * parent, Dir * head, char * name) {

  Dir * new_dir = init_Dir(parent, name);

  if (head == NULL)
    return new_dir;
  else {
    Dir * tmp = head;
    if (strcmp(tmp -> name, name) == 0) {
      printf("Directory already exists\n");
      return head;
    } else if (strcmp(tmp -> name, name) > 0) {
      new_dir -> next = head;
      return new_dir;
    }
    while (tmp -> next != NULL) {
      if (strcmp(tmp -> next -> name, name) < 0) {
        tmp = tmp -> next;
      } else {
        break;
      }
    }
    if (tmp -> next == NULL) {
      tmp -> next = new_dir;
      return head;
    } else if (strcmp(tmp -> next -> name, name) == 0) {
      printf("Directory already exists\n");
      return head;
    } else {
      new_dir -> next = tmp -> next;
      tmp -> next = new_dir;
      return head;
    }
  }
  return head;
}

//Functie de adaugare a unui fisier in lista de fisiere
File * file_add(Dir * parent, File * head, char * name) {

  File * new_file = init_File(parent, name);

  if (head == NULL)
    return new_file;
  else {
    File * tmp = head;
    if (strcmp(tmp -> name, name) == 0) {
      printf("File already exists\n");
      return head;
    } else if (strcmp(tmp -> name, name) > 0) {
      new_file -> next = head;
      return new_file;
    }
    while (tmp -> next != NULL) {
      if (strcmp(tmp -> next -> name, name) < 0) {
        tmp = tmp -> next;
      } else {
        break;
      }
    }
    if (tmp -> next == NULL) {
      tmp -> next = new_file;
      return head;
    } else if (strcmp(tmp -> next -> name, name) == 0) {
      printf("File already exists\n");
      return head;
    } else {
      new_file -> next = tmp -> next;
      tmp -> next = new_file;
      return head;
    }
  }
  return head;
}

//Printarea listei de directoare
void dir_print(Dir * head) {
  if (head == NULL)
    return;
  else if (head -> next == NULL) {
    printf("%s\n", head -> name);
    return;
  } else {
    printf("%s\n", head -> name);
    dir_print(head -> next);
  }
}

//Printarea listei de fisiere
void file_print(File * head, int level) {
  int i;
  if (head == NULL)
    return;
  else if (head -> next == NULL) {
    for (i = 0; i < level; i++) {
      printf("    ");
    }
    printf("%s\n", head -> name);
    return;
  } else {
    for (i = 0; i < level; i++) {
      printf("    ");
    }
    printf("%s\n", head -> name);
    file_print(head -> next, level);
  }
}

//Stergerea tuturor fisierelor indiferent de nume
void delete_files(File ** head) {
  File * current = NULL;
  while ( * head != NULL) {
    current = * head;
    * head = ( * head) -> next;
    free(current -> name);
    free(current);
  }
}

//Stergerea tuturor directoarelor indiferent de nume
void delete_dirs(Dir ** head) {
  Dir * current;
  while (( * head) != NULL) {
    delete_files( & ( * head) -> head_children_files);
    free(( * head) -> head_children_files);
    delete_dirs( & (( * head) -> head_children_dirs));
    current = * head;
    * head = ( * head) -> next;
    free(current -> name);
    free(current);
  }
}

//Stergerea unui fisier cu un anumit nume
File * remove_file(File * head, char * name) {
  if (head == NULL) {
    printf("Could not find the file\n");
    return NULL;
  }
  File * tmp = head;
  if (strcmp(tmp -> name, name) == 0) {
    head = head -> next;
    free(tmp -> name);
    free(tmp);
    return head;
  }
  File * prev = NULL;
  while (tmp != NULL) {
    if (strcmp(tmp -> name, name) == 0) {
      prev -> next = tmp -> next;
      free(tmp -> name);
      free(tmp);
      return head;
    }
    prev = tmp;
    tmp = tmp -> next;
  }
  printf("Could not find the file\n");
  return head;
}

//Stergerea unui director cu un anumit nume
Dir * remove_directory(Dir * head, char * name) {
  if (head == NULL) {
    printf("Could not find the dir\n");
    return NULL;
  }
  Dir * tmp = head;
  if (strcmp(head -> name, name) == 0) {
    delete_dirs( & tmp -> head_children_dirs);
    delete_files( & tmp -> head_children_files);
    head = head -> next;
    free(tmp -> name);
    free(tmp);
    return head;
  }
  Dir * prev = NULL;
  while (tmp != NULL) {
    if (strcmp(tmp -> name, name) == 0) {
      delete_dirs( & tmp -> head_children_dirs);
      delete_files( & tmp -> head_children_files);
      prev -> next = tmp -> next;
      free(tmp -> name);
      free(tmp);
      return head;
    }
    prev = tmp;
    tmp = tmp -> next;
  }
  printf("Could not find the dir\n");
  return head;
}

void touch(Dir * parent, char * name) {
  parent -> head_children_files = file_add(parent,
    parent -> head_children_files, name);
}

void mkdir(Dir * parent, char * name) {
  parent -> head_children_dirs = dir_add(parent,
    parent -> head_children_dirs, name);
}

void ls(Dir * parent) {
  dir_print(parent -> head_children_dirs);
  file_print(parent -> head_children_files, 0);
}

void rm(Dir * parent, char * name) {
  parent -> head_children_files = remove_file(parent -> head_children_files, name);
}

void rmdir(Dir * parent, char * name) {
  parent -> head_children_dirs =
    remove_directory(parent -> head_children_dirs, name);
}

void cd(Dir ** target, char * name) {
  if (strcmp(name, "..") == 0) {
    if (( * target) -> parent != NULL) {
      * target = ( * target) -> parent;
      return;
    }

    return;
  }
  Dir * tmp = ( * target) -> head_children_dirs;
  while (tmp != NULL) {
    if (strcmp(tmp -> name, name) == 0) {
      * target = tmp;
      return;
    }
    tmp = tmp -> next;
  }
  printf("No directories found!\n");
}

char * pwd(Dir * target) {
  char * string = (char * ) malloc((strlen(target -> name) + 2) * sizeof(char));
  strcpy(string, "/");
  strcat(string, target -> name);
  target = target -> parent;
  while (target != NULL) {
    char * prev_string = (char * ) malloc((strlen(string) + 1) * sizeof(char));
    strcpy(prev_string, string);
    string = (char * ) realloc(string, (strlen(string) + strlen(target -> name) + 2) * sizeof(char));
    strcpy(string, "/");
    strcat(string, target -> name);
    strcat(string, prev_string);
    target = target -> parent;
    free(prev_string);
  }

  return string;
}

void stop(Dir * target) {
  while (target -> parent != NULL) {
    target = target -> parent;
  }
  delete_dirs( & (target) -> head_children_dirs);
  delete_files( & target -> head_children_files);
  free(target -> name);
  free(target);
}

void tree(Dir * target, int level) {
  int i;
  Dir * tmp = target -> head_children_dirs;
  while (tmp != NULL) {
    for (i = 0; i < level; i++) {
      printf("    ");
    }
    printf("%s\n", tmp -> name);
    tree(tmp, level + 1);
    file_print(tmp -> head_children_files, level + 1);
    tmp = tmp -> next;
  }
}

void mv(Dir * parent, char * oldname, char * newname) {
  /*Verific mai intai existenta unui director/fisier care sa aiba acelasi 
  nume cu cu noua denumire pentru a salva din timp*/
  Dir * tmp_dir = parent -> head_children_dirs;
  while (tmp_dir != NULL) {
    if (strcmp(tmp_dir -> name, newname) == 0) {
      printf("File/Director already exists\n");
      return;
    }
    tmp_dir = tmp_dir -> next;
  }
  File * tmp_file = parent -> head_children_files;
  while (tmp_file != NULL) {
    if (strcmp(tmp_file -> name, newname) == 0) {
      printf("File/Director already exists\n");
      return;
    }
    tmp_file = tmp_file -> next;
  }

  // Se verifica existenta a cel putin un director in directorul curent
  if (parent -> head_children_dirs != NULL) {
	//Se verifica daca trebuie eliminat directorul header
    tmp_dir = parent -> head_children_dirs;
    if (strcmp(tmp_dir -> name, oldname) == 0) {
      // Se salveaza adresele headerelor
      Dir * dirs_copy = tmp_dir -> head_children_dirs;
      File * file_copy = tmp_dir -> head_children_files;
      parent -> head_children_dirs = parent -> head_children_dirs -> next;
      // Elimin directorul
      free(tmp_dir -> name);
      free(tmp_dir);
      // Creez un director nou
      mkdir(parent, newname);
      Dir * tmp2 = parent -> head_children_dirs;
      while (tmp2 != NULL) {
        if (strcmp(tmp2 -> name, newname) == 0) {
          /*Setez pointerii catre headerele listelor de fisiere 
          si directoare*/
          tmp2 -> head_children_dirs = dirs_copy;
          tmp2 -> head_children_files = file_copy;
          return;
        }
        tmp2 = tmp2 -> next;
      }
    }
    Dir * prev = tmp_dir;
    while (tmp_dir != NULL) {
      if (strcmp(tmp_dir -> name, oldname) == 0) {
		//Se salveaza adresele headerelor
        Dir * dirs_copy = tmp_dir -> head_children_dirs;
        File * file_copy = tmp_dir -> head_children_files;
        prev -> next = tmp_dir -> next;
		//Elimin directorul
        free(tmp_dir -> name);
        free(tmp_dir);
        mkdir(parent, newname);
        Dir * tmp2 = parent -> head_children_dirs;
        while (tmp2 != NULL) {
          if (strcmp(tmp2 -> name, newname) == 0) {
			  /*Setez pointerii catre headerele listelor de fisiere 
          si directoare*/
            tmp2 -> head_children_dirs = dirs_copy;
            tmp2 -> head_children_files = file_copy;
            return;
          }
          tmp2 = tmp2 -> next;
        }
      }
	  //Se retine 
      prev = tmp_dir;
      tmp_dir = tmp_dir -> next;
    }
  }
  //Verific daca exista minim un fisier
  if (parent -> head_children_files != NULL) {
	//Se verifica daca trebuie sters fisierul header
    tmp_file = parent -> head_children_files;
    if (strcmp(tmp_file -> name, oldname) == 0) {
      parent -> head_children_files = parent -> head_children_files -> next;
	  //Se elimina fisierul
      free(tmp_file -> name);
      free(tmp_file);
	  //Se creeaza unul nou
      touch(parent, newname);
      return;
    }

    File * prev2 = tmp_file;
    while (tmp_file != NULL) {
      if (strcmp(tmp_file -> name, oldname) == 0) {
        prev2 -> next = tmp_file -> next;
		//Se elimina fisierul
        free(tmp_file -> name);
        free(tmp_file);
		//Se creeaza unul nou
        touch(parent, newname);
        return;
      }
	  //Se retine fisierul anterior
      prev2 = tmp_file;
      tmp_file = tmp_file -> next;
    }
  }
  printf("File/Director not found\n");
}

int main() {

  char * buffer = (char * ) calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
  size_t buffsize = MAX_INPUT_LINE_SIZE;
  Dir * dir = init_Dir(NULL, "home");

  do {
    getline( & buffer, & buffsize, stdin);
    char * token = strtok(buffer, " \n");
    if (strcmp(token, "mkdir") == 0) {
      token = strtok(NULL, " \n");
      if (!check_dir(dir -> head_children_dirs, token))
        mkdir(dir, token);
    }
    if (strcmp(token, "touch") == 0) {
      token = strtok(NULL, " \n");
      if (!check_file(dir -> head_children_files, token))
        touch(dir, token);
    }
    if (strcmp(token, "ls") == 0) {
      ls(dir);
    }
    if (strcmp(token, "rm") == 0) {
      token = strtok(NULL, " \n");
      rm(dir, token);
    }
    if (strcmp(token, "rmdir") == 0) {
      token = strtok(NULL, " \n");
      rmdir(dir, token);
    }
    if (strcmp(token, "cd") == 0) {
      token = strtok(NULL, " \n");
      cd( & dir, token);
    }
    if (strcmp(token, "tree") == 0) {
      tree(dir, 0);
      file_print(dir -> head_children_files, 0);
    }
    if (strcmp(token, "pwd") == 0) {
      char * string = pwd(dir);
      printf("%s\n", string);
      free(string);
    }
    if (strcmp(token, "mv") == 0) {
      token = strtok(NULL, " \n");
      char * token2 = strtok(NULL, " \n");
      mv(dir, token, token2);
    }
  } while (strcmp(buffer, "stop") != 0);
  free(buffer);
  stop(dir);
  return 0;
}
