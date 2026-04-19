#include "ShaderLibrary.h"

ShaderLibrary::ShaderLibrary()
{
	// Empty constructor
}

const ShaderSource& ShaderLibrary::Get(ShaderType type)
{
	return GetLibrary().at(type);
}

std::unordered_map<ShaderType, ShaderSource>& ShaderLibrary::GetLibrary()
{
    static std::unordered_map<ShaderType, ShaderSource> library =
    {
        {
            ShaderType::MeshShader,
            {
                mesh_vertex_shader(),
                mesh_fragment_shader()
            }
        },
        {
            ShaderType::TextShader,
            {
                text_vertex_shader(),
                text_fragment_shader()
            }
        }
    };

    return library;
}


std::string ShaderLibrary::mesh_vertex_shader()
{
    return R"(

#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout(location = 0) in vec2 node_position;
layout(location = 1) in float zval;

out float v_Zval;

void main()
{
    // Pass the z value to the fragment shader
    v_Zval = zval;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(node_position, 0.0, 1.0);
}

        )";


}



std::string ShaderLibrary::mesh_fragment_shader()
{
    return R"(

#version 330 core

uniform int CmapOption; // User selected color map option
uniform float vertexTransparency;

in float v_Zval;

out vec4 f_Color; // fragment's final color (out to the fragment shader)


vec3 jetHeatmap(float x)
{
    float r = clamp(1.5 - abs(4.0 * x - 3.0), 0.0, 1.0);
    float g = clamp(1.5 - abs(4.0 * x - 2.0), 0.0, 1.0);
    float b = clamp(1.5 - abs(4.0 * x - 1.0), 0.0, 1.0);
    return vec3(r, g, b);
}

void main() 
{

    vec3 vertexColor;
    
    if (CmapOption == 0)
    {   
        // Jet heatmap color mapping
        vertexColor = jetHeatmap(v_Zval);
    }
    else if(CmapOption == 1)
    {
        // Theme 2 color mapping etc
        vertexColor = vec3(0.2, 0.6, 0.8); // Sky Blue
    }

    f_Color = vec4(vertexColor, vertexTransparency); // Set the final color
}


        )";

}


