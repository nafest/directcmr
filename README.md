### Basic Approach
- Propagate font/style to the child nodes
- layout the document and compute the required height
- render the doument

For the first implementation only support header, emph, paragraph,
and text nodes.

### Next Steps
- integrate skia via external project -> mostly working, needs windows alternative and some cleanup
- the document API must be extended, such that the origin can be passed to render()
- it should be possible to provide an external canvas
- cleanup/split element.h
- add a mechanism to store parameters with the renderer
- add lists
