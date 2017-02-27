### Basic Approach
- Propagate font/style to the child nodes
- layout the document and compute the required height
- render the doument

For the first implementation only support header, emph, paragraph,
and text nodes.

### Next Steps
- Push the current state somewhere (either bitbucket or a private github repo) (/)
- Since only a limited set of fonts is required, figure out a
  way to store these fonts globally (e.g. in a map) (/)
- implement the font/style propagation for the types above
    - styles are propagated. -> fonts have been choosen (from a font_chache?) according to this style (/)
    - font_cache could be a fixed part of the renderer and renderer could get a font_for_style() method (/)
- Store layouted words in text element to reuse them in the render step
- implement the render step for the elements above
