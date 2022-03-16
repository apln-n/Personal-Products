#if defined(_WIN64) || defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "GLUT_Model.h"
#include "Boolean.h"

#include <stdio.h>
#include <math.h> // for sqrt
#include <string.h> // for strrchr, strcmp
#include <assert.h> // for assert
#include <float.h> // for FLT_MAX

#if defined(_WIN64)
#define atoll(S) _atoi64(S)
#include <windows.h>
#elif defined(_WIN32)
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <unistd.h>
#endif

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include "libraries/tinyobj_loader_c.h"

#include "GL/freeglut.h"
#if defined(_WIN64) || defined(_WIN32)
#include "GL/glext.h"
#endif

//#include <cimport.h>
//#include <scene.h>
//#include <postprocess.h>
//#include "assimp.h"
//#include "aiPostProcess.h"
//#include "aiScene.h"

//const struct aiScene* scene = NULL;
//GLuint scene_list = 0;
//struct aiVector3D scene_min, scene_max, scene_center;

//static float angle = 0.;
#define MAX 10

typedef struct {
  GLuint vb;
  int numTriangles;
  float bbmin[3], bbmax[3];
} DrawObject;
//static DrawObject gDrawObject;
//float bbmin[3], bbmax[3];

static bool fill_list[MAX] = {0};
static DrawObject obj_list[MAX];

static unsigned int getEmptyID() {
  unsigned int i;
  for (i=0; i<MAX; ++i) {
    if(fill_list[i]==false) {
      fill_list[i]=true;
      return i+1;
    }
  }
  return 0;
}
static void delID(const unsigned int id) {
  if(1 > id || id > MAX) return;
  fill_list[id-1] = false;
}
static DrawObject *getDrawObject(const unsigned int id) {
  if(1 > id || id > MAX) return NULL;
  return &obj_list[id-1];
}

static void CheckErrors(const char* desc) {
  GLenum e = glGetError();
  if (e != GL_NO_ERROR) {
    fprintf(stderr, "OpenGL error in \"%s\": %d (%d)\n", desc, e, e);
    exit(20);
  }
}

static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]) {
  float v10[3];
  float v20[3];
  float len2;

  v10[0] = v1[0]-v0[0];
  v10[1] = v1[1]-v0[1];
  v10[2] = v1[2]-v0[2];

  v20[0] = v2[0]-v0[0];
  v20[1] = v2[1]-v0[1];
  v20[2] = v2[2]-v0[2];

  N[0] = v20[1]*v10[2] - v20[2]*v10[1];
  N[1] = v20[2]*v10[0] - v20[0]*v10[2];
  N[2] = v20[0]*v10[1] - v20[1]*v10[0];

  len2 = N[0]*N[0] + N[1]*N[1] + N[2]*N[2];
  if(len2 > 0.0f) {
    float len = (float)sqrt((double)len2);
    N[0]/=len;
    N[1]/=len;
  }
}

static const char* mmap_file(size_t* len, const char* filename) {
#if defined(_WIN64) || defined(_WIN32)
  HANDLE file;
  HANDLE fileMapping;
  LPVOID fileMapView;
  const char *fileMapViewChar;
  
  file =
    CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  assert(file != INVALID_HANDLE_VALUE);

  fileMapping = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);
  assert(fileMapping != INVALID_HANDLE_VALUE);

  fileMapView = MapViewOfFile(fileMapping, FILE_MAP_READ, 0, 0, 0);
  fileMapViewChar = (const char*)fileMapView;
  assert(fileMapView != NULL);

  *len = strlen(fileMapViewChar);
  return fileMapViewChar;
#else

  FILE* f;
  long file_size;
  struct stat sb;
  char* p;
  int fd;

  (*len) = 0;

  f = fopen(filename, "r");
  fseek(f, 0, SEEK_END);
  file_size = ftell(f);
  fclose(f);

  fd = open(filename, O_RDONLY);
  if (fd == -1) {
    perror("open");
    return NULL;
  }

  if (fstat(fd, &sb) == -1) {
    perror("fstat");
    return NULL;
  }

  if (!S_ISREG(sb.st_mode)) {
    fprintf(stderr, "%s is not a file\n", "lineitem.tbl");
    return NULL;
}

p = (char*)mmap(0, (size_t)file_size, PROT_READ, MAP_SHARED, fd, 0);

if (p == MAP_FAILED) {
  perror("mmap");
  return NULL;
 }

if (close(fd) == -1) {
  perror("close");
  return NULL;
 }

(*len) = (size_t)file_size;

return p;

#endif
}

static const char* get_file_data(size_t* len, const char* filename) {
  const char* ext = strrchr(filename, '.');

  size_t data_len = 0;
  const char* data = NULL;

  if (strcmp(ext, ".gz") == 0) {
    assert(0); /* todo */

  } else {
    data = mmap_file(&data_len, filename);
  }

  (*len) = data_len;
  return data;
}

//static int LoadObjAndConvert(unsigned int *id,
//			     float bmin[3], float bmax[3],
//			     const char* filename) {
static int LoadObjAndConvert(unsigned int *id,
			     const char* filename) {
  DrawObject *obj;
  size_t data_len = 0;
  const char *data = NULL;
  
  tinyobj_attrib_t attrib;
  tinyobj_shape_t* shapes = NULL;
  size_t num_shapes;
  tinyobj_material_t* materials = NULL;
  size_t num_materials;

  *id = getEmptyID();
  if(*id == 0) {
    fprintf(stderr,"obj_list is full: you should increase the maximam size of obj_list (MAX)\n");
    exit(-1);
  }
  //  DrawObject *obj = getDrawObject(*id);
  obj = getDrawObject(*id);

  //  size_t data_len = 0;
  //  const char* data = get_file_data(&data_len, filename);
  data = get_file_data(&data_len, filename);
  if (data == NULL) {
    exit(-1);
    /* return 0; */
  }
  printf("filesize: %d\n", (int)data_len);

  {
    unsigned int flags = TINYOBJ_FLAG_TRIANGULATE;
    int ret = tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials,
				&num_materials, data, data_len, flags);
    if (ret != TINYOBJ_SUCCESS) {
      return 0;
    }

    printf("# of shapes    = %d\n", (int)num_shapes);
    printf("# of materiasl = %d\n", (int)num_materials);

    /*
    {
      int i;
      for (i = 0; i < num_shapes; i++) {
        printf("shape[%d] name = %s\n", i, shapes[i].name);
      }
    }
    */
  }

  //  (obj->bbmin[0]) = (obj->bbmin[1]) = (obj->bbmin[2]) = FLT_MAX;
  //  (obj->bbmax[0]) = (obj->bbmax[1]) = (obj->bbmax[2]) = -FLT_MAX;

  {
    DrawObject o;
    float* vb;
    /* std::vector<float> vb; //  */
    size_t face_offset = 0;
    size_t i;

    /* Assume triangulated face. */
    size_t num_triangles = attrib.num_face_num_verts;
    size_t stride = 9; /* 9 = pos(3float), normal(3float), color(3float) */

    o.bbmin[0] = o.bbmin[1] = o.bbmin[2] = FLT_MAX;
    o.bbmax[0] = o.bbmax[1] = o.bbmax[2] = -FLT_MAX;
    vb = (float*)malloc(sizeof(float) * stride * num_triangles * 3);

    for (i = 0; i < attrib.num_face_num_verts; i++) {
      size_t f;
      assert(attrib.face_num_verts[i] % 3 ==
	     0); /* assume all triangle faces. */
      for (f = 0; f < (size_t)attrib.face_num_verts[i] / 3; f++) {
	size_t k;
	float v[3][3];
	float n[3][3];
	float c[3];
	float len2;

	tinyobj_vertex_index_t idx0 = attrib.faces[face_offset + 3 * f + 0];
	tinyobj_vertex_index_t idx1 = attrib.faces[face_offset + 3 * f + 1];
	tinyobj_vertex_index_t idx2 = attrib.faces[face_offset + 3 * f + 2];

	for (k = 0; k < 3; k++) {
	  int f0 = idx0.v_idx;
	  int f1 = idx1.v_idx;
	  int f2 = idx2.v_idx;
	  assert(f0 >= 0);
	  assert(f1 >= 0);
	  assert(f2 >= 0);

	  v[0][k] = attrib.vertices[3 * (size_t)f0 + k];
	  v[1][k] = attrib.vertices[3 * (size_t)f1 + k];
	  v[2][k] = attrib.vertices[3 * (size_t)f2 + k];
	  o.bbmin[k] = (v[0][k] < o.bbmin[k]) ? v[0][k] : o.bbmin[k];
	  o.bbmin[k] = (v[1][k] < o.bbmin[k]) ? v[1][k] : o.bbmin[k];
	  o.bbmin[k] = (v[2][k] < o.bbmin[k]) ? v[2][k] : o.bbmin[k];
	  o.bbmax[k] = (v[0][k] > o.bbmax[k]) ? v[0][k] : o.bbmax[k];
	  o.bbmax[k] = (v[1][k] > o.bbmax[k]) ? v[1][k] : o.bbmax[k];
	  o.bbmax[k] = (v[2][k] > o.bbmax[k]) ? v[2][k] : o.bbmax[k];
	}

	if (attrib.num_normals > 0) {
	  int f0 = idx0.vn_idx;
	  int f1 = idx1.vn_idx;
	  int f2 = idx2.vn_idx;
	  if (f0 >= 0 && f1 >= 0 && f2 >= 0) {
	    assert(f0 < (int)attrib.num_normals);
	    assert(f1 < (int)attrib.num_normals);
	    assert(f2 < (int)attrib.num_normals);
	    for (k = 0; k < 3; k++) {
	      n[0][k] = attrib.normals[3 * (size_t)f0 + k];
	      n[1][k] = attrib.normals[3 * (size_t)f1 + k];
	      n[2][k] = attrib.normals[3 * (size_t)f2 + k];
	    }
	  } else { /* normal index is not defined for this face */
	    /* compute geometric normal */
	    CalcNormal(n[0], v[0], v[1], v[2]);
	    n[1][0] = n[0][0];
	    n[1][1] = n[0][1];
	    n[1][2] = n[0][2];
	    n[2][0] = n[0][0];
	    n[2][1] = n[0][1];
	    n[2][2] = n[0][2];
	  }
	} else {
	  /* compute geometric normal */
	  CalcNormal(n[0], v[0], v[1], v[2]);
	  n[1][0] = n[0][0];
	  n[1][1] = n[0][1];
	  n[1][2] = n[0][2];
	  n[2][0] = n[0][0];
	  n[2][1] = n[0][1];
	  n[2][2] = n[0][2];
	}

	for (k = 0; k < 3; k++) {
	  vb[(3 * i + k) * stride + 0] = v[k][0];
	  vb[(3 * i + k) * stride + 1] = v[k][1];
	  vb[(3 * i + k) * stride + 2] = v[k][2];
	  vb[(3 * i + k) * stride + 3] = n[k][0];
	  vb[(3 * i + k) * stride + 4] = n[k][1];
	  vb[(3 * i + k) * stride + 5] = n[k][2];

	  /* Use normal as color. */
	  c[0] = n[k][0];
	  c[1] = n[k][1];
	  c[2] = n[k][2];
	  len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
	  if (len2 > 0.0f) {
	    float len = (float)sqrt((double)len2);

	    c[0] /= len;
	    c[1] /= len;
	    c[2] /= len;
	  }

	  vb[(3 * i + k) * stride + 6] = (c[0] * 0.5f + 0.5f);
	  vb[(3 * i + k) * stride + 7] = (c[1] * 0.5f + 0.5f);
	  vb[(3 * i + k) * stride + 8] = (c[2] * 0.5f + 0.5f);
	}
      }
      face_offset += (size_t)attrib.face_num_verts[i];
    }

    o.vb = 0;
    o.numTriangles = 0;
    if (num_triangles > 0) {
      glGenBuffers(1, &o.vb);
      glBindBuffer(GL_ARRAY_BUFFER, o.vb);
      glBufferData(GL_ARRAY_BUFFER, num_triangles * 3 * stride * sizeof(float),
		   vb, GL_STATIC_DRAW);
      o.numTriangles = (int)num_triangles;
    }

    free(vb);

    //    gDrawObject = o;
    *obj = o;
  }

  printf("bmin = %f, %f, %f\n", (double)obj->bbmin[0], (double)obj->bbmin[1],
	 (double)obj->bbmin[2]);
  printf("bmax = %f, %f, %f\n", (double)obj->bbmax[0], (double)obj->bbmax[1],
	 (double)obj->bbmax[2]);

  tinyobj_attrib_free(&attrib);
  tinyobj_shapes_free(shapes, num_shapes);
  tinyobj_materials_free(materials, num_materials);

  return 1;
}


unsigned int LoadModel(const char *name) {
  unsigned int id;
  //  float bmin[3], bmax[3];
  //  if(0 == LoadObjAndConvert(bmin, bmax, name)) {
  //  if(0 == LoadObjAndConvert(&id, bbmin, bbmax, name)) {
  if(0 == LoadObjAndConvert(&id, name)) {
    fprintf(stderr, "failed to load & conv\n");
    exit(1);
  }
  //  id.iid=0;
  return id;
}
//void DrawModel(const ModelID id, const float scale) {
void DrawModel(const unsigned int id, const float scale) {
  //  const DrawObject* draw_object = &gDrawObject;
  const DrawObject *obj = getDrawObject(id);
  const float max = ((obj->bbmax[0]-obj->bbmin[0])>(obj->bbmax[2]-obj->bbmin[2]))?((obj->bbmax[0]-obj->bbmin[0])>(obj->bbmax[1]-obj->bbmin[1]))?(obj->bbmax[0]-obj->bbmin[0]):(obj->bbmax[1]-obj->bbmin[1]):(obj->bbmax[2]-obj->bbmin[2]);

  if(scale>0) {
  glPushMatrix();
  //  glScalef(1./max*scale,1./max*scale,1./max*scale);
  glScalef((GLfloat)(scale/max),(GLfloat)(scale/max),(GLfloat)(scale/max));
  glTranslatef((GLfloat)(-(obj->bbmax[0]+obj->bbmin[0])*0.5),
	       (GLfloat)(-(obj->bbmax[1]+obj->bbmin[1])*0.5),
	       (GLfloat)(-(obj->bbmax[2]+obj->bbmin[2])*0.5));
  }

  glPolygonMode(GL_FRONT, GL_FILL);
  //  glPolygonMode(GL_BACK, GL_FILL);
  
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.0, 1.0);
  glColor3f(1.0f, 1.0f, 1.0f);
  //  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  //  if (draw_object->vb >= 1) {
  if (obj->vb >= 1) {
    //    glBindBuffer(GL_ARRAY_BUFFER, draw_object->vb);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vb);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 36, (const void*)0);
    glNormalPointer(GL_FLOAT, 36, (const void*)(sizeof(float) * 3));
    glColorPointer(3, GL_FLOAT, 36, (const void*)(sizeof(float) * 6));

    //    glDrawArrays(GL_TRIANGLES, 0, 3 * draw_object->numTriangles);
    glDrawArrays(GL_TRIANGLES, 0, 3 * obj->numTriangles);
    CheckErrors("drawarrays");
  }

  /* draw wireframe */
  /*
  glDisable(GL_POLYGON_OFFSET_FILL);
  glPolygonMode(GL_FRONT, GL_LINE);
  glPolygonMode(GL_BACK, GL_LINE);

  glColor3f(0.0f, 0.0f, 0.4f);

//  if (draw_object->vb >= 1) {
  if (obj->vb >= 1) {
//    glBindBuffer(GL_ARRAY_BUFFER, draw_object->vb);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vb);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 36, (const void*)0);
    glNormalPointer(GL_FLOAT, 36, (const void*)(sizeof(float) * 3));

//    glDrawArrays(GL_TRIANGLES, 0, 3 * draw_object->numTriangles);
    glDrawArrays(GL_TRIANGLES, 0, 3 * obj->numTriangles);
    CheckErrors("drawarrays");
  }
  */
  if(scale>0) glPopMatrix();

  /*
  float xmin=0,xmax=0;
  float ymin=0,ymax=0;
  float zmin=0,zmax=0;
  float xrange=0,yrange=0,zrange=0;
  float max=0.;
  unsigned int i,j,k;
  unsigned int index;
  const struct aiScene *sl=scene_list[id.iid];
  struct aiMesh *pmesh=NULL;
  struct aiVector3D *pvec=NULL;
  struct aiFace *pface=NULL;
  */
  /*** error check ***/
  /*
  if(!sl) {
    fprintf(stderr,"ModelID is NULL\n");
    return;
  }
  if(sl->mMeshes==NULL) {
    fprintf(stderr,"Mesh Pointer of ASSIMP is NULL\n");
    exit(1);
  }
  if(scale>0) {
  */
  /*** set min, max [x,y,z] ***/
  /*
  for(i=0; i<sl->mNumMeshes; ++i) {
    pmesh=sl->mMeshes[i];
    if(pmesh->mVertices != NULL) {
      pvec = pmesh->mVertices+0;
      xmin=pvec->x;ymin=pvec->y;zmin=pvec->z;
      xmax=pvec->x;ymax=pvec->y;zmax=pvec->z;
    }
  }
  */
  /*** set range [x,y,z] ***/
  /*
  for(i=0; i<sl->mNumMeshes; ++i) {
    pmesh=sl->mMeshes[i];
    for(j=0; j<pmesh->mNumVertices; ++j){
      pvec = pmesh->mVertices+j;
      if(xmin>pvec->x) xmin=pvec->x;
      if(ymin>pvec->y) ymin=pvec->y;
      if(zmin>pvec->z) zmin=pvec->z;
      if(xmax<pvec->x) xmax=pvec->x;
      if(ymax<pvec->y) ymax=pvec->y;
      if(zmax<pvec->z) zmax=pvec->z;
    }
    xrange=xmax-xmin;
    yrange=ymax-ymin;
    zrange=zmax-zmin;
    max = (xrange>yrange) ?
      ( (xrange>zrange)?(xrange):(zrange) ) :
      ( (yrange>zrange)?(yrange):(zrange) );
  }
  }
  */
  /*** draw model with normalizing ***/
  /*
  if(scale>0) {
  glPushMatrix();
  glScalef(1./max*scale,1./max*scale,1./max*scale);
  glTranslatef(-(xmax+xmin)*0.5,
	       -(ymax+ymin)*0.5,
	       -(zmax+zmin)*0.5);
  }
  for(i=0; i<sl->mNumMeshes; ++i) {
    pmesh=sl->mMeshes[i];
    for(j=0; j<pmesh->mNumFaces; ++j) {
      glBegin(GL_POLYGON);
      pface=pmesh->mFaces+j;
      for(k=0; k<pface->mNumIndices; ++k) {
	index = pface->mIndices[k];
	pvec=pmesh->mVertices+(index);
	glVertex3f(pvec->x, pvec->y, pvec->z);
      }
      glEnd();
    }
  }
  if(scale>0) glPopMatrix();
  */
}
void DrawWireModel(const unsigned int id, const float scale) {
  const DrawObject *obj = getDrawObject(id);
  const float max = ((obj->bbmax[0]-obj->bbmin[0])>(obj->bbmax[2]-obj->bbmin[2]))?((obj->bbmax[0]-obj->bbmin[0])>(obj->bbmax[1]-obj->bbmin[1]))?(obj->bbmax[0]-obj->bbmin[0]):(obj->bbmax[1]-obj->bbmin[1]):(obj->bbmax[2]-obj->bbmin[2]);

  if(scale>0) {
  glPushMatrix();
  //  glScalef(1./max*scale,1./max*scale,1./max*scale);
  glScalef((GLfloat)(scale/max),(GLfloat)(scale/max),(GLfloat)(scale/max));
  glTranslatef((GLfloat)(-(obj->bbmax[0]+obj->bbmin[0])*0.5),
	       (GLfloat)(-(obj->bbmax[1]+obj->bbmin[1])*0.5),
	       (GLfloat)(-(obj->bbmax[2]+obj->bbmin[2])*0.5));
  }

  glPolygonMode(GL_FRONT, GL_FILL);
  glDisable(GL_POLYGON_OFFSET_FILL);
  //  glPolygonMode(GL_FRONT, GL_LINE);
  //  glPolygonMode(GL_BACK, GL_LINE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //  glColor3f(0.0f, 0.0f, 0.4f);

//  if (draw_object->vb >= 1) {
  if (obj->vb >= 1) {
//    glBindBuffer(GL_ARRAY_BUFFER, draw_object->vb);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vb);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 36, (const void*)0);
    glNormalPointer(GL_FLOAT, 36, (const void*)(sizeof(float) * 3));

//    glDrawArrays(GL_TRIANGLES, 0, 3 * draw_object->numTriangles);
    glDrawArrays(GL_TRIANGLES, 0, 3 * obj->numTriangles);
    CheckErrors("drawarrays");
  }
  if(scale>0) glPopMatrix();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  /*
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  DrawModel(id,scale);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  */
}
void UnloadModel(const unsigned int id) {
  DrawObject *obj = getDrawObject(id);
  //  glDeleteBuffers(1, &gDrawObject.vb);
  glDeleteBuffers(1, &obj->vb);
  delID(id);
  /*
  aiReleaseImport(scene_list[0]);
  fill_list[id.iid] = false;
  */
}


