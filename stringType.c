#include "stringType.h"

#include <string.h>
#include "darray.h"

String string_new(StringView view){
  String str = {0};
  darrayAppendMany(str, view.data, view.len);
  return str;
}

StringView string_newView(const char *txt, u32 len){
  StringView view = {
    .data = txt,
    .len = len
  };
  return view;
}

StringView string_view(String *str){
  StringView view = {
    .data = str->data,
    .len = str->len
  };
  return view;
}

void string_free(String *str){
  free(str->data);
}

void string_destroy(String *str){
  memset(str->data, 0, str->len);
  free(str->data);
  *str = (String){0};
}

void string_append(String *str, StringView view){
  darrayAppendMany(*str, view.data, view.len);
}

u32 string_findAmount(StringView view, char c){
  u32 amount = 0;
  for(u32 i = 0; i < view.len; ++i)
    if(view.data[i] == c) ++amount;
  return amount;
}

u32 *string_findAll(StringView view, char c, u32 *amount){
  u32 found = string_findAmount(view, c), cur = 0;
  u32 *list = malloc(found * sizeof *list);
  for(u32 i = 0; cur < found; ++i)
    if(view.data[i] == c) list[cur++] = i;
  *amount = found;
  return list;
}

void string_findDynamic(StringView view, char c, void *darray_u32){
  darrayTemplate(u32) *darray = darray_u32;
  u32 found = string_findAmount(view, c), cur = 0;
  darrayGrow(*darray, darray->size + found);
  for(u32 i = 0; cur < found; ++i)
    if(view.data[i] == c){
      darrayAppend(*darray, i);
      ++cur;
    }
}

u32 string_findFirst(StringView view, char c){
  char *sub = memchr(view.data, c, view.len);
  if(sub) return sub - view.data;
  return -1u;
}

u32 string_findLast(StringView view, char c){
  for(u32 i = view.len - 1; i < view.len; --i)
    if(view.data[i] == c) return i;
  return -1u;
}

u32 string_contains(StringView view, StringView sub){
  if(view.len < sub.len) return -1u;
  u32 cur = 0;
  while(view.len - cur >= sub.len){
    if(!memcmp(view.data + cur, sub.data, sub.len)) return cur;
    char *findings = memchr(view.data + cur + 1, sub.data[0], view.len - cur - sub.len);
    if(!findings) break;
    cur = findings - view.data;
  }
  return -1u;
}

void string_split(StringView view, StringView delimiter, void *darray_string){
  darrayTemplate(String) *darray = darray_string;
  u32 index = string_contains(view, delimiter);
  while(index + 1){
    String newEl = string_new(string_newView(view.data, index));
    darrayAppend(*darray, newEl);
    view.data += index + delimiter.len;
    view.len -= index + delimiter.len;
    index = string_contains(view, delimiter);
  }
  darrayAppend(*darray, string_new(view));
}

String string_join(String *list, u32 len, StringView join){
  asm("ud2");
}

String string_joinView(StringView *list, u32 len, StringView join){
  asm("ud2");
}

i32 string_compare(StringView base, StringView target){
  if(base.len != target.len) return (base.len > target.len) - (base.len < target.len);
  return memcmp(base.data, target.data, base.len);
}