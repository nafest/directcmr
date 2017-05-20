## directcmr - the direct CommonMark renderer you never wanted

directcmr renders CommonMark documents without intermediate conversion to another markup language like HTML.
Given a a simple renderer implementation (an example implementation with Skia is
included), directcmr directly outputs rendering calls.

### Basic Approach
1. Create a document instance from a string or from a file


    auto doc = dcmr::document::from_file("README.md");
    
2. Implement and set a backend for rendering


    my_backend_class my_backend;
    
    doc.set_backend(&my_backend);

2. Layout the document which returns the required height


    auto height = doc.layout(800);

3. Render the document


    doc.render(vec2(), 600);


### Handling of positions and margins
Every element contains its position in the coordinate system of its parent element. This
position does not contain any margin, which must be taken into account when rendering
the contents of an element.

### (incomplete) List of open tasks
- add clipping
- common base class for tests with styling resets
- document and cleanup the backend API
- it should be possible to provide an external canvas
- extend GitHubs cmark extensions to get the alignment of the table rows
- add support for more of GitHub's extensions
- read styling form a CSS-like file
- add additional line spacing (configurable), such that the rectangle of inline code spans
  can be increased in height
- add basic syntax highlighting in code blocks/spans
