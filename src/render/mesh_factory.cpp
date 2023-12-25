#include "render/mesh_factory.hpp"

#include "render/material_factory.hpp"

namespace duckhacker
{
	namespace render
	{
		Mesh * MeshFactory::Box(Shader * shader, float width, float height, float depth)
		{
			size_t vertices_count = 36;
			size_t vertices_stride = 8;
			size_t vertices_size = vertices_count * vertices_stride;
			float * vertices = (float *) malloc(vertices_size * sizeof(float));

			/*
			* front face
			*/
			vertices[(vertices_stride * 0) + 0] = (width / 2);
			vertices[(vertices_stride * 0) + 1] = (height / 2);
			vertices[(vertices_stride * 0) + 2] = (depth / 2);
			vertices[(vertices_stride * 0) + 3] = 0.00f;
			vertices[(vertices_stride * 0) + 4] = 0.00f;
			vertices[(vertices_stride * 0) + 5] = 1.00f;
			vertices[(vertices_stride * 0) + 6] = 1.00f;
			vertices[(vertices_stride * 0) + 7] = 1.00f;

			vertices[(vertices_stride * 1) + 0] = (width / 2);
			vertices[(vertices_stride * 1) + 1] = -(height / 2);
			vertices[(vertices_stride * 1) + 2] = (depth / 2);
			vertices[(vertices_stride * 1) + 3] = 0.00f;
			vertices[(vertices_stride * 1) + 4] = 0.00f;
			vertices[(vertices_stride * 1) + 5] = 1.00f;
			vertices[(vertices_stride * 1) + 6] = 1.00f;
			vertices[(vertices_stride * 1) + 7] = 0.00f;

			vertices[(vertices_stride * 2) + 0] = -(width / 2);
			vertices[(vertices_stride * 2) + 1] = -(height / 2);
			vertices[(vertices_stride * 2) + 2] = (depth / 2);
			vertices[(vertices_stride * 2) + 3] = 0.00f;
			vertices[(vertices_stride * 2) + 4] = 0.00f;
			vertices[(vertices_stride * 2) + 5] = 1.00f;
			vertices[(vertices_stride * 2) + 6] = 0.00f;
			vertices[(vertices_stride * 2) + 7] = 0.00f;

			vertices[(vertices_stride * 3) + 0] = -(width / 2);
			vertices[(vertices_stride * 3) + 1] = (height / 2);
			vertices[(vertices_stride * 3) + 2] = (depth / 2);
			vertices[(vertices_stride * 3) + 3] = 0.00f;
			vertices[(vertices_stride * 3) + 4] = 0.00f;
			vertices[(vertices_stride * 3) + 5] = 1.00f;
			vertices[(vertices_stride * 3) + 6] = 0.00f;
			vertices[(vertices_stride * 3) + 7] = 1.00f;

			vertices[(vertices_stride * 4) + 0] = (width / 2);
			vertices[(vertices_stride * 4) + 1] = (height / 2);
			vertices[(vertices_stride * 4) + 2] = (depth / 2);
			vertices[(vertices_stride * 4) + 3] = 0.00f;
			vertices[(vertices_stride * 4) + 4] = 0.00f;
			vertices[(vertices_stride * 4) + 5] = 1.00f;
			vertices[(vertices_stride * 4) + 6] = 1.00f;
			vertices[(vertices_stride * 4) + 7] = 1.00f;

			vertices[(vertices_stride * 5) + 0] = -(width / 2);
			vertices[(vertices_stride * 5) + 1] = -(height / 2);
			vertices[(vertices_stride * 5) + 2] = (depth / 2);
			vertices[(vertices_stride * 5) + 3] = 0.00f;
			vertices[(vertices_stride * 5) + 4] = 0.00f;
			vertices[(vertices_stride * 5) + 5] = 1.00f;
			vertices[(vertices_stride * 5) + 6] = 0.00f;
			vertices[(vertices_stride * 5) + 7] = 0.00f;

			/*
			* back face
			*/
			vertices[(vertices_stride * 6) + 0] = -(width / 2);
			vertices[(vertices_stride * 6) + 1] = -(height / 2);
			vertices[(vertices_stride * 6) + 2] = -(depth / 2);
			vertices[(vertices_stride * 6) + 3] = 0.00f;
			vertices[(vertices_stride * 6) + 4] = 0.00f;
			vertices[(vertices_stride * 6) + 5] = -1.00f;
			vertices[(vertices_stride * 6) + 6] = 0.32f;
			vertices[(vertices_stride * 6) + 7] = 0.32f;

			vertices[(vertices_stride * 7) + 0] = (width / 2);
			vertices[(vertices_stride * 7) + 1] = -(height / 2);
			vertices[(vertices_stride * 7) + 2] = -(depth / 2);
			vertices[(vertices_stride * 7) + 3] = 0.00f;
			vertices[(vertices_stride * 7) + 4] = 0.00f;
			vertices[(vertices_stride * 7) + 5] = -1.00f;
			vertices[(vertices_stride * 7) + 6] = 0.32f;
			vertices[(vertices_stride * 7) + 7] = 0.32f;

			vertices[(vertices_stride * 8) + 0] = (width / 2);
			vertices[(vertices_stride * 8) + 1] = (height / 2);
			vertices[(vertices_stride * 8) + 2] = -(depth / 2);
			vertices[(vertices_stride * 8) + 3] = 0.00f;
			vertices[(vertices_stride * 8) + 4] = 0.00f;
			vertices[(vertices_stride * 8) + 5] = -1.00f;
			vertices[(vertices_stride * 8) + 6] = 0.32f;
			vertices[(vertices_stride * 8) + 7] = 0.32f;

			vertices[(vertices_stride * 9) + 0] = (width / 2);
			vertices[(vertices_stride * 9) + 1] = (height / 2);
			vertices[(vertices_stride * 9) + 2] = -(depth / 2);
			vertices[(vertices_stride * 9) + 3] = 0.00f;
			vertices[(vertices_stride * 9) + 4] = 0.00f;
			vertices[(vertices_stride * 9) + 5] = -1.00f;
			vertices[(vertices_stride * 9) + 6] = 0.32f;
			vertices[(vertices_stride * 9) + 7] = 0.32f;

			vertices[(vertices_stride * 10) + 0] = -(width / 2);
			vertices[(vertices_stride * 10) + 1] = (height / 2);
			vertices[(vertices_stride * 10) + 2] = -(depth / 2);
			vertices[(vertices_stride * 10) + 3] = 0.00f;
			vertices[(vertices_stride * 10) + 4] = 0.00f;
			vertices[(vertices_stride * 10) + 5] = -1.00f;
			vertices[(vertices_stride * 10) + 6] = 0.32f;
			vertices[(vertices_stride * 10) + 7] = 0.32f;

			vertices[(vertices_stride * 11) + 0] = -(width / 2);
			vertices[(vertices_stride * 11) + 1] = -(height / 2);
			vertices[(vertices_stride * 11) + 2] = -(depth / 2);
			vertices[(vertices_stride * 11) + 3] = 0.00f;
			vertices[(vertices_stride * 11) + 4] = 0.00f;
			vertices[(vertices_stride * 11) + 5] = -1.00f;
			vertices[(vertices_stride * 11) + 6] = 0.32f;
			vertices[(vertices_stride * 11) + 7] = 0.32f;

			/*
			* left face
			*/
			vertices[(vertices_stride * 12) + 0] = -(width / 2);
			vertices[(vertices_stride * 12) + 1] = -(height / 2);
			vertices[(vertices_stride * 12) + 2] = -(depth / 2);
			vertices[(vertices_stride * 12) + 3] = -1.00f;
			vertices[(vertices_stride * 12) + 4] = 0.00f;
			vertices[(vertices_stride * 12) + 5] = 0.00f;
			vertices[(vertices_stride * 12) + 6] = 0.42f;
			vertices[(vertices_stride * 12) + 7] = 0.42f;

			vertices[(vertices_stride * 13) + 0] = -(width / 2);
			vertices[(vertices_stride * 13) + 1] = (height / 2);
			vertices[(vertices_stride * 13) + 2] = (depth / 2);
			vertices[(vertices_stride * 13) + 3] = -1.00f;
			vertices[(vertices_stride * 13) + 4] = 0.00f;
			vertices[(vertices_stride * 13) + 5] = 0.00f;
			vertices[(vertices_stride * 13) + 6] = 0.42f;
			vertices[(vertices_stride * 13) + 7] = 0.42f;

			vertices[(vertices_stride * 14) + 0] = -(width / 2);
			vertices[(vertices_stride * 14) + 1] = (height / 2);
			vertices[(vertices_stride * 14) + 2] = -(depth / 2);
			vertices[(vertices_stride * 14) + 3] = -1.00f;
			vertices[(vertices_stride * 14) + 4] = 0.00f;
			vertices[(vertices_stride * 14) + 5] = 0.00f;
			vertices[(vertices_stride * 14) + 6] = 0.42f;
			vertices[(vertices_stride * 14) + 7] = 0.42f;

			vertices[(vertices_stride * 15) + 0] = -(width / 2);
			vertices[(vertices_stride * 15) + 1] = (height / 2);
			vertices[(vertices_stride * 15) + 2] = (depth / 2);
			vertices[(vertices_stride * 15) + 3] = -1.00f;
			vertices[(vertices_stride * 15) + 4] = 0.00f;
			vertices[(vertices_stride * 15) + 5] = 0.00f;
			vertices[(vertices_stride * 15) + 6] = 0.42f;
			vertices[(vertices_stride * 15) + 7] = 0.42f;

			vertices[(vertices_stride * 16) + 0] = -(width / 2);
			vertices[(vertices_stride * 16) + 1] = -(height / 2);
			vertices[(vertices_stride * 16) + 2] = -(depth / 2);
			vertices[(vertices_stride * 16) + 3] = -1.00f;
			vertices[(vertices_stride * 16) + 4] = 0.00f;
			vertices[(vertices_stride * 16) + 5] = 0.00f;
			vertices[(vertices_stride * 16) + 6] = 0.42f;
			vertices[(vertices_stride * 16) + 7] = 0.42f;

			vertices[(vertices_stride * 17) + 0] = -(width / 2);
			vertices[(vertices_stride * 17) + 1] = -(height / 2);
			vertices[(vertices_stride * 17) + 2] = (depth / 2);
			vertices[(vertices_stride * 17) + 3] = -1.00f;
			vertices[(vertices_stride * 17) + 4] = 0.00f;
			vertices[(vertices_stride * 17) + 5] = 0.00f;
			vertices[(vertices_stride * 17) + 6] = 0.42f;
			vertices[(vertices_stride * 17) + 7] = 0.42f;

			/*
			* right face
			*/
			vertices[(vertices_stride * 18) + 0] = (width / 2);
			vertices[(vertices_stride * 18) + 1] = (height / 2);
			vertices[(vertices_stride * 18) + 2] = (depth / 2);
			vertices[(vertices_stride * 18) + 3] = 1.00f;
			vertices[(vertices_stride * 18) + 4] = 0.00f;
			vertices[(vertices_stride * 18) + 5] = 0.00f;
			vertices[(vertices_stride * 18) + 6] = 0.42f;
			vertices[(vertices_stride * 18) + 7] = 0.42f;

			vertices[(vertices_stride * 19) + 0] = (width / 2);
			vertices[(vertices_stride * 19) + 1] = -(height / 2);
			vertices[(vertices_stride * 19) + 2] = -(depth / 2);
			vertices[(vertices_stride * 19) + 3] = 1.00f;
			vertices[(vertices_stride * 19) + 4] = 0.00f;
			vertices[(vertices_stride * 19) + 5] = 0.00f;
			vertices[(vertices_stride * 19) + 6] = 0.42f;
			vertices[(vertices_stride * 19) + 7] = 0.42f;

			vertices[(vertices_stride * 20) + 0] = (width / 2);
			vertices[(vertices_stride * 20) + 1] = (height / 2);
			vertices[(vertices_stride * 20) + 2] = -(depth / 2);
			vertices[(vertices_stride * 20) + 3] = 1.00f;
			vertices[(vertices_stride * 20) + 4] = 0.00f;
			vertices[(vertices_stride * 20) + 5] = 0.00f;
			vertices[(vertices_stride * 20) + 6] = 0.42f;
			vertices[(vertices_stride * 20) + 7] = 0.42f;

			vertices[(vertices_stride * 21) + 0] = (width / 2);
			vertices[(vertices_stride * 21) + 1] = -(height / 2);
			vertices[(vertices_stride * 21) + 2] = -(depth / 2);
			vertices[(vertices_stride * 21) + 3] = 1.00f;
			vertices[(vertices_stride * 21) + 4] = 0.00f;
			vertices[(vertices_stride * 21) + 5] = 0.00f;
			vertices[(vertices_stride * 21) + 6] = 0.42f;
			vertices[(vertices_stride * 21) + 7] = 0.42f;

			vertices[(vertices_stride * 22) + 0] = (width / 2);
			vertices[(vertices_stride * 22) + 1] = -(height / 2);
			vertices[(vertices_stride * 22) + 2] = (depth / 2);
			vertices[(vertices_stride * 22) + 3] = 1.00f;
			vertices[(vertices_stride * 22) + 4] = 0.00f;
			vertices[(vertices_stride * 22) + 5] = 0.00f;
			vertices[(vertices_stride * 22) + 6] = 0.42f;
			vertices[(vertices_stride * 22) + 7] = 0.42f;

			vertices[(vertices_stride * 23) + 0] = (width / 2);
			vertices[(vertices_stride * 23) + 1] = (height / 2);
			vertices[(vertices_stride * 23) + 2] = (depth / 2);
			vertices[(vertices_stride * 23) + 3] = 1.00f;
			vertices[(vertices_stride * 23) + 4] = 0.00f;
			vertices[(vertices_stride * 23) + 5] = 0.00f;
			vertices[(vertices_stride * 23) + 6] = 0.42f;
			vertices[(vertices_stride * 23) + 7] = 0.42f;


			/*
			* bottom face
			*/
			vertices[(vertices_stride * 24) + 0] = -(width / 2);
			vertices[(vertices_stride * 24) + 1] = -(height / 2);
			vertices[(vertices_stride * 24) + 2] = (depth / 2);
			vertices[(vertices_stride * 24) + 3] = 0.00f;
			vertices[(vertices_stride * 24) + 4] = -1.00f;
			vertices[(vertices_stride * 24) + 5] = 0.00f;
			vertices[(vertices_stride * 24) + 6] = 0.42f;
			vertices[(vertices_stride * 24) + 7] = 0.42f;

			vertices[(vertices_stride * 25) + 0] = -(width / 2);
			vertices[(vertices_stride * 25) + 1] = -(height / 2);
			vertices[(vertices_stride * 25) + 2] = -(depth / 2);
			vertices[(vertices_stride * 25) + 3] = 0.00f;
			vertices[(vertices_stride * 25) + 4] = -1.00f;
			vertices[(vertices_stride * 25) + 5] = 0.00f;
			vertices[(vertices_stride * 25) + 6] = 0.42f;
			vertices[(vertices_stride * 25) + 7] = 0.42f;

			vertices[(vertices_stride * 26) + 0] = (width / 2);
			vertices[(vertices_stride * 26) + 1] = -(height / 2);
			vertices[(vertices_stride * 26) + 2] = -(depth / 2);
			vertices[(vertices_stride * 26) + 3] = 0.00f;
			vertices[(vertices_stride * 26) + 4] = -1.00f;
			vertices[(vertices_stride * 26) + 5] = 0.00f;
			vertices[(vertices_stride * 26) + 6] = 0.42f;
			vertices[(vertices_stride * 26) + 7] = 0.42f;

			vertices[(vertices_stride * 27) + 0] = (width / 2);
			vertices[(vertices_stride * 27) + 1] = -(height / 2);
			vertices[(vertices_stride * 27) + 2] = -(depth / 2);
			vertices[(vertices_stride * 27) + 3] = 0.00f;
			vertices[(vertices_stride * 27) + 4] = -1.00f;
			vertices[(vertices_stride * 27) + 5] = 0.00f;
			vertices[(vertices_stride * 27) + 6] = 0.42f;
			vertices[(vertices_stride * 27) + 7] = 0.42f;

			vertices[(vertices_stride * 28) + 0] = (width / 2);
			vertices[(vertices_stride * 28) + 1] = -(height / 2);
			vertices[(vertices_stride * 28) + 2] = (depth / 2);
			vertices[(vertices_stride * 28) + 3] = 0.00f;
			vertices[(vertices_stride * 28) + 4] = -1.00f;
			vertices[(vertices_stride * 28) + 5] = 0.00f;
			vertices[(vertices_stride * 28) + 6] = 0.42f;
			vertices[(vertices_stride * 28) + 7] = 0.42f;

			vertices[(vertices_stride * 29) + 0] = -(width / 2);
			vertices[(vertices_stride * 29) + 1] = -(height / 2);
			vertices[(vertices_stride * 29) + 2] = (depth / 2);
			vertices[(vertices_stride * 29) + 3] = 0.00f;
			vertices[(vertices_stride * 29) + 4] = -1.00f;
			vertices[(vertices_stride * 29) + 5] = 0.00f;
			vertices[(vertices_stride * 29) + 6] = 0.42f;
			vertices[(vertices_stride * 29) + 7] = 0.42f;

			/*
			* top face
			*/
			vertices[(vertices_stride * 30) + 0] = -(width / 2);
			vertices[(vertices_stride * 30) + 1] = (height / 2);
			vertices[(vertices_stride * 30) + 2] = (depth / 2);
			vertices[(vertices_stride * 30) + 3] = 0.00f;
			vertices[(vertices_stride * 30) + 4] = 1.00f;
			vertices[(vertices_stride * 30) + 5] = 0.00f;
			vertices[(vertices_stride * 30) + 6] = 0.42f;
			vertices[(vertices_stride * 30) + 7] = 0.42f;

			vertices[(vertices_stride * 31) + 0] = (width / 2);
			vertices[(vertices_stride * 31) + 1] = (height / 2);
			vertices[(vertices_stride * 31) + 2] = -(depth / 2);
			vertices[(vertices_stride * 31) + 3] = 0.00f;
			vertices[(vertices_stride * 31) + 4] = 1.00f;
			vertices[(vertices_stride * 31) + 5] = 0.00f;
			vertices[(vertices_stride * 31) + 6] = 0.42f;
			vertices[(vertices_stride * 31) + 7] = 0.42f;

			vertices[(vertices_stride * 32) + 0] = -(width / 2);
			vertices[(vertices_stride * 32) + 1] = (height / 2);
			vertices[(vertices_stride * 32) + 2] = -(depth / 2);
			vertices[(vertices_stride * 32) + 3] = 0.00f;
			vertices[(vertices_stride * 32) + 4] = 1.00f;
			vertices[(vertices_stride * 32) + 5] = 0.00f;
			vertices[(vertices_stride * 32) + 6] = 0.42f;
			vertices[(vertices_stride * 32) + 7] = 0.42f;

			vertices[(vertices_stride * 33) + 0] = (width / 2);
			vertices[(vertices_stride * 33) + 1] = (height / 2);
			vertices[(vertices_stride * 33) + 2] = -(depth / 2);
			vertices[(vertices_stride * 33) + 3] = 0.00f;
			vertices[(vertices_stride * 33) + 4] = 1.00f;
			vertices[(vertices_stride * 33) + 5] = 0.00f;
			vertices[(vertices_stride * 33) + 6] = 0.42f;
			vertices[(vertices_stride * 33) + 7] = 0.42f;

			vertices[(vertices_stride * 34) + 0] = -(width / 2);
			vertices[(vertices_stride * 34) + 1] = (height / 2);
			vertices[(vertices_stride * 34) + 2] = (depth / 2);
			vertices[(vertices_stride * 34) + 3] = 0.00f;
			vertices[(vertices_stride * 34) + 4] = 1.00f;
			vertices[(vertices_stride * 34) + 5] = 0.00f;
			vertices[(vertices_stride * 34) + 6] = 0.42f;
			vertices[(vertices_stride * 34) + 7] = 0.42f;

			vertices[(vertices_stride * 35) + 0] = (width / 2);
			vertices[(vertices_stride * 35) + 1] = (height / 2);
			vertices[(vertices_stride * 35) + 2] = (depth / 2);
			vertices[(vertices_stride * 35) + 3] = 0.00f;
			vertices[(vertices_stride * 35) + 4] = 1.00f;
			vertices[(vertices_stride * 35) + 5] = 0.00f;
			vertices[(vertices_stride * 35) + 6] = 0.42f;
			vertices[(vertices_stride * 35) + 7] = 0.42f;

			Mesh * box = new Mesh(shader, vertices, vertices_size * sizeof(float), vertices_count);
			return box;
		}

		static bool is_unwanted_whitespace(unsigned char c)
		{
			return (c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == '\f');
		}

		typedef struct {
			float x;
			float y;
			float z;
		} float3;

		typedef struct {
			float x;
			float y;
			float z;
			float nx;
			float ny;
			float nz;
			float u;
			float v;
		} vertex;

		Mesh * MeshFactory::OBJ(content::Manager * content_manager, Shader * shader, const char * path)
		{
			// we read the file in to a buffer
			// TODO: can we make this better by reading chunk-by-chunk from the file?
			char * file_data = content_manager->File(path, nullptr);

			std::vector<float3> vertex_coords;
			std::vector<float3> vertex_normals;
			std::vector<float3> vertex_textures;

			std::vector<vertex> vertices;

			std::vector<std::tuple<size_t, Material>> material_changes;

			std::map<std::string, Material> materials;

			char * file_position = file_data;
			while (true)
			{
				char * line_end = strchr(file_position, '\n');
				if (!line_end)
				{
					// end of file
					break;
				}
				size_t line_length = line_end - file_position;

				// we now have a line, starting at file_position and continuing for line_length bytes
				std::string line = std::string(file_position, line_length);
				file_position += line_length + 1;

				line.erase(std::remove_if(line.begin(), line.end(), is_unwanted_whitespace), line.end());

				if (line.size() == 0)
				{
					// blank line
					continue;
				}

				if (line[0] == '#')
				{
					// comment
					continue;
				}

				// tokenize it
				std::vector<std::string> tokens;
				std::istringstream line_stream(line);
				std::string token;
				while (std::getline(line_stream, token, ' '))
				{
					tokens.push_back(token);
				}

				// std::cout << "LINE: " << line << std::endl;

				// decide what to do based on the first token
				std::string& first_token = tokens[0];
				if (first_token == "v" || first_token == "vn" || first_token == "vt")
				{
					// info about a vertex!
					float3 vertex_info_data;
					size_t expected_number = (first_token == "vt" ? 2 : 3);

					if (tokens.size() < expected_number + 1)
					{
						// invalid
						std::cout << "Could not parse line: " << line << std::endl;
						break;
					}

					vertex_info_data.x = std::stof(tokens[1]);
					vertex_info_data.y = std::stof(tokens[2]);
					if (expected_number == 3)
					{
						vertex_info_data.z = std::stof(tokens[3]);
					}

					if (first_token == "v")
					{
						vertex_coords.push_back(vertex_info_data);
					}
					else if (first_token == "vn")
					{
						vertex_normals.push_back(vertex_info_data);
					}
					else if (first_token == "vt")
					{
						vertex_textures.push_back(vertex_info_data);
					}
				}
				else if (first_token == "f")
				{
					// a face!
					size_t face_vert_count = tokens.size() - 1;
					if (face_vert_count != 3 && face_vert_count != 4)
					{
						std::cout << "Currently, only faces with three or four vertices are supported." << std::endl;
						break;
					}
					vertex face_verts[4];
					for (size_t token_index = 0; token_index < face_vert_count; token_index++)
					{
						std::string& vertex_numbers = tokens[1 + token_index];
						std::istringstream vertex_numbers_stream(vertex_numbers);
						std::string vertex_number_string;
						size_t i = 0;
						while (std::getline(vertex_numbers_stream, vertex_number_string, '/'))
						{
							int vertex_index = std::stoi(vertex_number_string) - 1;
							if (i == 0)
							{
								// this is a vertex coord
								float3& matching = vertex_coords[vertex_index];
								face_verts[token_index].x = matching.x;
								face_verts[token_index].y = matching.y;
								face_verts[token_index].z = matching.z;
							}
							else if (i == 1)
							{
								// this is a vertex texture
								float3& matching = vertex_textures[vertex_index];
								face_verts[token_index].u = matching.x;
								face_verts[token_index].v = matching.y;
							}
							else if (i == 2)
							{
								// this is a vertex normal
								float3& matching = vertex_normals[vertex_index];
								face_verts[token_index].nx = matching.x;
								face_verts[token_index].ny = matching.y;
								face_verts[token_index].nz = matching.z;
							}
							i++;
						}
					}

					if (face_vert_count == 3)
					{
						// triangle
						vertices.push_back(face_verts[0]);
						vertices.push_back(face_verts[1]);
						vertices.push_back(face_verts[2]);
					}
					else if (face_vert_count == 4)
					{
						// quadrilateral (two triangles)

						// first triangle
						vertices.push_back(face_verts[0]);
						vertices.push_back(face_verts[1]);
						vertices.push_back(face_verts[3]);

						// second triangle
						vertices.push_back(face_verts[1]);
						vertices.push_back(face_verts[2]);
						vertices.push_back(face_verts[3]);
					}
				}
				else if (first_token == "o" || first_token == "g")
				{
					// no object or group support
					continue;
				}
				else if (first_token == "mtllib")
				{
					// load mtl file

					// TODO: relative path support
					std::string mtlpath = "models/" + tokens[1];

					materials = render::MaterialFactory::MTL(content_manager, mtlpath.c_str());
				}
				else if (first_token == "usemtl")
				{
					// change active material
					std::string material_name = tokens[1];
					Material& material = materials[material_name];
					material_changes.push_back(std::make_tuple(vertices.size(), material));
					continue;
				}
				else if (first_token == "s")
				{
					// no smooth-shading support
					continue;
				}
				else if (first_token == "")
				{
					// blank line
					continue;
				}
				else
				{
					std::cout << "Could not parse line: " << line << std::endl;
					break;
				}
			}

			free(file_data);

			std::vector<std::tuple<size_t, Material>> material_groups;

			// std::cout << material_changes.size() << " material changes" << std::endl;
			Material active_material;
			size_t last_change = 0;
			for (auto material_change : material_changes)
			{
				size_t index = std::get<0>(material_change);
				Material& material = std::get<1>(material_change);

				// std::cout << "  index " << std::get<0>(material_change) << " starts material " << std::get<1>(material_change) << std::endl;

				if (index == 0)
				{
					// skip the first one
					active_material = material;
					continue;
				}

				size_t count = index - last_change;
				material_groups.push_back(std::make_tuple(count, active_material));
				last_change = index;

				active_material = material;
			}

			// add the last one
			size_t count = vertices.size() - last_change;
			if (count > 0)
			{
				material_groups.push_back(std::make_tuple(count, active_material));
			}

			// convert vertices vector into float array
			size_t vertices_stride = 8;
			size_t vertices_size = vertices.size() * vertices_stride;
			float * vertices_data = (float *) malloc(vertices_size * sizeof(float));

			auto material_change = material_changes.begin();
			size_t count_in_this_material = 0;

			for (size_t i = 0; i < vertices.size(); i++)
			{
				// std::cout << "vertex " << i << ": " << vertices[i].x << ", " << vertices[i].y << ", " << vertices[i].z << std::endl;
				vertices_data[(i * vertices_stride) + 0] = vertices[i].x;
				vertices_data[(i * vertices_stride) + 1] = vertices[i].y;
				vertices_data[(i * vertices_stride) + 2] = vertices[i].z;
				vertices_data[(i * vertices_stride) + 3] = vertices[i].nx;
				vertices_data[(i * vertices_stride) + 4] = vertices[i].ny;
				vertices_data[(i * vertices_stride) + 5] = vertices[i].nz;
				vertices_data[(i * vertices_stride) + 6] = vertices[i].u;
				vertices_data[(i * vertices_stride) + 7] = vertices[i].v;

				count_in_this_material++;
			}

			// std::cout << material_groups.size() << " material groups" << std::endl;
			// for (auto material_group : material_groups)
			// {
			// 	std::cout << "  " << std::get<0>(material_group) << " vertices in material " << std::get<1>(material_group) << std::endl;
			// }

			Mesh * object = new Mesh(shader, vertices_data, vertices_size * sizeof(float), vertices.size());
			object->SetMaterials(material_groups);
			return object;
		}
	}
}