#include "svg_renderer.h"
#include "draw_utils.h"
#include "animations.h"

#define DEFAULT_STEPS 40

void svg_render(const SVGCommand* commands, const float* params, size_t num_commands) {
    reset_frame();
    size_t param_idx = 0;
    
    for (size_t i = 0; i < num_commands; i++) {
        svg_render_single(i, commands, params);
    }
}

void svg_render_single(size_t cmd_index, const SVGCommand* commands, const float* params) {
    (void)cmd_index;
    SVGCommand cmd = commands[cmd_index];
    
    switch (cmd) {
        case SVG_CMD_MOVE_TO:
            add_point(params[0], params[1]);
            break;
            
        case SVG_CMD_LINE_TO:
            add_point(params[0], params[1]);
            break;
            
        case SVG_CMD_LINE:
            draw_line(params[0], params[1], params[2], params[3], DEFAULT_STEPS);
            blank_for(2);
            break;
            
        case SVG_CMD_CIRCLE:
            draw_circle(params[0], params[1], params[2], DEFAULT_STEPS);
            blank_for(2);
            break;
            
        case SVG_CMD_ELLIPSE:
            draw_ellipse(params[0], params[1], params[2], params[3], DEFAULT_STEPS);
            blank_for(2);
            break;
            
        case SVG_CMD_RECT:
            draw_rect(params[0], params[1], params[2], params[3], DEFAULT_STEPS / 4);
            blank_for(2);
            break;
            
        case SVG_CMD_BEZIER_QUADRATIC:
            draw_bezier_quadratic(params[0], params[1], params[2], params[3], params[4], params[5], DEFAULT_STEPS);
            blank_for(2);
            break;
            
        case SVG_CMD_BEZIER_CUBIC:
            draw_bezier_cubic(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], DEFAULT_STEPS);
            blank_for(2);
            break;
            
        case SVG_CMD_ARC:
            draw_arc(params[0], params[1], params[2], params[3], params[4], params[5], DEFAULT_STEPS);
            blank_for(2);
            break;
            
        case SVG_CMD_BLANK:
            blank_for((int)params[0]);
            break;
            
        case SVG_CMD_END:
        default:
            break;
    }
}
