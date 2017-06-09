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

namespace fem_2d {

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

}
