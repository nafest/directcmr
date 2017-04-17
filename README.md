## cmark_renderer - directly render CommonMark

cmark_renderer renders CommonMark documents without intermediate conversion to another markup language like HTML.
Given a a simple renderer implementation (an example implementation with Skia is
included), cmark_renderer directly outputs rendering calls.

### Basic Approach
- Propagate font/style to the child nodes
- layout the document and compute the required height
- render the document

For the first implementation only support header, emph, paragraph,
and text nodes.

### Handling of positions and margins
Every element contains its position in the coordinate system of its parent element. This
position does not contain any margin, which must be taken into account when rendering
the contents of an element.

### (incomplete) List of open tasks
- the document API must be extended, such that the origin can be passed to render()
- it should be possible to provide an external canvas
- name the output of skia_cmark based on the input file
- extend GitHubs cmark extensions to get the alignment of the table rows
- add missing node types
    -  CMARK_NODE_IMAGE         = CMARK_NODE_TYPE_INLINE | 0x000a,
    -  CMARK_NODE_HTML_BLOCK     = CMARK_NODE_TYPE_BLOCK | 0x0006,
    -  CMARK_NODE_CUSTOM_BLOCK   = CMARK_NODE_TYPE_BLOCK | 0x0007,
    -  CMARK_NODE_THEMATIC_BREAK = CMARK_NODE_TYPE_BLOCK | 0x000a,

    -  CMARK_NODE_LINEBREAK     = CMARK_NODE_TYPE_INLINE | 0x0003,
    -  CMARK_NODE_HTML_INLINE   = CMARK_NODE_TYPE_INLINE | 0x0005,
    -  CMARK_NODE_CUSTOM_INLINE = CMARK_NODE_TYPE_INLINE | 0x0006,
    -  CMARK_NODE_LINK          = CMARK_NODE_TYPE_INLINE | 0x0009,
- read styling form a CSS-like file
- add a background rectangle (with rounded corners) to code blocks
- add additional line spacing (configurable), such that the rectangle of inline code spans
  can be increased in height
- add basic syntax highlighting in code spans
- handle subsequent text elements correctly (cmark splits cmark_renderer, e.g), the
  easiest solution seems to be to fuse subsequent text elements
