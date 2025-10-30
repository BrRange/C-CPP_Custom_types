#include <corecrt.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define QUADTREE_MAXPOINTS 4

struct QuadPoint{
  size_t index;
  float x, y;
};
typedef struct QuadPoint QuadPoint;

struct QuadTree{
  struct QuadTree *child;
  union{
    float bound[4];
    struct{
      float x, y, w, h;
    };
  };
  QuadPoint point[QUADTREE_MAXPOINTS];
  uint8_t used;
  bool branch;
};
typedef struct QuadTree QuadTree;

QuadTree QuadTree_newRoot(float x, float y, float w, float h){
  QuadTree root = {
    .bound = {x, y, w, h}
  };
  return root;
}

QuadTree *QuadTree_newDivision(float bound[4]){
  QuadTree *pack = malloc(sizeof(QuadTree) * 4ull);
  if(!pack){
    puts("No RAM lol");
    return NULL;
  }
  pack[0] = (QuadTree){
    .bound = {bound[0], bound[1], bound[2] / 2.f, bound[3] / 2.f},
  };
  pack[1] = (QuadTree){
    .bound = {bound[0] + bound[2] / 2.f, bound[1], bound[2], bound[3] / 2.f},
  };
  pack[2] = (QuadTree){
    .bound = {bound[0] + bound[2] / 2.f, bound[1] + bound[3] / 2.f, bound[2], bound[3]},
  };
  pack[3] = (QuadTree){
    .bound = {bound[0], bound[1] + bound[3] / 2.f, bound[2] / 2.f, bound[3]},
  };
  return pack;
}

bool QuadTree_inBound(QuadTree *tree, float x, float y){
  return (tree->x <= x && x <= tree->x + tree->w) && (tree->y <= y && y <= tree->y + tree->h);
}

void QuadTree_insert(QuadTree *tree, QuadPoint point){
  if(tree->used < QUADTREE_MAXPOINTS){
    tree->point[tree->used] = point;
    ++tree->used;
    return;
  }
  if(!tree->branch){
    tree->branch = true;
    tree->child = QuadTree_newDivision(tree->bound);
  }
  for(int i = 0; i < 4; ++i)
  if(QuadTree_inBound(tree->child + i, point.x, point.y)){
    QuadTree_insert(tree->child + i, point);
    return;
  }
}

void QuadTree_free(QuadTree *tree){
  if(tree->branch){
    for(int i = 0; i < 4; ++i)
      QuadTree_free(tree->child + i);
    free(tree->child);
  }
}