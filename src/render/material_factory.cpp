#include "render/material_factory.hpp"

#include <cstddef>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

#include "physfs.h"

namespace duckhacker
{
	namespace render
	{
		static bool is_unwanted_whitespace(unsigned char c)
		{
			return (c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == '\f');
		}

		std::map<std::string, Material> MaterialFactory::MTL(const char * path)
		{
			PHYSFS_File * file = PHYSFS_openRead(path);

			// we read the file in to a buffer
			// TODO: can we make this better by reading chunk-by-chunk from the file?
			PHYSFS_sint64 file_size = PHYSFS_fileLength(file);
			char * file_data = (char *) malloc(file_size * sizeof(char));
			PHYSFS_readBytes(file, file_data, file_size);

			std::map<std::string, Material> result;

			std::string current_material;
			Material m;
			bool have_material = false;

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
				if (first_token == "newmtl")
				{
					if (have_material)
					{
						result[current_material] = m;
					}

					current_material = tokens[1];
					have_material = true;
				}
				else if (first_token == "illum")
				{
					if (tokens[1] != "2")
					{
						std::cout << "Currently, only illumination model 2 is supported." << std::endl;
					}
				}
				else if (first_token == "Ns")
				{
					std::cout << "Currently, shininess is not supported." << std::endl;
					// float shininess = std::stof(tokens[1]);
					// m.SetShininess(shininess);
				}
				else if (first_token == "Ka" || first_token == "Kd" || first_token == "Ks" || first_token == "Ke")
				{
					glm::vec4 color;
					color.r = std::stof(tokens[1]);
					color.g = std::stof(tokens[2]);
					color.b = std::stof(tokens[3]);
					color.a = 1.0f;

					if (first_token == "Ka")
					{
						m.SetAmbient(color);
					}
					else if (first_token == "Kd")
					{
						m.SetDiffuse(color);
					}
					else if (first_token == "Ks")
					{
						m.SetSpecular(color);
					}
					else if (first_token == "Ke")
					{
						if (color.x != 0.0f || color.y != 0.0f || color.z != 0.0f)
						{
							std::cout << "Currently, emission is not supported." << std::endl;
						}
					}
				}
				else if (first_token == "Ni")
				{
					std::cout << "Currently, refraction is not supported." << std::endl;
				}
				else if (first_token == "d")
				{
					std::cout << "Currently, transparency is not supported." << std::endl;
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

			if (have_material)
			{
				result[current_material] = m;
			}

			free(file_data);
			PHYSFS_close(file);

			return result;
		}
	}
}