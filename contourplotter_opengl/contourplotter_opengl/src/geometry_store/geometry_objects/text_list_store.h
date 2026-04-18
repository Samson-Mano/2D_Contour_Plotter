#pragma once
#include <iostream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "../geometry_buffers/gBuffers.h"
#include "../geometry_buffers/font_atlas.h"
#include "../geom_parameters.h"


struct text_store
{
	// Store the individual label
	int label_id = -1;
	std::string label = "";
	glm::vec2 label_loc = glm::vec2(0);

};


class text_list_store
{
	// Stores all the labels
public:
	unsigned int total_char_count = 0;
	text_store text_label;

	text_list_store();
	~text_list_store();

	void init(geom_parameters* geom_param_ptr);
	void add_text(std::string& label, glm::vec2& label_loc);
	void update_text(std::string& label, glm::vec2& label_loc);

	void set_buffer();
	void update_buffer();
	void set_text_color(const glm::vec3& text_color);

	void paint_static_texts();
	void paint_dynamic_texts();


	void clear_texts();
	void update_opengl_uniforms(bool set_modelmatrix, bool set_viewmatrix, bool set_transparency);
private:
	geom_parameters* geom_param_ptr = nullptr;

	gBuffers text_buffer;
	Shader text_shader;

	void get_label_vertex_buffer(text_store& txt, float* text_vertices, unsigned int& text_v_index);
		
	void get_label_index_buffer(unsigned int* text_vertex_indices, unsigned int& text_i_index);

};

