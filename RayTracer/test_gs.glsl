#version 440

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec4 vs_color[];
out vec4 gs_color;

void main(void)
{
	int i;
	for (i = 0; i < gl_in.length(); i++)
    {
		gl_Position = gl_in[i].gl_Position;
		gs_color = vs_color[i];
		EmitVertex();
    }
	EndPrimitive();
}
