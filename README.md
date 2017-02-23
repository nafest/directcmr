### Basic Approach
- Propagate font/style to the child nodes
- layout the document and compute the required height
- render the doument

For the first implementation only support header, emph, paragraph,
and text nodes.

### Next Steps
- Since only a limited set of fonts is required, figure out a
  way to store these fonts globally (e.g. in a map)
- implement the font/style propagation for the types above
