# User Manual

## Compilation  
The ImGui, glm, glfw, and glew libraries are used.  
The ImGui and glm libraries are unpacked into the `src/vendor` folder, while GLFW and GLEW are added as project dependencies.

## Active Polygon  
While using the program, the active polygon is selected based on the active polygon tab in the menu (adding and removing polygons is also done based on these tabs).

## Polygon Modes  
Each polygon has 2 possible modes: the editing mode, which allows all operations, and the adding mode, which only allows adding vertices.

The polygon mode can be changed in the main menu.

### Add Mode  
In add mode, the only possible operation on the canvas is adding vertices by left-clicking.

### Edit Mode  
In edit mode, you can:
- Add a new vertex (from the active one) by double-clicking the left mouse button
- Activate a vertex by left-clicking on it
- Move vertices and edges by holding the left mouse button
- Move the active polygon by holding the left mouse button and the Shift key
- Add a relationship to an edge through the context menu by right-clicking on the selected edge
