#version 410 core

// in int gl_VertexID;
// in int gl_InstanceID;
// in int gl_DrawID;        // Requires GLSL 4.60 or ARB_shader_draw_parameters
// in int gl_BaseVertex;    // Requires GLSL 4.60 or ARB_shader_draw_parameters
// in int gl_BaseInstance;  // Requires GLSL 4.60 or ARB_shader_draw_parameters
// 
// out gl_PerVertex
// {
//     vec4     gl_Position;
//     float    gl_PointSize;
//     float    gl_ClipDistance[];
// };

layout(location = 0) in vec2 posAttr;

void main() {
    gl_Position = vec4(posAttr, 0.0, 1.0);
}
