#pragma once

/*** identifier for inported model ***/
typedef struct {
  /*** internal variable ***/
  /*** don't touch directly this. ***/
  long int iid;
} ModelID;

//ModelID LoadModel(const char *name);
unsigned int  LoadModel(const char *name);
//void UnloadModel(const ModelID id);
void UnloadModel(const unsigned int id);
//void DrawModel(const ModelID id, const float scale);
void DrawModel(const unsigned int id, const float scale);
//void DrawWireModel(const ModelID id, const float scale);
void DrawWireModel(const unsigned int id, const float scale);
