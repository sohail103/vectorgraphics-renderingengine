#include <stdio.h>
#include "../src/generated_svg.c"

int main(void) {
    // call render function to ensure it links (in host build this will need svg_render stub)
    extern void svg_render(const SVGCommand* commands, const float* params, size_t num_commands);
    render_svg();
    printf("render_svg invoked\n");
    return 0;
}
