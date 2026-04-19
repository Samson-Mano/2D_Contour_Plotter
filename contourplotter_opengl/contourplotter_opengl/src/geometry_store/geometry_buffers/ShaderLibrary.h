#pragma once
#include <string>
#include <unordered_map>



class ShaderLibrary
{
    public:
        enum class ShaderType
        {
            MeshShader,
            TextShader
        };

        struct ShaderSource
        {
            std::string vertex;
            std::string fragment;
        };


        ShaderLibrary();
        ~ShaderLibrary() = default;

        static const ShaderSource& Get(ShaderType type);


private:
    static std::unordered_map<ShaderType, ShaderSource>& GetLibrary();

	// Mesh shader
    static std::string mesh_vertex_shader();
    static std::string mesh_fragment_shader();

	// Text shader
    static std::string text_vertex_shader();
    static std::string text_fragment_shader();


};