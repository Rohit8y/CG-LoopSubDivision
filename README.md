# Advance Computer Graphics Assignment 3
In this assignment loop subdivision scheme is used and the application has the following functionalities:
- MA: Geometry refinement
- MA: Boundary rules
- MA: Isophotes
- AF: Vertex selection using raycasting 
> NOTE:- Vertex selection feature works best in Opencube model and mouse events are disabled deliberately in this feature.

### Enviornment
>OPENGL 4.1
>
>QTCREATOR 6.4

To run the application, open the file CMakeLists.txt in QTCreator.

## MA: Geometry refinement & Boundary rules
The geometry refinement for vertex and edges is implemented in loopsubdivider.cpp file.  For both vertex point and edge point, it is first schecked if the given vertex/edge is on boundary or not. Depending on that we use Loop's stencil to find the position of these points. For vertex points we also need to find position of all surrounding neighbours, which is done by using the half-edge data structure given in the framework. The given function finds the position of all surrounding neighbours:
>std::vector<QVector3D> LoopSubdivider::getSurroundingCoords(const Vertex& vertex)
  
For edge vertices on boundary, the valence is by default set to 4 after performing visual inspection. Finally to see the results in UI after loading the model select Phong shading under Render settings and increase the subdivision level. the max. value of subdivision level is set to 8.

## MA: Isophotes
Isophotes are implemented in the fragment shader isophotes.frag by finding the angle between a fix vector(1,0,0) and the normal. This angle is given as input to the sin function multiplied by the frequency of stripes required. The frequency is a uniform variable who's value is updated from UI. As the output of sin lies between [-1,1], so the value is scaled to [0,1]. This scaled value is used to decide the color of the stripe in the final mesh. To make a more user friendly UI, three different color stripe options are given in th UI:
  - Black & White
  - Red & White
  - Blue & White

The frequency and the color of stripes can only be choosen once the user clicks on the Isophotes checkbox under Render settings. To make the UI user friendly, at a given time the user can only select Phong shading or Isophotes checkbox from the UI. The frequency value has a limit of 50.
  
## AF : Vertex selection
### This feature can be only used on the Phong shading mesh and all the mouse events are disabled intentionally. The raycasting works best in case of Opencube model.
  
Every time the checkbox of Vertex selection is selected from the UI, the scale and rotation of the window is reset to their default values, which means the user cannot zoom in/out and rotate the mesh as long as the checkbox is checked.
 
The vertex selection is implemented in mainView.cpp file and is done using raycasting. First the mouse coordinates (x,y) are taken and transformed to Normalised device coordinates with [-1,1] range. Then the ray is clipped and viewport transformation is done by inversing the projection matrix. After that the w component of the object is multiplied to the ray and inverse modelview is performed to finally get the world coordinates. Finally we have the 3D point, and we check in the given mesh, which point is closest and highlight that point.
