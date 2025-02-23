#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_tex_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time; // Uniform for time

// Output
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_tex_coord;
out vec3 frag_color;

// Random function
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

// Noise function
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Smooth Interpolation

    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f*f*(3.0-2.0*f);
    // u = smoothstep(0.,1.,f);

    // Mix 4 coorners percentages
    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

void main() {
    // Compute noise-based height modification
    float frequency = 0.2;
    float noiseValue = noise(v_position.xz * frequency);
    float height = noiseValue * 0.5; // Reduced amplitude for gentle waves

    vec3 position = v_position;
    position.y = height;

    // Define color gradients
    vec3 color1 = vec3(0.0, 0.5, 0.0);    // Green
    vec3 color2 = vec3(1.0, 0.3, 0.0);    // Orange


    vec3 finalColor;
    finalColor = mix(color1, color2, noiseValue); // Interpolare direct? între verde ?i portocaliu


    // Set outputs
    frag_position = vec3(Model * vec4(position, 1.0));
    frag_normal = mat3(transpose(inverse(Model))) * v_normal; // Transform normals
    frag_tex_coord = v_tex_coord;
    frag_color = finalColor;

    // Compute final vertex position
    gl_Position = Projection * View * Model * vec4(position, 1.0);
}
