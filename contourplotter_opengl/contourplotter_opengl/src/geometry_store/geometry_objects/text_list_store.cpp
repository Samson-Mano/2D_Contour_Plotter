#include "text_list_store.h"

text_list_store::text_list_store()
{
	// Empty constructor

}

text_list_store::~text_list_store()
{
	// Empty destructor

}

void text_list_store::init(geom_parameters* geom_param_ptr)
{
	// Set the geometry parameter pointer
	this->geom_param_ptr = geom_param_ptr;

	// Create the label shader
	std::filesystem::path shadersPath = geom_param_ptr->resourcePath;

	text_shader.create_shader((shadersPath.string() + "/resources/shaders/text_vert_shader.vert").c_str(),
		(shadersPath.string() + "/resources/shaders/text_frag_shader.frag").c_str());

	// Set texture uniform variables
	text_shader.setUniform("u_Texture", 0);

	// Set vertex color
	text_shader.setUniform("vertexColor", geom_param_ptr->geom_colors.text_color);

	// Delete all the labels
	total_char_count = 0;
}

void text_list_store::add_text(std::string& label, glm::vec2& label_loc)
{
	// Create a temporary element
	text_label.label = label;
	text_label.label_loc = label_loc;

	// Add to the char_count
	total_char_count = static_cast<unsigned int>(label.size());

}



void text_list_store::update_text(std::string& label, glm::vec2& label_loc)
{

	// Curtail the length of text
	if (static_cast<unsigned int>(label.size()) > total_char_count)
	{
		label = label.substr(0, total_char_count);
	}

	// Create a temporary element
	text_label.label = label;
	text_label.label_loc = label_loc;

}



void text_list_store::set_buffer()
{
	// 6 indices to form a quadrilateral (2 trianlges) which is used for texture mapping
	unsigned int label_indices_count = 6 * total_char_count;
	unsigned int* label_vertex_indices = new unsigned int[label_indices_count];

	unsigned int label_i_index = 0;

	for (int i = 0; text_label.label[i] != '\0'; ++i)
	{
		// Create the index buffers for every individual character
		get_label_index_buffer(label_vertex_indices, label_i_index);
	}

	// Create a layout
	VertexBufferLayout label_layout;
	label_layout.AddFloat(2);  // Character Position
	label_layout.AddFloat(2);  // Texture glyph coordinate

	// Define the label vertices of the model for the entire label
	// (4 vertex (to form a triangle) * (2 character position +  2 Texture Glyph coordinate) 
	const unsigned int label_vertex_count = 4 * 4 * total_char_count;
	unsigned int label_vertex_size = label_vertex_count * sizeof(float); // Size of the node_vertex

	// Create the text dynamic buffers
	text_buffer.CreateDynamicBuffers(label_vertex_size, label_vertex_indices, label_indices_count, label_layout);

	// Delete the dynamic index array
	delete[] label_vertex_indices;

	update_buffer();

}

void text_list_store::set_text_color(const glm::vec3& text_color)
{
	// Set vertex color
	text_shader.setUniform("vertexColor", text_color);

}

void text_list_store::paint_static_texts()
{
	// Paint all the labels
	text_shader.Bind();
	text_buffer.Bind();

	glActiveTexture(GL_TEXTURE0);
	//// Bind the texture to the slot
	glBindTexture(GL_TEXTURE_2D, geom_param_ptr->main_font.textureID);

	glDrawElements(GL_TRIANGLES, 6 * total_char_count, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	text_buffer.UnBind();
	text_shader.UnBind();
}

void text_list_store::paint_dynamic_texts()
{
	// Paint all the labels
	text_shader.Bind();
	text_buffer.Bind();

	// Update the label buffer data for dynamic drawing
	update_buffer();

	glActiveTexture(GL_TEXTURE0);
	//// Bind the texture to the slot
	glBindTexture(GL_TEXTURE_2D, geom_param_ptr->main_font.textureID);

	glDrawElements(GL_TRIANGLES, 6 * total_char_count, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	text_buffer.UnBind();
	text_shader.UnBind();

}


void text_list_store::update_buffer()
{

	// Define the label vertices of the model for the entire label
	// (4 vertex (to form a triangle) * (2 character position + 2 Texture Glyph coordinate) 
	const unsigned int label_vertex_count = 4 * 4 * total_char_count;
	float* label_vertices = new float[label_vertex_count];

	unsigned int label_v_index = 0;

	// Set the label vertex buffers
	get_label_vertex_buffer(text_label, label_vertices, label_v_index);


	unsigned int label_vertex_size = label_vertex_count * sizeof(float); // Size of the tri_vertex

	// Update the buffer
	text_buffer.UpdateDynamicVertexBuffer(label_vertices, label_vertex_size);

	// Delete the dynamic vertices array
	delete[] label_vertices;
}


void text_list_store::clear_texts()
{
	// Delete all the labels
	// labels.clear();
	total_char_count = 0;

}

void text_list_store::update_opengl_uniforms(bool set_modelmatrix, bool set_viewmatrix, bool set_transparency)
{
	if (set_modelmatrix == true)
	{
		// set the transparency
		text_shader.setUniform("vertexTransparency", 1.0f);

		// text_shader.setUniform("geom_scale", static_cast<float>(geom_param_ptr->geom_scale));

		// set the model matrix
		text_shader.setUniform("modelMatrix", geom_param_ptr->modelMatrix, false);

		// set an unit view matrix
		glm::mat4 viewMatrix = glm::mat4(1.0);
		text_shader.setUniform("viewMatrix", viewMatrix, false);

	}

	if (set_viewmatrix == true)
	{

		glm::mat4 scalingMatrix = glm::mat4(1.0) * static_cast<float>(geom_param_ptr->zoom_scale);
		scalingMatrix[3][3] = 1.0f;

		glm::mat4 viewMatrix = glm::transpose(geom_param_ptr->panTranslation) * scalingMatrix;

		// set the pan translation
		text_shader.setUniform("viewMatrix", viewMatrix, false);
	}


	if (set_transparency == true)
	{
		// set the alpha transparency
		text_shader.setUniform("vertexTransparency", static_cast<float>(geom_param_ptr->geom_transparency));
	}

}

void text_list_store::get_label_vertex_buffer(text_store& txt, float* text_vertices, unsigned int& text_v_index)
{
	float font_scale = static_cast<float>(geom_param_ptr->font_size / geom_param_ptr->geom_scale);

	//// Find the label total width and total height
	//float total_label_width = 0.0f;
	//float total_label_height = 0.0f;


	//for (int i = 0; txt.label[i] != '\0'; ++i)
	//{
	//	// get the atlas information
	//	char ch = txt.label[i];
	//	Character ch_data = geom_param_ptr->main_font.ch_atlas[ch];

	//	total_label_width += (ch_data.Advance >> 6) * font_scale;
	//	total_label_height = std::max(total_label_height, ch_data.Size.y * font_scale);
	//}


	// Store the x,y location
	glm::vec2 loc = txt.label_loc;

	float x = loc.x; // -(total_label_width * 0.5f);
	float y = loc.y + (20.0 * font_scale);// -(total_label_height + (total_label_height * 0.5f)); // Paint below the location


	for (int i = 0; txt.label[i] != '\0'; ++i)
	{
		// get the atlas information
		char ch = txt.label[i];

		Character ch_data = geom_param_ptr->main_font.ch_atlas[ch];

		float xpos = x + (ch_data.Bearing.x * font_scale);
		float ypos = y - (ch_data.Size.y - ch_data.Bearing.y) * font_scale;

		float w = ch_data.Size.x * font_scale;
		float h = ch_data.Size.y * font_scale;

		float margin = 0.00002f; // This value prevents the minor overlap with the next char when rendering

		// Point 1
		// Vertices [0,0] // 0th point

		text_vertices[text_v_index + 0] = xpos;
		text_vertices[text_v_index + 1] = ypos + h;

		// Texture Glyph coordinate
		text_vertices[text_v_index + 2] = ch_data.top_left.x + margin;
		text_vertices[text_v_index + 3] = ch_data.top_left.y;

		// Iterate
		text_v_index = text_v_index + 4;

		//__________________________________________________________________________________________

		// Point 2
		// Vertices [0,1] // 1th point
		
		text_vertices[text_v_index + 0] = xpos;
		text_vertices[text_v_index + 1] = ypos;

		// Texture Glyph coordinate
		text_vertices[text_v_index + 2] = ch_data.top_left.x + margin;
		text_vertices[text_v_index + 3] = ch_data.bot_right.y;

		// Iterate
		text_v_index = text_v_index + 4;

		//__________________________________________________________________________________________

		// Point 3
		// Vertices [1,1] // 2th point

		text_vertices[text_v_index + 0] = xpos + w;
		text_vertices[text_v_index + 1] = ypos;

		// Texture Glyph coordinate
		text_vertices[text_v_index + 2] = ch_data.bot_right.x - margin;
		text_vertices[text_v_index + 3] = ch_data.bot_right.y;

		// Iterate
		text_v_index = text_v_index + 4;

		//__________________________________________________________________________________________

		// Point 4
		// Vertices [1,0] // 3th point

		text_vertices[text_v_index + 0] = xpos + w;
		text_vertices[text_v_index + 1] = ypos + h;

		// Texture Glyph coordinate
		text_vertices[text_v_index + 2] = ch_data.bot_right.x - margin;
		text_vertices[text_v_index + 3] = ch_data.top_left.y;

		// Iterate
		text_v_index = text_v_index + 4;

		//__________________________________________________________________________________________
		x += (ch_data.Advance >> 6) * font_scale;

	}

}

void text_list_store::get_label_index_buffer(unsigned int* text_vertex_indices, unsigned int& text_i_index)
{

	// Fix the index buffers
	// Set the node indices
	unsigned int t_id = ((text_i_index / 6) * 4);

	// Triangle 0,1,2
	text_vertex_indices[text_i_index + 0] = t_id + 0;
	text_vertex_indices[text_i_index + 1] = t_id + 1;
	text_vertex_indices[text_i_index + 2] = t_id + 2;

	// Triangle 2,3,0
	text_vertex_indices[text_i_index + 3] = t_id + 2;
	text_vertex_indices[text_i_index + 4] = t_id + 3;
	text_vertex_indices[text_i_index + 5] = t_id + 0;

	// Increment
	text_i_index = text_i_index + 6;

}
