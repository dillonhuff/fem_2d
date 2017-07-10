#include "visual_debug.h"

#include <vtkCellData.h>
#include <vtkCellArray.h>
#include <vtkLine.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolygon.h>
#include <vtkTriangle.h>
#include <vtkTriangleFilter.h>

#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkPoints.h>
#include <vtkFeatureEdges.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkClipPolyData.h>
#include <vtkProperty.h>
#include <vtkCellArray.h>
#include <vtkPolyDataNormals.h>
#include <vtkVertex.h>

#include <vtkAxesActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

using namespace std;

namespace fem_2d {

  class color {
  protected:
    unsigned r, g, b;

  public:
    color(unsigned p_r, unsigned p_g, unsigned p_b) :
      r(p_r), g(p_g), b(p_b) {}

    unsigned red() const { return r; }
    unsigned green() const { return r; }
    unsigned blue() const { return b; }
  };

  void color_polydata(vtkSmartPointer<vtkPolyData> data,
		      const unsigned char red,
		      const unsigned char green,
		      const unsigned char blue) {
    unsigned char color[3];
    color[0] = red;
    color[1] = green;
    color[2] = blue;

    // Create a vtkUnsignedCharArray container and store the colors in it
    vtkSmartPointer<vtkUnsignedCharArray> colors =
      vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    for (vtkIdType i = 0; i < data->GetNumberOfCells(); i++) {
      colors->InsertNextTupleValue(color);
    }
 

    data->GetCellData()->SetScalars(colors);

  }

  void color_polydata(vtkSmartPointer<vtkPolyData> data,
		      const std::vector<color>& colors) {

    // Create a vtkUnsignedCharArray container and store the colors in it
    vtkSmartPointer<vtkUnsignedCharArray> cell_colors =
      vtkSmartPointer<vtkUnsignedCharArray>::New();
    cell_colors->SetNumberOfComponents(3);
    for (vtkIdType i = 0; i < data->GetNumberOfCells(); i++) {
      unsigned char color[3];
      color[0] = colors[i].red();
      color[1] = colors[i].green();
      color[2] = colors[i].blue();

      cout << "red = " << colors[i].red() << endl;
      cout << "green = " << colors[i].green() << endl;
      cout << "blue = " << colors[i].blue() << endl;

      cell_colors->InsertNextTupleValue(color);
    }
 

    data->GetCellData()->SetScalars(cell_colors);

  }
  
  void visualize_actors(const std::vector<vtkSmartPointer<vtkActor> >& actors);

  vtkSmartPointer<vtkActor> polydata_actor(vtkSmartPointer<vtkPolyData> polyData)
  {
    vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polyData);
 
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    return actor;
  }
  
  void visualize_polydatas(const std::vector<vtkSmartPointer<vtkPolyData> >& pds) {
    std::vector<vtkSmartPointer<vtkActor> > actors;

    for (auto& pd : pds) {
      actors.push_back(polydata_actor(pd));
    }

    visualize_actors(actors);
  }

  void visualize_actors(const std::vector<vtkSmartPointer<vtkActor> >& actors)
  {
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    // Create axes
    vtkSmartPointer<vtkAxesActor> axes =
      vtkSmartPointer<vtkAxesActor>::New();

    renderer->AddActor(axes);
    for (auto actor : actors) {
      renderer->AddActor(actor);
    }
    renderer->SetBackground(0.0, 0.0, 0.0); //1.0, 1.0, 1.0); //0.0, 0.0, 0.0); //.1, .2, .3);

    vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    renderWindow->Render();
    renderWindowInteractor->Start();
  }

  vtkSmartPointer<vtkPolyData>
  polydata_for_trimesh(const trimesh& mesh) {
    vtkSmartPointer<vtkPoints> points =
      vtkSmartPointer<vtkPoints>::New();

    // TODO: Does i++ matter here?
    for (auto p : mesh.verts) {
      points->InsertNextPoint(p.x(), p.y(), 0.0);
    }

    vtkSmartPointer<vtkCellArray> triangles =
      vtkSmartPointer<vtkCellArray>::New();

    for (auto t : mesh.tris) {
      vtkSmartPointer<vtkTriangle> triangle =
	vtkSmartPointer<vtkTriangle>::New();

      triangle->GetPointIds()->SetId(0, t.verts[0]);
      triangle->GetPointIds()->SetId(1, t.verts[1]);
      triangle->GetPointIds()->SetId(2, t.verts[2]);

      triangles->InsertNextCell(triangle);    
    }

    // Create a polydata object
    vtkSmartPointer<vtkPolyData> polyData =
      vtkSmartPointer<vtkPolyData>::New();
 
    // Add the geometry and topology to the polydata
    polyData->SetPoints(points);
    polyData->SetPolys(triangles);

    return polyData;
  }
  
  void visualize_mesh(const trimesh& mesh) {
    auto pd = polydata_for_trimesh(mesh);
    visualize_polydatas({pd});
  }

  double norm(const ublas::vector<double>& vec) {
    double nm = 0.0;
    for (int i = 0; i < vec.size(); i++) {
      nm += vec(i)*vec(i);
    }
    return sqrt(nm);
  }

  double min_stress(const std::vector<ublas::vector<double>>& stresses) {
    assert(stresses.size() > 0);

    double min_stress = 1e30;
    for (auto& s : stresses) {
      double mag = norm(s);
      if (mag < min_stress) {
	min_stress = mag;
      }
    }

    return min_stress;
  }
  
  double max_stress(const std::vector<ublas::vector<double>>& stresses) {
    assert(stresses.size() > 0);

    double max_stress = -1.0;
    for (auto& s : stresses) {
      double mag = norm(s);
      if (mag > max_stress) {
	max_stress = mag;
      }
    }

    return max_stress;
  }

  color scaled_color(const double min,
		     const double max,
		     const double val) {
    assert(min <= val);
    assert(val <= max);

    double normed = val - min;
    double normed_max = max - min;

    double frac = (normed / normed_max)*100;
    int R = (255 * frac) / 100;
    int G = (255 * (100 - frac)) / 100 ;
    int B = 0;

    cout << "R = " << R << endl;
    cout << "G = " << G << endl;
    cout << "B = " << B << endl;

    return color(R, G, B);
  }

  void visualize_stresses(const trimesh& mesh,
			  const std::vector<ublas::vector<double>>& stresses) {
    assert(stresses.size() == mesh.tris.size());

    auto pd = polydata_for_trimesh(mesh);

    double ms = max_stress(stresses);
    double mn = min_stress(stresses);

    cout << "Max stress = " << ms << endl;
    cout << "Min stress = " << mn << endl;

    vector<color> colors;
    
    for (auto& s : stresses) {
      cout << s << endl;
      colors.push_back(scaled_color(mn, ms, norm(s)));
    }

    color_polydata(pd, colors);
    visualize_polydatas({pd});
  }

}
