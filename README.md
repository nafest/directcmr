## cmark_renderer - directly render CommonMark

cmark_renderer renders CommonMark documents without intermediate conversion to another markup language like HTML.
Given a a simple renderer implementation (an example implementation with Skia is
included), cmark_renderer directly outputs rendering calls.

### Basic Approach
1. Create a document instance from a string or from a file
2. Layout the document which returns the required height
3. Render the document 

### Handling of positions and margins
Every element contains its position in the coordinate system of its parent element. This
position does not contain any margin, which must be taken into account when rendering
the contents of an element.

### (incomplete) List of open tasks
- extend the documentation of the public API, i.e. the document
  and the renderer class
- add a test with UFT-8 characters
- add a test for table_element
- add clipping
- it should be possible to provide an external canvas
- extend GitHubs cmark extensions to get the alignment of the table rows
- add support for more of GitHub's extensions
- read styling form a CSS-like file
- add additional line spacing (configurable), such that the rectangle of inline code spans
  can be increased in height
- add basic syntax highlighting in code blocks/spans
