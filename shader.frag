#version 330 core
in vec4 gl_FragCoord;

out vec4 frag_color;

#define MAX_ITERATIONS 1000

uniform float center_x;
uniform float center_y;
uniform float zoom;

uniform float increment;

int get_iterations(vec2 coord) {
    float real = ((coord.x / 720.0 - 0.5f) * zoom + center_x) * 4.0f;
    float imag = ((coord.y / 720.0 - 0.5f) * zoom + center_y) * 4.0f;

    int iterations = 0;
    float const_real = real;
    float const_imag = imag;

    while (iterations < MAX_ITERATIONS) {
        float tmp_real = real;
        real = (real * real - imag * imag) + const_real + increment * 10;
        imag = (2.0f * tmp_real * imag) + const_imag;

        float dist = real * real + imag * imag;

        if (dist > 4.0f) break;

        ++iterations;
    }

    return iterations;
}

uniform vec4 color_ranges;

vec4 return_color(vec2 coord, out float depth) {
    int iter = get_iterations(coord);
    if (iter == MAX_ITERATIONS) {
        depth = 0.0f;
        return vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    float iterations = float(iter) / MAX_ITERATIONS;
    depth = iterations;

    vec4 color_0 = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 color_1 = vec4(0.0f, 0.2f, 0.5f, 1.0f);
    vec4 color_2 = vec4(1.0f, 0.8f, 0.0f, 1.0f);
    vec4 color_3 = vec4(1.0f, 0.0f, 0.4f, 1.0f);

    float fraction = 0.0f;
    if (iterations < color_ranges[1]) {
        fraction = (iterations - color_ranges[0]) / (color_ranges[1] - color_ranges[0]);
        return mix(color_0, color_1, fraction);
    } else if (iterations < color_ranges[2]) {
        fraction = (iterations - color_ranges[1]) / (color_ranges[2] - color_ranges[1]);
        return mix(color_1, color_2, fraction);
    } else {
        fraction = (iterations - color_ranges[2]) / (color_ranges[3] - color_ranges[2]);
        return mix(color_2, color_3, fraction);
    } 
}

void main() {
    vec2 offsets[4] = vec2[](
            vec2(0.25, 0.25),
            vec2(0.75, 0.25),
            vec2(0.25, 0.75),
            vec2(0.75, 0.75)
            );

    vec4 color_sum = vec4(0.0f);
    float depth_sum = 0.0f;

    for (int i = 0; i < 4; i++) {
        float depth;
        color_sum += return_color(gl_FragCoord.xy - 0.5 + offsets[i], depth);
        depth_sum += depth;
    }

    frag_color = color_sum / 4.0f;
    gl_FragDepth = depth_sum / 4.0f;
}
