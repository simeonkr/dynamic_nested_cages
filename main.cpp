#include "extrap_cage.h"
#include <Eigen/Core>
#include <igl/read_triangle_mesh.h>
#include <igl/opengl/glfw/Viewer.h>

using namespace Eigen;
using namespace igl;

bool viewer_empty = true;


void add_to_viewer(igl::opengl::glfw::Viewer &viewer, MatrixXd V, MatrixXi F) 
{
  if (viewer_empty) viewer_empty = false;
  else viewer.append_mesh();
  viewer.data().set_mesh(V, F);
  viewer.data().set_face_based(true);
}


void view_all_meshes(igl::opengl::glfw::Viewer &viewer) 
{
  // based on tutorial code (107)
  int k = viewer.data_list.size();
  std::map<int, Eigen::RowVector3d> colors;
  for (int i = 0; i < k; i++)
    colors.emplace(i, 0.5*Eigen::RowVector3d::Random().array() + 0.5);
  int last_selected = -1;
  viewer.callback_key_down =
    [&](igl::opengl::glfw::Viewer &, unsigned int key, int mod)
  {
    if(key == GLFW_KEY_UP)
    {
      viewer.selected_data_index = (viewer.selected_data_index + 1) % k;
      return true;
    }
    else if(key == GLFW_KEY_DOWN)
    {
      viewer.selected_data_index = (k + viewer.selected_data_index - 1) % k;
      return true;
    }
    return false;
  };
  viewer.callback_pre_draw =
    [&](igl::opengl::glfw::Viewer &)
  {
    if (last_selected != viewer.selected_data_index)
    {
      for (auto &data : viewer.data_list)
      {
        data.set_colors(colors[data.id]);
      }
      viewer.data_list[viewer.selected_data_index].set_colors(Eigen::RowVector3d(1., 1., 1.));
      last_selected = viewer.selected_data_index;
    }
    return false;
  };
  viewer.launch();
}


// ./gen_cage <method> <mode> <mesh_basename> <initial_cage> <frame_num> [reference_cage]
int main(int argc, char *argv[])
{
  ExtrapMethod method;
  if (strcmp(argv[1], "s") == 0) method = SIMPLE;
  if (strcmp(argv[1], "sa") == 0) method = SIMPLE_ALIGNED;
  if (strcmp(argv[1], "se") == 0) method = SIMPLE_EXPAND;
  if (strcmp(argv[1], "mvc") == 0) method = MVC;
  enum Mode {
    DIRECT = 0,
    INCREMENTAL = 1
  } mode;
  if (strcmp(argv[2], "d") == 0) mode = DIRECT;
  if (strcmp(argv[2], "i") == 0) mode = INCREMENTAL;
  char *basename = argv[3];
  char *initial_cage = argv[4];
  int frame_num = atoi(argv[5]);
  char *ref_cage = NULL;
  if (argc > 6) ref_cage = argv[6];
  
  MatrixXd V1, V2;
  MatrixXi F;
  MatrixXd V1_C, V2_C;
  MatrixXi F_C;
  read_triangle_mesh(initial_cage, V1_C, F_C);
  
  char *finemesh1_name;
  char *finemesh2_name;
  if (mode == DIRECT || frame_num == 0) {
    asprintf(&finemesh1_name, "%s_%04d.obj", basename, 0);
    asprintf(&finemesh2_name, "%s_%04d.obj", basename, frame_num);
    read_triangle_mesh(finemesh1_name, V1, F);
    read_triangle_mesh(finemesh2_name, V2, F);
    // use initial cage as basis for new cage
    extrap_cage(V1, V2, F, V1_C, F_C, method, V2_C);
  }
  else if (mode == INCREMENTAL) {
    V2_C = V1_C;
    for (int i = 0; i < frame_num; i++) {
      asprintf(&finemesh1_name, "%s_%04d.obj", basename, i);
      asprintf(&finemesh2_name, "%s_%04d.obj", basename, i+1);
      read_triangle_mesh(finemesh1_name, V1, F);
      read_triangle_mesh(finemesh2_name, V2, F);
      // use previously generated cage as basis for new cage
      extrap_cage(V1, V2, F, V2_C, F_C, method, V2_C);
    }
  }
  
  igl::opengl::glfw::Viewer viewer;
  
  // just draw everything for now (use 't' to hide faces of individual meshes)
  add_to_viewer(viewer, V1, F);
  add_to_viewer(viewer, V2, F);
  add_to_viewer(viewer, V1_C, F_C);
  add_to_viewer(viewer, V2_C, F_C);
  
  if (ref_cage) {
    MatrixXd V_C_ref;
    MatrixXi F_C_ref;
    read_triangle_mesh(ref_cage, V_C_ref, F_C_ref);
    add_to_viewer(viewer, V_C_ref, F_C_ref);
  }
  
  view_all_meshes(viewer);
  
  char *output;
  asprintf(&output, "%s_%04d_gen_cage.obj", basename, frame_num);
  writeOBJ(output, V2_C, F_C);
  
  return 0;
}
