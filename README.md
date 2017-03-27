### Basic Approach
- Propagate font/style to the child nodes
- layout the document and compute the required height
- render the doument

For the first implementation only support header, emph, paragraph,
and text nodes.

### Next Steps
- complete code node implementation
- integrate skia via external project -> mostly working, needs windows alternative and some cleanup
- the document API must be extended, such that the origin can be passed to render()
- it should be possible to provide an external canvas
- cleanup/split element.h
- add a mechanism to store parameters with the renderer
- add missing node types
  CMARK_NODE_BLOCK_QUOTE    = CMARK_NODE_TYPE_BLOCK | 0x0002,
  CMARK_NODE_HTML_BLOCK     = CMARK_NODE_TYPE_BLOCK | 0x0006,
  CMARK_NODE_CUSTOM_BLOCK   = CMARK_NODE_TYPE_BLOCK | 0x0007,
  CMARK_NODE_THEMATIC_BREAK = CMARK_NODE_TYPE_BLOCK | 0x000a,

  CMARK_NODE_SOFTBREAK     = CMARK_NODE_TYPE_INLINE | 0x0002,
  CMARK_NODE_LINEBREAK     = CMARK_NODE_TYPE_INLINE | 0x0003,
  CMARK_NODE_CODE          = CMARK_NODE_TYPE_INLINE | 0x0004,
  CMARK_NODE_HTML_INLINE   = CMARK_NODE_TYPE_INLINE | 0x0005,
  CMARK_NODE_CUSTOM_INLINE = CMARK_NODE_TYPE_INLINE | 0x0006,
  CMARK_NODE_LINK          = CMARK_NODE_TYPE_INLINE | 0x0009,
  CMARK_NODE_IMAGE         = CMARK_NODE_TYPE_INLINE | 0x000a,
}
