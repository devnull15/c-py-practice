#include "../include/serialize.h"
#include "../tests/helper_functions.h"
#include <stdio.h>

#define HEADER_LEN 1
#define INT_LEN 8
#define STR_WINDOW_LENGTH_LEN 2
#define STR_LAST_WINDOW_LEN 1
#define STR_MAX_LEN 0xFFFF
#define LAST_ITEM_LEN 1
#define KEY_TYPE_LEN 1
#define VAL_TYPE_LEN 1
#define LONG_STR_LEN 0xFFFFF //from test gen_tests.c
#define ARR_DEFAULT_LEN 0xFFFF // = 65535 ; arbitrary
#define DICT_DEFAULT_LEN 0xFFFF // = 65535 ; arbitrary


//I know I shouldn't do this but I don't want to modify the header file
// cause it might break the tests harness. Or I'm lazy, take your pick.
#ifndef MY_HELPERS
#define MY_HELPERS
uint8_t _serialize_helper(int,proto_item_node_t*);
proto_item_node_t *_deserialize_helper(int,uint8_t);
proto_item_node_t *_readint(int,uint8_t);
proto_item_node_t *_readstr(int);
proto_item_node_t *_readarray(int);
proto_item_node_t *_readdict(int);
int _read_handler(int,void*,int);
int _writeint(int,proto_item_node_t*);
int _wriestr(int,proto_string_t*);
int _writearray(int,proto_array_t*);
int _writedict(int,proto_dict_t*);
int _write_handler(int,char*,int);
#endif


int _read_handler(int fd, void* buf, int readlen) {
  int ret = read(fd,buf,readlen);
  if(-1 == ret) {
    perror("error in _read_handler");
    goto RET;
  }

  //print_buffer_as_hex(buf, ret);
  
  if(readlen > ret) {
    fprintf(stderr, "!!! incomplete read: %d>%d -- Rereading...\n", readlen, ret);
    while(readlen > 0) {
      buf += ret;
      readlen -= ret;
      ret = read(fd,buf,readlen);
      if(-1 == ret) {
	perror("error in _read_handler");
	goto RET;
      }
    }
  }
  
 RET:
  return ret;
}

proto_item_node_t *_readdict(int fd) {

  uint8_t last_item = 0;
  uint8_t first = 1;
  
  
  // Make item
  proto_item_node_t *item = calloc(1,sizeof(proto_item_node_t));
  if(NULL == item) {
    goto ERR1;
  }
  item->value.dict_p = calloc(1,sizeof(proto_dict_t));
  if(NULL == item->value.dict_p) {
    goto ERR2;
  }
  proto_dict_t *dict = item->value.dict_p; //for convenience
  dict->items = calloc(DICT_DEFAULT_LEN,sizeof(proto_dict_item_t));
    if(NULL == dict->items) {
    goto ERR3;
  }

  proto_item_node_t *next_node = NULL;
  uint8_t key_header = 0;
  uint8_t val_header = 0;

  
  // Set header
  item->type = PROTO_DICT;
	
  while(!last_item) {    
    // read last item
    if(-1 == _read_handler(fd,(void*)&last_item,LAST_ITEM_LEN)) {
      goto ERR4;
    }
    
    //read key
    if(-1 == _read_handler(fd,(void*)&key_header,HEADER_LEN)) {
      goto ERR4;
    }
    if(first && key_header == PROTO_NULL) {
      free(dict->items);
      dict->items = NULL;
      break;
    }
    if(!first && key_header == PROTO_NULL) {
      fprintf(stderr, "!!! Can't have a PROTO_NULL after the first dictionary key.\n");
      goto RET_NULL;
    }
    
    //read value
    if(-1 == _read_handler(fd,(void*)&val_header,HEADER_LEN)) {
      goto ERR4;
    }
    if(val_header == PROTO_NULL) {
      fprintf(stderr, "!!! Can't have a PROTO_NULL in the dictionary value\n");
      goto RET_NULL;
    }

    //copy key
    next_node = _deserialize_helper(fd,key_header);
    memcpy(&dict->items[dict->size].key, next_node, sizeof(proto_item_node_t));
    free(next_node);
    
    //copy value
    next_node = _deserialize_helper(fd,val_header);
    memcpy(&dict->items[dict->size].value, next_node, sizeof(proto_item_node_t));
    free(next_node);
    
    first = 0;
    dict->size += 1;
  }

  return item;

 ERR4:
  free(dict->items);
  dict->items = NULL;
 ERR3:
  free(item->value.dict_p);
  item->value.dict_p = NULL;
 ERR2:
  free(item);
  item = NULL;
 ERR1:
  perror("error in _readdict");
 RET_NULL:
  return NULL;
}


proto_item_node_t *_readarray(int fd) {

  uint8_t last_item = 0;
  uint8_t first = 1;
  
  // Make item
  proto_item_node_t *item = calloc(1,sizeof(proto_item_node_t));
  if(NULL == item) {
    goto ERR1;
  }
  item->value.array_p = calloc(1,sizeof(proto_array_t));
  if(NULL == item->value.array_p) {
    goto ERR2;
  }
  proto_array_t *arr = item->value.array_p; //for convenience
  arr->nodes = calloc(ARR_DEFAULT_LEN,sizeof(proto_item_node_t));
  if(NULL == arr->nodes) {
    goto ERR3;
  }
  proto_item_node_t *next_node = NULL;


  // Set header
  item->type = PROTO_ARRAY;
	
  while(!last_item) {    
    // read last item
    if(-1 == _read_handler(fd,(void*)&last_item,LAST_ITEM_LEN)) {
      goto ERR4;
    }
    
    //read data type
    next_node = proto_deserialize(fd);
    if(first && next_node == NULL) {
      free(arr->nodes);
      arr->nodes = NULL;
    }
    if(!first && next_node == NULL) {
      fprintf(stderr, "!!! Can't have a PROTO_NULL after the first node of an array.\n");
      goto RET_NULL;
    }
    if(next_node != NULL) {
      memcpy(&arr->nodes[arr->size], next_node, sizeof(proto_item_node_t));
      free(next_node);
      arr->size += 1;
    }
    first = 0;
  }

  return item;

 ERR4:
  free(arr->nodes);
  arr->nodes = NULL;
 ERR3:
  free(item->value.array_p);
  item->value.array_p = NULL;
 ERR2:
  free(item);
  item = NULL;
 ERR1:
  perror("error in _readarray");
 RET_NULL:
  return NULL;
}


proto_item_node_t *_readstr(int fd) {

  char buf[STR_MAX_LEN];
  uint16_t window_size = 0;
  uint8_t last_window = 0;
  
  // Make item
  proto_item_node_t *item = calloc(1,sizeof(proto_item_node_t));
  if(NULL == item) {
    goto ERR1;
  }
  item->value.string_p = calloc(1,sizeof(proto_string_t));
  if(NULL == item->value.string_p) {
    goto ERR2;
  }
  proto_string_t *str = item->value.string_p; //for convenience
  str->string = calloc(1,LONG_STR_LEN);
  if(NULL == str->string) {
    goto ERR3;
  }
  char *str_ptr = str->string;


  // Set header
  item->type = PROTO_STRING;

	
  while(!last_window) {
    // read window len
    window_size = 0;
    if(-1 == _read_handler(fd,(void*)&window_size,STR_WINDOW_LENGTH_LEN)) {
      goto ERR4;
    }
    window_size = be16toh(window_size);
    str->length += window_size;
    
    // read last window
    if(-1 == _read_handler(fd,(void*)&last_window,STR_LAST_WINDOW_LEN)) {
      goto ERR4;
    }
    
    // read strs
    if(window_size > 0) {
      if(-1 == _read_handler(fd,buf,window_size)) {
	goto ERR4;
      }
      memcpy(str_ptr, buf, window_size);
      str_ptr += window_size;
    }
  }

  return item;

 ERR4:
  free(str->string);
  str->string = NULL;
 ERR3:
  free(item->value.string_p);
  item->value.string_p = NULL;
 ERR2:
  free(item);
  item = NULL;
 ERR1:
  perror("error in _readstr");
  return NULL;
}


proto_item_node_t *_readint(int fd, uint8_t header) {
  proto_item_node_t *item = calloc(1,sizeof(proto_item_node_t));
   if(NULL == item) {
     goto ERR1;
   }
  uint64_t buf = 0;

  if(header == PROTO_UINT64) {
    item->type = PROTO_UINT64;
    if(-1 == _read_handler(fd,(void*)&buf,sizeof(uint64_t))) {
      goto ERR2;
    }
    item->value.uint64 = be64toh(buf);
  }
  else if(header == PROTO_INT64) {
    item->type = PROTO_INT64;
    if(-1 ==_read_handler(fd,(void*)&buf,sizeof(int64_t))) {
      goto ERR2;
    }
    item->value.int64 = be64toh(buf);
  }
  else {
    fprintf(stderr, "!!! error in _readint\n");
    goto RET_NULL;
  }

  return item;

 ERR2:
  free(item);
  item = NULL;
 ERR1:
  perror("error in _readint");
 RET_NULL:
  return NULL;
}

proto_item_node_t * _deserialize_helper(int fd, uint8_t header) {

  proto_item_node_t *item = NULL;
  
  switch(header) {
  case PROTO_UINT64:
    item = _readint(fd,header);
    break;
  case PROTO_INT64:
    item = _readint(fd,header);
    break;
  case PROTO_STRING:
    item = _readstr(fd);
    break;
  case PROTO_ARRAY:
    item = _readarray(fd);
    break;
    case PROTO_DICT:
      item = _readdict(fd);
      break;
  case PROTO_NULL:
    //fprintf(stderr, "\n!!! Can't deserialize NULL.\n");
    return NULL;
  default:
    fprintf(stderr, "\n!!! Type Not Recognized.\n");
    return NULL;
  }
  
  return item;
}

proto_item_node_t * proto_deserialize(int fd) {
  if(fd < 0) {
    fprintf(stderr, "\n!!! Invalid file descriptor.\n");
    return NULL;
  }

  uint8_t header;
  if(-1 == _read_handler(fd, &header, HEADER_LEN)) {
    perror("read error in proto_deserialize");
    return NULL;
  }


  return _deserialize_helper(fd,header);
}

int _write_handler(int fd, char *buf, int writelen) {
  fprintf(stderr, "! writing stuff...\n");
  int ret = write(fd,buf,writelen);
  if(-1 == ret) {
    perror("error in _write_handler");
    goto RET;
  }
  
  if(writelen > ret) {
    fprintf(stderr, "!!! incomplete write: %d>%d\n", writelen, ret);
  }
  
 RET:  
  return ret;
} 



int _writeint(int fd, proto_item_node_t *item) {

  char buf[INT_LEN];
  uint64_t num = htobe64(item->value.uint64);
  int writelen = 0;
  memcpy(buf, &num, INT_LEN);
  //print_buffer_as_hex((char*)(buf), HEADER_LEN+INT_LEN);

  writelen = INT_LEN;
  if(writelen > _write_handler(fd,buf,writelen)) {
    return -1;
  }

  return writelen;
}

int _writestr(int fd, proto_string_t *str) {
  if(str == NULL || str->string == NULL) {
    return -1;
  }
  
  uint64_t maxbufsize = STR_WINDOW_LENGTH_LEN + STR_LAST_WINDOW_LEN + STR_MAX_LEN;
  char buf[maxbufsize];
  
  uint64_t ret = 0;
  uint64_t strlen = str->length;
  uint16_t window_size = htobe16(strlen&STR_MAX_LEN);
  char *strptr = str->string;
  int last_window = 0;
  int writelen = 0;
  if(strlen > STR_MAX_LEN) { last_window = 0;  }
  else { last_window = 1; }
  
  //First write
  memset(buf+STR_WINDOW_LENGTH_LEN, last_window, STR_LAST_WINDOW_LEN);    
  if(last_window) {
    memcpy(buf, &window_size, STR_WINDOW_LENGTH_LEN);
    memcpy(buf+STR_WINDOW_LENGTH_LEN+STR_LAST_WINDOW_LEN, strptr, strlen);
    writelen = maxbufsize-(STR_MAX_LEN-strlen);
  }
  else {
    memset(buf, STR_MAX_LEN, STR_WINDOW_LENGTH_LEN);
    memcpy(buf+STR_WINDOW_LENGTH_LEN+STR_LAST_WINDOW_LEN, strptr, STR_MAX_LEN);
    writelen = maxbufsize;
  }

  if(writelen > _write_handler(fd,buf,writelen)) {
    return -1;
  }
  ret += writelen;


  //optional repeat writes
  while(!last_window) {
    memset(buf, 0, maxbufsize);
    strptr += STR_MAX_LEN;
    strlen -= STR_MAX_LEN;
    window_size = htobe16(strlen&STR_MAX_LEN);
    if(strlen > STR_MAX_LEN) { last_window = 0; }
    else { last_window = 1; }
    memset(buf+STR_WINDOW_LENGTH_LEN, last_window, STR_LAST_WINDOW_LEN);
    if(last_window) {
      memcpy(buf, &window_size, STR_WINDOW_LENGTH_LEN);
      memcpy(buf+STR_WINDOW_LENGTH_LEN+STR_LAST_WINDOW_LEN, strptr, strlen);
      writelen = maxbufsize-(STR_MAX_LEN-strlen);
    }
    else {
      memset(buf, STR_MAX_LEN, STR_WINDOW_LENGTH_LEN);
      memcpy(buf+STR_WINDOW_LENGTH_LEN+STR_LAST_WINDOW_LEN, strptr, STR_MAX_LEN);
      writelen = maxbufsize;
    }
    //print_buffer_as_hex(buf,writelen);

    if(writelen > _write_handler(fd,buf,writelen)) {
      return -1;
    }

    ret += writelen;
  }

  return ret;
}

int _writearray(int fd, proto_array_t *arr) {
  if(arr == NULL || (arr->size > 0 && arr->nodes == NULL)) {
    return -1;
  }
  int writelen = 0;
  uint64_t maxbufsize = LAST_ITEM_LEN + HEADER_LEN;
  char buf[maxbufsize];
  int lastitem = 0;
  int ret = 0;
  proto_item_node_t *cur_item = NULL;
  
  if(arr->nodes == NULL || arr->nodes->type == PROTO_NULL) {
    lastitem = 1;
    memset(buf, lastitem, LAST_ITEM_LEN);
    memset(buf+LAST_ITEM_LEN, PROTO_NULL, HEADER_LEN);
    writelen = maxbufsize;
    if(writelen > _write_handler(fd,buf,writelen)) {
      return -1;
    }
    ret += writelen;

  }
  else {
    for(uint64_t i = 0; i < arr->size; i++) {
      cur_item = &arr->nodes[i];
      if(cur_item == NULL) { return -1; }
      if(cur_item->type == PROTO_ARRAY && cur_item->value.array_p == NULL) { return -1; }
      memset(buf, 0, maxbufsize);
      if(i == arr->size-1) { lastitem = 1; }
      else { lastitem = 0; }
      memset(buf, lastitem, LAST_ITEM_LEN);
      writelen = LAST_ITEM_LEN;
      if(writelen > _write_handler(fd,buf,writelen)) {
	return -1;
      }
      ret += writelen;
      proto_serialize(fd,cur_item); // doesn't call helper so that proto can write the header
    }
  }

  return ret;
}
int _writedict(int fd, proto_dict_t *dict) {
  if(dict == NULL || (dict->size > 0 && dict->items == NULL)) {
    return -1;
  }
  int writelen = 0;
  uint64_t maxbufsize = LAST_ITEM_LEN + KEY_TYPE_LEN + VAL_TYPE_LEN;
  char buf[maxbufsize];
  int lastitem = 0;
  int ret = 0;
  proto_dict_item_t *cur_item = NULL;
  

  if(dict->items == NULL || dict->items->key.type == PROTO_NULL) {
    lastitem = 1;
    memset(buf, lastitem, LAST_ITEM_LEN);
    memset(buf+LAST_ITEM_LEN, PROTO_NULL, KEY_TYPE_LEN);
    memset(buf+LAST_ITEM_LEN+KEY_TYPE_LEN, PROTO_NULL, VAL_TYPE_LEN);
    writelen = maxbufsize;
    if(writelen > _write_handler(fd,buf,writelen)) {
      return -1;
    }
    ret += writelen;
  }
  else {
    for(uint64_t i = 0; i < dict->size; i++) {
      memset(buf, 0, maxbufsize);
      cur_item = &dict->items[i];
      if(i != 0) {
	if(&cur_item->key == NULL) { return -1; }
      }
      if(&cur_item->value == NULL) { return -1; }
      if(i == dict->size-1) { lastitem = 1; }
      else { lastitem = 0; }
      memset(buf, lastitem, LAST_ITEM_LEN);
      memset(buf+LAST_ITEM_LEN, cur_item->key.type, KEY_TYPE_LEN);
      memset(buf+LAST_ITEM_LEN+KEY_TYPE_LEN, cur_item->value.type, VAL_TYPE_LEN);
      writelen = maxbufsize;
      if(writelen > _write_handler(fd,buf,writelen)) {
	return -1;
      }
      ret += writelen;
      _serialize_helper(fd,&cur_item->key);
      _serialize_helper(fd,&cur_item->value);
    }
  }


  return ret;
}


uint8_t _serialize_helper(int fd, proto_item_node_t *item) {
  
  switch(item->type) {
  case PROTO_UINT64:
    
    _writeint(fd,item);
    break;
   case PROTO_INT64:
    _writeint(fd,item);
    break;
  case PROTO_STRING:
    if(_writestr(fd,item->value.string_p) == -1) {
      fprintf(stderr, "\n!!! Can't serialize NULL string.\n");
      return 1;
    }
    break;
  case PROTO_ARRAY:
    if(_writearray(fd,item->value.array_p) == -1) {
      fprintf(stderr, "\n!!! Can't serialize NULL array.\n");
      return 1;
    }
    break;
  case PROTO_DICT:
    if(_writedict(fd,item->value.dict_p) == -1) {
      fprintf(stderr, "\n!!! Can't serialize NULL dict.\n");
      return 1;
    }
    break;
  case PROTO_NULL:
    fprintf(stderr, "\n!!! Can't serialize NULL.\n");
    return 1;
  default:
    fprintf(stderr, "\n!!! Type Not Recognized.\n");
    return 1;
  }

  return 0;
}



uint8_t proto_serialize(int fd, proto_item_node_t *item) {
  if(item == NULL) {
    fprintf(stderr, "\n!!! Can't serialize NULL.\n");
    return 1;
  }
  if(fd < 0) {
    fprintf(stderr, "\n!!! Invalid file descriptor.\n");
    return 1;
  }

  char buf[HEADER_LEN];
  memset(buf, item->type, HEADER_LEN);
  if(-1 == write(fd,buf,HEADER_LEN)) {
    perror("error in proto_serialize");
    goto RET;
  }

  uint8_t ret = _serialize_helper(fd,item);

 RET:
  return ret;
}
