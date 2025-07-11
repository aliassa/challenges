#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 64
typedef enum ObjType {
  ROOM,
  CAR,
  DOG,
  TV,
  PERSON,
  HOOD,
  PARKING_LOT,
  TYPE_COUNT
} ObjType_t;

const char* enum_to_string(ObjType_t Type) {
    switch (Type)
    {
    case ROOM:
        return "ROOM";
        break;
    break;
    case CAR:
        return "CAR";
        break;
    break;
    case DOG:
        return "DOG";
        break;
    break;
    case TV:
        return "TV";
        break;
    break;
    case PERSON:
        return "PERSON";
        break;
    break;
    case HOOD:
        return "HOOD";
        break;
    break;
    case PARKING_LOT:
        return "PARKING_LOT";
        break;
    break;    
    default:
        return "NONE";
        break;
    break;
    }
}
typedef struct funcs {
  void (*act)(void *);
  void (*deleter)(void *);
  void (*adder)(void *);
} Funcs_t;

typedef struct Object {
  ObjType_t type;
  char name[MAX_NAME];
  Funcs_t funcs;
  struct Object *father_objects;
  struct Object *child_objects;
  struct Object *neighbour_objects;
} Object_t;

/* Linked list of Objects_t */

typedef struct Node {
  Object_t *object;
  struct Node *next;
} Node_t;

Node_t* Objects[TYPE_COUNT];

Object_t *add_node(Node_t **node);

/* Returns the new objects so that we initialize it after calling add_node s*/
Object_t *add_node(Node_t **head) {
    if (!head)
        return NULL;

    Node_t *new_node = calloc(1, sizeof(Node_t));
    if (!new_node)
        return NULL;

    new_node->object = calloc(1, sizeof(Object_t));
    if (!new_node->object) {
        free(new_node);
        return NULL;
    }

    if (*head == NULL) {
        // List is empty: set new node as head
        *head = new_node;
    } else {
        // Traverse to end and append
        Node_t *current = *head;
        while (current->next)
            current = current->next;
        current->next = new_node;
    }

    return new_node->object;
}

Object_t *find_object_by_name(Node_t *node, const char *name);
Object_t *find_object_by_name(Node_t *node, const char *name) {
  while (node) {
    if (strcmp(node->object->name, name) == 0)
      return node->object;
    node = node->next;
  }
  return NULL;
}
Object_t *create_object(char *name, ObjType_t Type);

Object_t *create_object(char *name, ObjType_t Type) {
    if(Type >= TYPE_COUNT) {
        printf("No such type\n");
        return NULL;
    }
    Object_t *obj = add_node(&Objects[Type]);
    if(!obj) {
        printf("Can't create obj\n");
        return NULL;
    }
    obj->type = Type;
    strcpy(obj->name, name);
    return obj;
}

void print_list(Node_t* head) {
    if(!head) { printf("List empty"); }
    Node_t* node = head;
    while (node)
    {
        printf("%s -> ", node->object->name);
        node = node->next;
    }
    printf("\n");
}
void print_objects() {
    for(int i = 0; i < TYPE_COUNT; i++) {
        print_list(Objects[i]);
    }
}

void print_objects_by_type(ObjType_t Type) {
    Node_t* node = Objects[Type];
    printf("printing %s\n", enum_to_string(Type));
    print_list(node);
}

int main(void) {

    create_object("Mamadou", PERSON);
    create_object("206", CAR);
    create_object("Dembele", PERSON);
    create_object("Passat", CAR);
    print_objects();
    return 0;    
}
