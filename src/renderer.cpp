#include "renderer.h"

font *renderer::font_for_style(const style &st) {
    auto search = m_cached_fonts.find(st);
    if (search != m_cached_fonts.end())
        return search->second;

    std::string style_str = "";
    if (st.get_strong())
        style_str += "Bold";
    if (st.get_emph())
        style_str += "Italic";

    if (style_str == "")
        style_str = "Normal";

    std::string family_str = default_family();

    int size = default_size();
    if (st.get_heading_level() > 0)
        size = heading_size(st.get_heading_level());

    auto font = create_font(family_str, style_str, size);
    m_cached_fonts[st] = font;
    return font;
}
